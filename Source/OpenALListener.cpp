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

#include "OpenALListener.h"

namespace openal
{
	void OpenALListener::EAXEnv(_EAXLISTENERPROPERTIES& env,
								unsigned long dwEnvironment,
								float flEnvironmentSize,
								float flEnvironmentDiffusion,
								long lRoom,
								long lRoomHF,
								float flRoomRolloffFactor,
								float flAirAbsorptionHF,
								long lReflections,
								float flReflectionsDelay,
								long lReverb,
								float flReverbDelay,
								float flDecayTime,
								float flDecayHFRatio)
	{
		env.lRoom = lRoom;
		env.lRoomHF = lRoomHF;
		env.flRoomRolloffFactor = flRoomRolloffFactor;
		env.flDecayTime = flDecayTime;
		env.flDecayHFRatio = flDecayHFRatio;
		env.lReflections = lReflections;
		env.flReflectionsDelay = flReflectionsDelay;
		env.lReverb = lReverb;
		env.flReverbDelay = flReverbDelay;
		env.dwEnvironment = dwEnvironment;
		env.flEnvironmentSize = flEnvironmentSize;
		env.flEnvironmentDiffusion = flEnvironmentDiffusion;
		env.flAirAbsorptionHF = flAirAbsorptionHF;
		env.dwFlags = EAXLISTENER_DEFAULTFLAGS;
	}

