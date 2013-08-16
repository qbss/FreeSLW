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

#ifndef __OPENAL_GLOBALS_H
#define __OPENAL_GLOBALS_H

#include "Common.h"

#ifndef OPENAL
#define OPENAL
#endif

extern "C"
{
	#include "al/al.h"
	#include "al/alc.h"
	#include "al/alut.h"
	#include "al/efx.h"
	#include "al/eax.h"
	#include "al/eaxac3.h"
	#include "Ogg/ogg.h"
	#include "Vorbis/codec.h"
	#include "Vorbis/vorbisenc.h"
	#include "Vorbis/vorbisfile.h"
}

#include "FreeSLW.h"

#define MAX_OPENAL_SOURCES	16
#define NO_SOURCE			-1

namespace freeslw
{
	void ErrorPrintf(const char* str, ...);
};

namespace openal
{
	class OpenALStaticSound;
	class OpenALStreamSound;
	class Buffer;

	extern ALCdevice* alDevice;
	extern ALCcontext* alContext;

	extern ALuint sources[MAX_OPENAL_SOURCES];
	extern bool freeSources[MAX_OPENAL_SOURCES];

	extern std::list<OpenALStreamSound*> streamPlayList;
	extern std::list<OpenALStaticSound*> staticPlayList;

	extern std::list<freeslw::Sound*> soundList;

	extern std::list<Buffer*> bufferList;

	extern bool groupActive[256];

	bool ContextInit(const char* type);

	int AllocSource(unsigned char group, unsigned char priority);
	void FreeSource(int number);
	
	Buffer* GetBuffer(const char* name);
	Buffer* LoadBuffer(const char* name, const char* type, const char* data, size_t size);
	Buffer* CreateBuffer(const char* name, const void* data, size_t size, freeslw::soundFormat_e format, int frequency);

	Buffer* LoadWAV(const char* name, const void* data, size_t size);
	Buffer* LoadOGG(const char* name, const void* data, size_t size);
}

#endif