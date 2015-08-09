//----------------------------------------------------------------------------------------------
// OpenES: Open Entertainment System
// Copyright (C) 2010  Yura Gunko email: yura.gunko@gmail.com
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------------------------
#ifndef _utfile_h_
#define _utfile_h_

#pragma once

#include "platform_specific.h"

#define FILE_SAVE   false       //!< Fileselect parameter.
#define FILE_LOAD   true        //!< Fileselect parameter.

// File access
#define FILE_READ               0x1
#define FILE_WRITE              0x2
#define FILE_SHAREREAD          0x4
#define FILE_SHAREWRITE         0x8

// File attrib
#define FILE_ATTRB_READONLY     0x00000001  
#define FILE_ATTRB_HIDDEN       0x00000002  
#define FILE_ATTRB_SYSTEM       0x00000004  
#define FILE_ATTRB_DIRECTORY    0x00000008  
#define FILE_ATTRB_ARCHIVE      0x00000010  
#define FILE_ATTRB_NORMAL       0x00000020  

// File open flags
enum FILE_CREATEATTRB
{
	FILE_CREATE_NEW             = 1,                            // Create the file, fails if the same filename already exists
	FILE_CREATE_ALWAYS          = 2,                            // Create or open an existing file (with truncation to zero)
	FILE_OPEN_EXISTING          = 3,                            // Open an existing file, fails if the file doesn't exist
	FILE_OPEN_ALWAYS            = 4,                            // Open an existing file, or create one if it doens't exist
	FILE_OPFORCEDW              = 0x7FFFFFFF,
};

// File seek flags
enum FILE_SEEK
{
	FILE_FROM_BEGIN             = 1,
	FILE_FROM_CURRENT           = 2, 
	FILE_FROM_END               = 4,
	FILE_FROM_FORCE             = 0x7FFFFFFF,
};

std::wstring EXPORT BuildFolderPath(const wchar_t *folder);

bool	EXPORT RegisterPath(const char* path);
bool	EXPORT UnRegisterPath(const char* path);

udword	EXPORT GetFileSize(const char * name);     
bool	EXPORT FileExists(const char * filename);
bool	EXPORT FindFile(const char* filename, std::string* OutFile = nullptr, std::string* OutTag = nullptr);
bool	EXPORT GetTagByPath(std::string* InFile, std::string* OutTag);

#endif //_utfile_h_