	OpenALListener::OpenALListener() : activeEnvironment(freeslw::AE_NONE), hasEAX(false), alEAXSet(0), alEAXGet(0)
	{
		SetDistanceModel(freeslw::DM_INVERSE_DISTANCE_CLAMPED);

		if (alIsExtensionPresent("EAX2.0") == AL_TRUE)
		{
			alEAXSet = (EAXSet)alGetProcAddress("EAXSet");
			alEAXGet = (EAXGet)alGetProcAddress("EAXGet");
			
			if (alEAXSet && alEAXGet)
			{
				hasEAX = true;
				alEAXGet(&DSPROPSETID_EAX_ListenerProperties,DSPROPERTY_EAXLISTENER_ALLPARAMETERS,0,&defaultEAX,sizeof(_EAXLISTENERPROPERTIES));

				EAXEnv(eaxProperties[freeslw::AE_GENERIC],EAX_ENVIRONMENT_GENERIC,7.5f,1.0f,-1000,-100,0.0f,-5.0f,-2602,0.007f,200,0.011f,1.49f,0.83f);
				EAXEnv(eaxProperties[freeslw::AE_PADDEDCELL],EAX_ENVIRONMENT_PADDEDCELL,1.4f,1.0f,-1000,-6000,0.0f,-5.0f,-1204,0.002f,207,0.02f,0.17f,0.1f);
				EAXEnv(eaxProperties[freeslw::AE_ROOM],EAX_ENVIRONMENT_ROOM,1.9f,1.0f,-1000,-454,0.0f,-5.0f,-1646,0.002f,53,0.003f,0.4f,0.83f);
				EAXEnv(eaxProperties[freeslw::AE_BATHROOM],EAX_ENVIRONMENT_BATHROOM,1.4f,1.0f,-1000,-1200,0.0f,-5.0f,-370,0.007f,1030,0.011f,1.49f,0.54f);
				EAXEnv(eaxProperties[freeslw::AE_LIVINGROOM],EAX_ENVIRONMENT_LIVINGROOM,2.5f,1.0f,-1000,-6000,0.0f,-5.0f,-1376,0.003f,-1104,0.004f,0.5f,0.1f);
				EAXEnv(eaxProperties[freeslw::AE_STONEROOM],EAX_ENVIRONMENT_STONEROOM,11.6f,1.0f,-1000,-300,0.0f,-5.0f,-711,0.012f,83,0.017f,2.31f,0.64f);
				EAXEnv(eaxProperties[freeslw::AE_AUDITORIUM],EAX_ENVIRONMENT_AUDITORIUM,21.6f,1.0f,-1000,-476,0.0f,-5.0f,-789,0.02f,-289,0.03f,4.32f,0.59f);
				EAXEnv(eaxProperties[freeslw::AE_CONCERTHALL],EAX_ENVIRONMENT_CONCERTHALL,19.6f,1.0f,-1000,-500,0.0f,-5.0f,-1230,0.02f,-2,0.029f,3.92f,0.7f);
				EAXEnv(eaxProperties[freeslw::AE_CAVE],EAX_ENVIRONMENT_CAVE,14.6f,1.0f,-1000,0,0.0f,-5.0f,-602,0.015f,-302,0.022f,2.91f,1.3f);
				EAXEnv(eaxProperties[freeslw::AE_ARENA],EAX_ENVIRONMENT_ARENA,36.2f,1.0f,-1000,-698,0.0f,-5.0f,-1166,0.02f,16,0.03f,7.24f,0.33f);
				EAXEnv(eaxProperties[freeslw::AE_HANGAR],EAX_ENVIRONMENT_HANGAR,50.3f,1.0f,-1000,-1000,0.0f,-5.0f,-602,0.02f,198,0.03f,10.05f,0.23f);
				EAXEnv(eaxProperties[freeslw::AE_CARPETEDHALLWAY],EAX_ENVIRONMENT_CARPETEDHALLWAY,1.9f,1.0f,-1000,-4000,0.0f,-5.0f,-1831,0.002f,-1630,0.03f,0.3f,0.1f);
				EAXEnv(eaxProperties[freeslw::AE_HALLWAY],EAX_ENVIRONMENT_HALLWAY,1.8f,1.0f,-1000,-300,0.0f,-5.0f,-1219,0.007f,441,0.011f,1.49f,0.59f);
				EAXEnv(eaxProperties[freeslw::AE_STONECORRIDOR],EAX_ENVIRONMENT_STONECORRIDOR,13.5f,1.0f,-1000,-237,0.0f,-5.0f,-1214,0.013f,395,0.02f,2.7f,0.79f);
				EAXEnv(eaxProperties[freeslw::AE_ALLEY],EAX_ENVIRONMENT_ALLEY,7.5f,0.3f,-1000,-270,0.0f,-5.0f,-1204,0.007f,-4,0.011f,1.49f,0.86f);
				EAXEnv(eaxProperties[freeslw::AE_FOREST],EAX_ENVIRONMENT_FOREST,38.0f,0.3f,-1000,-33000,0.0f,-5.0f,-2560,0.162f,-229,0.088f,1.49f,0.54f);
				EAXEnv(eaxProperties[freeslw::AE_CITY],EAX_ENVIRONMENT_CITY,7.5f,0.5f,-1000,-800,0.0f,-5.0f,-2273,0.007f,-1691,0.011f,1.49f,0.67f);
				EAXEnv(eaxProperties[freeslw::AE_MOUNTAINS],EAX_ENVIRONMENT_MOUNTAINS,100.0f,0.27f,-1000,-2500,0.0f,-5.0f,-2780,0.3f,-1434,0.1f,1.49f,0.21f);
				EAXEnv(eaxProperties[freeslw::AE_QUARRY],EAX_ENVIRONMENT_QUARRY,17.5f,1.0f,-1000,-1000,0.0f,-5.0f,-1000,0.061f,500,0.025f,1.49f,0.83f);
				EAXEnv(eaxProperties[freeslw::AE_PLAIN],EAX_ENVIRONMENT_PLAIN,42.5f,0.21f,-1000,-2000,0.0f,-5.0f,-2466,0.179f,-1925,0.1f,1.49f,0.5f);
				EAXEnv(eaxProperties[freeslw::AE_PARKINGLOT],EAX_ENVIRONMENT_PARKINGLOT,8.3f,1.0f,-1000,0,0.0f,-5.0f,-1363,0.008f,-1153,0.012f,1.65f,1.5f);
				EAXEnv(eaxProperties[freeslw::AE_SEWERPIPE],EAX_ENVIRONMENT_SEWERPIPE,1.7f,0.8f,-1000,-1000,0.0f,-5.0f,429,0.014f,1023,0.021f,2.81f,0.14f);
				EAXEnv(eaxProperties[freeslw::AE_UNDERWATER],EAX_ENVIRONMENT_UNDERWATER,1.8f,1.0f,-1000,-4000,0.0f,-5.0f,-449,0.007f,1700,0.011f,1.49f,0.1f);
				EAXEnv(eaxProperties[freeslw::AE_DRUGGED],EAX_ENVIRONMENT_DRUGGED,1.9f,0.5f,-1000,0,0.0f,-5.0f,-115,0.002f,985,0.03f,8.39f,1.39f);
				EAXEnv(eaxProperties[freeslw::AE_DIZZY],EAX_ENVIRONMENT_DIZZY,1.8f,0.6f,-1000,-400,0.0f,-5.0f,-1713,0.02f,-613,0.03f,17.23f,0.56f);
				EAXEnv(eaxProperties[freeslw::AE_PSYCHOTIC],EAX_ENVIRONMENT_PSYCHOTIC,1.0f,0.5f,-1000,-151,0.0f,-5.0f,-626,0.02f,774,0.03f,7.56f,0.91f);
			}
			else
			{
				freeslw::ErrorPrintf("Failed to retrieve EAXGet and/or EAXSet functions");
			}
		}
	}

