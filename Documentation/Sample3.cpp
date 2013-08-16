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

// Code Sample 3: Groups

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

		freeslw::Sound* snd0 = lib->LoadSound("test0.wav",1);
		freeslw::Sound* snd1 = lib->LoadSound("test1.ogg",2);

		if (snd0 && snd1)
		{
			int counter = 0;
			lib->SetGroup(1,false);
			lib->SetGroup(2,true);

			snd1->Play();

			while (snd0->IsPlaying() || snd1->IsPlaying() || snd1->IsPaused())
			{
				if (counter == 6)
				{
					// lets switch the active group of sounds that should play
					lib->SetGroup(1,true);
					lib->SetGroup(2,false);
					snd0->Play();
				}

				if (counter == 12)
				{
					// lets switch back the active group of sounds that should play
					lib->SetGroup(1,false);
					lib->SetGroup(2,true);
				}

				lib->Update();
				Sleep(500);
				counter++;
			}
			
			lib->ReleaseSound(snd0);
			lib->ReleaseSound(snd1);
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