// Copyright (c) 2012 Lukas Heise (http://www.lukasheise.com)
//
// This file is part of FreeSLW.
//
// FreeSLW is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// FreeSLW is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with FreeSLW.  If not, see <http://www.gnu.org/licenses/>.

#include "Vector.h"
#include "OpenALGlobals.h"
#include "OpenALStreamSound.h"
#include "OpenALStaticSound.h"
#include "OpenALBuffer.h"
#include "OggCallback.h"
#include "FreeSLW.h"

#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "alut.lib")
#pragma comment(lib, "eax.lib")
#pragma comment(lib, "eaxguid.lib")
#pragma comment(lib, "ogg_static.lib")
#pragma comment(lib, "vorbis_static.lib")
#pragma comment(lib, "vorbisenc_static.lib")
#pragma comment(lib, "vorbisfile_static.lib")

namespace openal
{
	ALCdevice* alDevice = 0;
	ALCcontext* alContext = 0;

	ALuint sources[MAX_OPENAL_SOURCES];
	bool freeSources[MAX_OPENAL_SOURCES];

	std::list<OpenALStreamSound*> streamPlayList;
	std::list<OpenALStaticSound*> staticPlayList;

	std::list<freeslw::Sound*> soundList;

	std::list<Buffer*> bufferList;

	bool groupActive[256];

	bool ContextInit(const char* type)
	{
		alDevice = alcOpenDevice(type);
		if (alDevice)
		{
			alContext = alcCreateContext(alDevice,NULL);

			if (alContext)
			{
				alcMakeContextCurrent(alContext);
				return true;
			}
			else 
			{
				alcCloseDevice(alDevice);
				alDevice = 0;
				return false;
			}
		}

		return false;
	}

	int AllocSource(unsigned char group, unsigned char priority)
	{
		if (groupActive[group] == false) return NO_SOURCE;

		for (int i = 0; i < MAX_OPENAL_SOURCES; i++)
		{
			if (freeSources[i])
			{
				freeSources[i] = false;
				return i;
			}
		}

		// TODO: check the sounds that are playing and use the priority system to find a source

		return NO_SOURCE;
	}

	void FreeSource(int number)
	{
		freeSources[number] = true;
	}

	Buffer* GetBuffer(const char* name)
	{
		size_t l = strlen(name);

		for (std::list<Buffer*>::iterator w = bufferList.begin(); w != bufferList.end(); w++)
		{
			if ((*w)->name_length == l && _stricmp(name,(*w)->name) == 0)
			{
				(*w)->AddRef();
				return (*w);
			}
		}

		return 0;
	}

	Buffer* LoadBuffer(const char* name, const char* type, const char* data, size_t size)
	{
		if (_stricmp(type,"wav") == 0) return LoadWAV(name,data,size);
		if (_stricmp(type,"ogg") == 0) return LoadOGG(name,data,size);
		// add more file formats here
		
		freeslw::ErrorPrintf("File extension \"%s\" is not supported",type);
		
		return 0;
	}

	Buffer* CreateBuffer(const char* name, const void* data, size_t size, freeslw::soundFormat_e format, int frequency)
	{
		Buffer* b = new Buffer(name);
		b->format = format;
		b->frequency = frequency;
		b->activeBuffer = 0;
		b->numBuffers = 1;

		size_t d = 1;
		if (format == freeslw::TA_MONO16 || format == freeslw::TA_STEREO8) d = 2; else if (format == freeslw::TA_STEREO16) d = 4;
		b->length = (size * 1000) / (frequency * d);

		ALenum f = AL_FORMAT_MONO8;
			 if (format == freeslw::TA_MONO16) f = AL_FORMAT_MONO16;
		else if (format == freeslw::TA_STEREO8) f = AL_FORMAT_STEREO8;
		else if (format == freeslw::TA_STEREO16) f = AL_FORMAT_STEREO16;

		alGenBuffers(1,&b->bufferId[0]);
		if (b->bufferId[0] == NO_BUFFER) { freeslw::ErrorPrintf("Failed to create buffer for \"%s\"",name); delete b; return 0; }

		alBufferData(b->bufferId[0],f,data,size,frequency);

		return b;
	}