	OpenALListener::~OpenALListener()
	{
	}

	void OpenALListener::SetPosition(float x, float y, float z)
	{
		float p[3] = { x, y, z };
		alListenerfv(AL_POSITION,p);
	}

	void OpenALListener::SetPosition(const float* xyz)
	{
		alListenerfv(AL_POSITION,xyz);
	}

	void OpenALListener::GetPosition(float& x, float& y, float& z) const
	{
		float p[3];
		alGetListenerfv(AL_POSITION,p);
		x=p[0]; y=p[1]; z=p[2];
	}

	void OpenALListener::SetOrientation(float ax, float ay, float az, float ux, float uy, float uz)
	{
		float p[6] = { ax, ay, az, ux, uy, uz };
		alListenerfv(AL_ORIENTATION,p);
	}

	void OpenALListener::SetOrientation(const float* a, const float* u)
	{
		float p[6] = { *a, *(a+1), *(a+2), *u, *(u+1), *(u+2) };
		alListenerfv(AL_ORIENTATION,p);
	}

	void OpenALListener::GetOrientation(float& ax, float& ay, float& az, float& ux, float& uy, float& uz) const
	{
		float p[6];
		alGetListenerfv(AL_ORIENTATION,p);
		ax=p[0]; ay=p[1]; az=p[2];
		ux=p[3]; uy=p[4]; uz=p[5];
	}

	void OpenALListener::SetVelocity(float x, float y, float z)
	{
		float p[3] = { x, y, z };
		alListenerfv(AL_VELOCITY,p);
	}

	void OpenALListener::SetVelocity(const float* xyz)
	{
		alListenerfv(AL_VELOCITY,xyz);
	}

	void OpenALListener::GetVelocity(float& x, float& y, float& z) const
	{
		float p[3];
		alGetListenerfv(AL_VELOCITY,p);
		x=p[0]; y=p[1]; z=p[2];
	}

	void OpenALListener::SetDistanceModel(freeslw::distanceModel_e m)
	{
		if (activeDistanceModel != m)
		{
			activeDistanceModel = m;
			switch (activeDistanceModel)
			{
				case freeslw::DM_INVERSE_DISTANCE: alDistanceModel(AL_INVERSE_DISTANCE); break;
				case freeslw::DM_INVERSE_DISTANCE_CLAMPED: alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED); break;
				case freeslw::DM_LINEAR_DISTANCE: alDistanceModel(AL_LINEAR_DISTANCE); break;
				case freeslw::DM_LINEAR_DISTANCE_CLAMPED: alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED); break;
				case freeslw::DM_EXPONENT_DISTANCE: alDistanceModel(AL_EXPONENT_DISTANCE); break;
				case freeslw::DM_EXPONENT_DISTANCE_CLAMPED: alDistanceModel(AL_EXPONENT_DISTANCE_CLAMPED); break;
			};
		}
		
	}

	freeslw::distanceModel_e OpenALListener::GetDistanceModel() const
	{
		return activeDistanceModel;
	}

	void OpenALListener::SetDopplerParameters(float factor, float velocity)
	{
		alDopplerFactor(factor);
		alDopplerVelocity(velocity);
	}

	void OpenALListener::GetDopplerParameters(float& factor, float& velocity) const
	{
		factor = alGetFloat(AL_DOPPLER_FACTOR);
		velocity = alGetFloat(AL_DOPPLER_VELOCITY);
	}

	void OpenALListener::SetEnvironment(freeslw::audioEnvironment_e env)
	{
		if (hasEAX && activeEnvironment != env)
		{
			activeEnvironment = env;
			if (env == freeslw::AE_NONE)
			{
				alEAXSet(&DSPROPSETID_EAX_ListenerProperties,DSPROPERTY_EAXLISTENER_ALLPARAMETERS,0,&defaultEAX,sizeof(_EAXLISTENERPROPERTIES));
			}
			else
			{
				alEAXSet(&DSPROPSETID_EAX_ListenerProperties,DSPROPERTY_EAXLISTENER_ALLPARAMETERS,0,&eaxProperties[env],sizeof(_EAXLISTENERPROPERTIES));
			}
		}
	}

	freeslw::audioEnvironment_e OpenALListener::GetEnvironment() const
	{
		return activeEnvironment;
	}
}