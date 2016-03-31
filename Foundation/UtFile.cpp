#include "UtFile.h"

#ifdef _MSC_VER
#pragma warning( disable : 4996 ) // _CRT_SECURE_NO_DEPRECATE
#endif

std::vector<std::string> gPathStr;

//----------------------------------------------------------------------------------
std::wstring BuildFolderPath(const wchar_t *folder)
{
	std::wstring outPath;

	TCHAR szPath[MAX_PATH];

	if (folder)
	{
		DWORD length = GetModuleFileName(nullptr, szPath, sizeof(szPath));

		if (length > 0)
		{
			std::wstring exec_path(szPath);

			std::wstring::size_type pos = exec_path.find_last_of(L"/\\", exec_path.find_last_of(L"/\\") - 1);

			assert(pos != std::wstring::npos);

			outPath = exec_path.substr(0, pos) + L'\\' + folder;
		}
	}

	return outPath;
}

//----------------------------------------------------------------------------------
bool RegisterPath(const char *path)
{
	TCHAR szPath[MAX_PATH]; 
	char  pmbbuf[MAX_PATH];

	bool bResult = false;

	if (path)
	{
		DWORD length = GetModuleFileName(nullptr, szPath, sizeof(szPath));

		if (length > 0)
		{
			wcstombs(pmbbuf, szPath, MAX_PATH);

			std::string exec_path(pmbbuf);

			std::string::size_type pos = exec_path.find_last_of("/\\", exec_path.find_last_of("/\\") - 1);

			assert(pos != std::string::npos);

			std::string new_path = exec_path.substr(0, pos) + '\\' + path;

			std::vector<std::string>::const_iterator IterFind = std::find(gPathStr.begin(), gPathStr.end(), new_path);

			if (IterFind == gPathStr.end()){
				gPathStr.push_back(new_path);
			}

			bResult = true;
		}
	}
	return bResult; 
}

//-------------------------------------------------------------------
bool UnRegisterPath(const char* path)
{
	TCHAR szPath[MAX_PATH]; 
	char  pmbbuf[MAX_PATH];

	DWORD length = GetModuleFileName(nullptr, szPath, sizeof(szPath));

	if (!path || length == 0) {
		return false;
	}

	wcstombs(pmbbuf, szPath, MAX_PATH);

	std::string exec_path(pmbbuf);

	std::string::size_type pos = exec_path.rfind("bin");

	std::string new_path = exec_path.substr(0,pos) + path;

	std::vector<std::string>::iterator IterFind = std::find(gPathStr.begin(), gPathStr.end(), new_path);

	if (IterFind != gPathStr.end()){
		gPathStr.erase(IterFind);
	}

    return false;
}

//-------------------------------------------------------------------
TUInt32 GetFileSize(const char * name)
{
#ifndef SEEK_END
#define SEEK_END 2
#endif

    FILE * File = fopen(name, "rb");

	if(!File){
		return 0;
	}
	fseek(File, 0, SEEK_END);
    TUInt32 eof_ftell = ftell(File);
    fclose(File);
	return eof_ftell;
}

//-------------------------------------------------------------------
bool FileExists(const char * filename)
{
	if(!filename || !strlen(filename)){
		return false;
	}

	FILE * File = fopen(filename, "rb");
	if (!File) { return false; }
	fclose(File);
	return true;
}

//-------------------------------------------------------------------
bool FindFile(const char* filename, std::string* OutFile /*= NULL*/, std::string* OutTag /*= NULL*/)
{
	if (!filename){
		return false;
	}

    if (FileExists(filename))
	{
		if (OutFile)
		{
			*OutFile = filename;
			return true;
		}
	}

	for (size_t i = 0; i < gPathStr.size(); i++)
	{
		std::string   tmpFile = gPathStr[i];
        TUInt32 lastChar = (unsigned int)tmpFile.length();

		if (lastChar)
		{
			lastChar--;

			if (tmpFile[lastChar]!='/' && tmpFile[lastChar]!='\\')
			{
				tmpFile+="\\";
				tmpFile+=filename;
                
				if (FileExists(tmpFile.c_str()))
				{
				    if(OutFile) *OutFile = tmpFile;
					return true;
				}
			}
		}
	}
	return false;
}

//-------------------------------------------------------------------
bool GetTagByPath(std::string* InFile, std::string* OutTag)
{
	if (InFile && OutTag)
	{
		// @TODO Convert filepath To Tag
	}
	return false;
}