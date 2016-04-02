#pragma once

#include "OEMBase.h"
#include "TypesBase.h"
#include <string>

namespace oes
{
namespace common_base
{
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
            FILE_CREATE_NEW = 1,                            // Create the file, fails if the same filename already exists
            FILE_CREATE_ALWAYS = 2,                            // Create or open an existing file (with truncation to zero)
            FILE_OPEN_EXISTING = 3,                            // Open an existing file, fails if the file doesn't exist
            FILE_OPEN_ALWAYS = 4,                            // Open an existing file, or create one if it doens't exist
            FILE_OPFORCEDW = 0x7FFFFFFF,
        };

        // File seek flags
        enum FILE_SEEK
        {
            FILE_FROM_BEGIN = 1,
            FILE_FROM_CURRENT = 2,
            FILE_FROM_END = 4,
            FILE_FROM_FORCE = 0x7FFFFFFF,
        };

        std::wstring COMMON_BASE_EXPORT BuildFolderPath(const wchar_t *folder);

        bool	COMMON_BASE_EXPORT RegisterPath(const char* path);
        bool	COMMON_BASE_EXPORT UnRegisterPath(const char* path);

        TUInt32	COMMON_BASE_EXPORT GetFileSize(const char *name);
        bool	COMMON_BASE_EXPORT FileExists(const char *filename);
        bool	COMMON_BASE_EXPORT FindFile(const char *filename, std::string *OutFile = nullptr, std::string *OutTag = nullptr);
        bool	COMMON_BASE_EXPORT GetTagByPath(std::string *InFile, std::string *OutTag);

}
}