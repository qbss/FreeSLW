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

#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

bool IsFileExtension(const char* file, const char* extension);
void GetFileExtension(const char* file, char* extension, size_t maxLength);
void FixFileSeparators(char* file);

#endif