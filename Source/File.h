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

#ifndef __FILE_H
#define __FILE_H

#include "Common.h"

class File
{
private:
	char* data;
	size_t length;

public:
	File() : data(0), length(0)
	{
	}

	File(const char* file) : data(0), length(0)
	{
		Read(file);
	}

	~File()
	{
		Close();
	}

	bool Read(const char* file)
	{
		Close();

		FILE* fp = 0;
		if (fopen_s(&fp,file,"rb") == 0)
		{
			fseek(fp,0,SEEK_END);
			length = ftell(fp);
			fseek(fp,0,SEEK_SET);
			data = new char[length];
			fread(data,sizeof(char),length,fp);
			fclose(fp);
			return true;
		}

		return false;
	}

	void Close()
	{
		delete [] data;
		data = 0;
	}

	INLINE char* GetData() const
	{
		return data;
	}

	INLINE size_t GetLength() const
	{
		return length;
	}
};

#endif