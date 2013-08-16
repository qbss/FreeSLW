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
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

// Code Sample 2: 3D Sound, Listener and Environments

#include <windows.h> // Sleep()
#include <stdio.h>
#include <math.h>

#include "FreeSLW.h"

#pragma comment(lib,"FreeSLW.lib")

void ErrorCallback(const char* message)
{
	printf("Error: %s\n",message);
}

int main(int argv, char** argc)
{
	freeslw::SetErrorCallback(ErrorCallback);
	freeslw::AudioInterface* lib = freeslw::GetInterface(freeslw::AS_OPENAL);

	if (lib)
	{
		printf("Found Audio Interface...\n");

		lib->GetListener()->SetEnvironment(freeslw::AE_ARENA);

		freeslw::Sound* snd = lib->LoadSound("test1.ogg");
		if (snd)
		{
			printf("Playing Sound (%0.1fs)...",snd->GetPlayDuration());
			snd->Play();

			if (snd->GetPlayDuration() > 0.5f)
			{
				while (snd->IsPlaying())
				{
					// we rotate the listener some bit
					float angle = 1.5f * (snd->GetPlayTime() / snd->GetPlayDuration());
					float ax = cos(angle);
					float az = -sin(angle);

					lib->GetListener()->SetPosition(0.0f,0.0f,-10.0f);
					lib->GetListener()->SetOrientation(ax,0.0f,az,0.0f,1.0f,0.0f);

					lib->Update();
					Sleep(50);
				}
			}
			Sleep(2000); // let the eko fade out
			printf("Done\n");

			lib->ReleaseSound(snd);
		}
		else
		{
			printf("Error: Failed to load Sound\n");
		}

		freeslw::ReleaseInterface();
	}
	else
	{
		printf("Error: Failed to retrieve AudioInterface\n");
	}

	printf("Press Return to exit..."); getchar();
	return 0;
}