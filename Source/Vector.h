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

#ifndef __VECTOR_H
#define __VECTOR_H

#include "Common.h"

template<class T, const int INCR_COUNT = 8>
class Vector
{
private:
	T* data;
	long count;
	long size;
	
private:
	INLINE void Realloc()
	{
		if (count >= size)
		{
			size += (((count - size) / INCR_COUNT) + 1) * INCR_COUNT;
			data = (T*)realloc(data,sizeof(T) * size);
		}
		else if (count < size - INCR_COUNT)
		{
			size = ((count / INCR_COUNT) * INCR_COUNT) + INCR_COUNT;
			data = (T*)realloc(data,sizeof(T) * size);
		}
	}
	
public:
	Vector() : size(INCR_COUNT), count(0)
	{
		data = (T*)malloc(sizeof(T) * size);
	}
		
	~Vector()
	{
		free(data);
	}
		
	INLINE void Push(const T& p)
	{
		count++;
		Realloc();
		data[count-1] = p;
	}

	INLINE void Append(const T* array, long array_count)
	{
		int pcount = count;
		count += array_count;
		Realloc();
		memcpy(data + pcount,array,array_count * sizeof(T));
	}
		
	INLINE void Pop()
	{
		count--;
		Realloc();
	}
		
	INLINE void Clear()
	{
		count = 0;
		size = 0;
		Realloc();
	}
		
	INLINE long Size() const
	{
		return count;
	}
		
	INLINE bool IsEmpty() const
	{
		return (Size() == 0);
	}
		
	INLINE T* GetData()
	{
		return data;
	}
		
	INLINE T& operator [] (long id)
	{
		return data[id];
	}
};

#endif