	Buffer* LoadWAV(const char* name, const void* data, size_t size)
	{
		Buffer* b = new Buffer(name);

		b->bufferId[0] = alutCreateBufferFromFileImage(data,size);
		
		ALenum error = alutGetError();
		if (error != ALUT_ERROR_NO_ERROR)
		{
			freeslw::ErrorPrintf(alutGetErrorString(error));
			delete b; return 0;
		}

		if (b->bufferId[0] == NO_BUFFER)
		{
			freeslw::ErrorPrintf("Failed to create buffer for \"%s\"",name);
			delete b; return 0;
		}

		ALint format;
		ALint isize;
		ALint freq;
		ALint bits;
		ALint channels;
		ALint d = 1;

		alGetBufferi(b->bufferId[0],AL_CHANNELS,&format);
		alGetBufferi(b->bufferId[0],AL_SIZE,&isize);
		alGetBufferi(b->bufferId[0],AL_FREQUENCY,&freq);

			 if (format == AL_FORMAT_MONO8) b->format = freeslw::TA_MONO8;
		else if (format == AL_FORMAT_MONO16) b->format = freeslw::TA_MONO16;
		else if (format == AL_FORMAT_STEREO8) b->format = freeslw::TA_STEREO8;
		else if (format == AL_FORMAT_STEREO16) b->format = freeslw::TA_STEREO16;
		else freeslw::ErrorPrintf("Failed to retrieve a valid sound format (format = %d)",format);

		alGetBufferi(b->bufferId[0],AL_BITS,&bits);
		alGetBufferi(b->bufferId[0],AL_CHANNELS,&channels);
			
			 if (bits == 16 && channels == 1) d = 2;
		else if (bits == 8 && channels == 2) d = 2;
		else if (bits == 16 && channels == 2) d = 4;

		if (freq == 0)
		{
			freeslw::ErrorPrintf("\"%s\": Frequency = 0",name);
			freq = 1;
		}

		b->frequency = (int)freq;
		b->length = (isize * 1000) / (freq * d);
		b->activeBuffer = 0;
		b->numBuffers = 1;

		return b;
	}

	Buffer* LoadOGG(const char* name, const void* data, size_t size)
	{
		Buffer* b = new Buffer(name);
		OggStaticFile fp(data,size);

		long bytes;
		Vector<char> buffer;
		int bitStream;
		char arr[OGG_STATIC_BUFFER_SIZE] = {0};
		OggVorbis_File oggStream;
		vorbis_info* vorbisInfo;
		ALenum format;
		ALsizei freq;
		ALint isize;
		ALint d = 1;

		if (ov_open_callbacks(&fp,&oggStream,NULL,0,OggStaticGetCallback()) != 0)
		{
			freeslw::ErrorPrintf("Failed to load \"%s\"",name);
			delete b; return 0;
		}

		alGenBuffers(1,&b->bufferId[0]);
		if (b->bufferId[0] == NO_BUFFER)
		{
			freeslw::ErrorPrintf("Failed to create buffer for \"%s\"",name);
			delete b; return 0;
		}

		vorbisInfo = ov_info(&oggStream,-1);
		format = (vorbisInfo->channels == 1)? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		freq = vorbisInfo->rate;

		do
		{
			bytes = ov_read(&oggStream,arr,OGG_STATIC_BUFFER_SIZE,0,2,1,&bitStream);
			buffer.Append(arr,bytes / sizeof(char));
		} while (bytes > 0);

		ov_clear(&oggStream);

		alBufferData(b->bufferId[0],format,buffer.GetData(),(ALsizei)buffer.Size() * sizeof(char),freq);
		alGetBufferi(b->bufferId[0],AL_SIZE,&isize);

			 if (format == AL_FORMAT_MONO8) b->format = freeslw::TA_MONO8;
		else if (format == AL_FORMAT_MONO16) b->format = freeslw::TA_MONO16;
		else if (format == AL_FORMAT_STEREO8) b->format = freeslw::TA_STEREO8;
		else if (format == AL_FORMAT_STEREO16) b->format = freeslw::TA_STEREO16;
		else freeslw::ErrorPrintf("Failed to retrieve a valid sound format (format = %d)",format);

		if (b->format == freeslw::TA_MONO16 || b->format == freeslw::TA_STEREO8) d = 2; else if (format == freeslw::TA_STEREO16) d = 4;

		if (freq == 0)
		{
			freeslw::ErrorPrintf("\"%s\": Frequency = 0",name);
			freq = 1;
		}

		b->frequency = (int)freq;
		b->length = (isize * 1000) / (freq * d);
		b->activeBuffer = 0;
		b->numBuffers = 1;

		return b;
	}
}