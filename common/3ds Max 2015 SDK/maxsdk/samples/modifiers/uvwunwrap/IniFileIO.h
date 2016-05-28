//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include <maxapi.h>
#include <map>


// A basic tool to read/write .ini configure file.
/* A IniFileIO is a light-weight configure tool that reads/writes the content of .ini file.
	The items you want to write into the .ini file are firstly cached into the instance of IniFileIO which
	has a container to hold the items. When all the items are ready, you can use SaveAs function to write 
	all the items into the .ini file.
For example:
		\code
		    void theFunctionTested()
			{
				IniFileIO iniFileIO;
				iniFileIO.InitCacheFromIniFile(_T("fileName.ini"));
				....
				iniFileIO.AddSectionKeyValue(section0, key0, value0);
				iniFileIO.AddSectionKeyValue(section0, key1, value1);
				iniFileIO.AddSectionKeyValue(section1, key2, value2);
				.....
				iniFileIO.SaveAs(_T("fileName.ini"));
			}
		\encode
*/
class IniFileIO : public MaxHeapOperators
{
	typedef std::map<TSTR, TSTR> MapKeyValues;
	typedef std::pair<TSTR, TSTR> TStrToTStrPairs;
	typedef std::map<TSTR, MapKeyValues> MapSections;
	typedef std::pair<TSTR, MapKeyValues> TStrToKeyValuePairs;

public:
	// Construct the IniFileIO Object
	IniFileIO();
	// Destruct the IniFileIO Object
	~IniFileIO();
	// This method lets you load the existing .ini file and the items will be cached
	bool InitCacheFromIniFile(const TSTR& strFileName);
	// This method lets you save the cached items into the .ini file
	bool SaveAs(const TSTR& strFileName);
	// This method lets you get the value from the section and key in the .ini file
	TSTR GetValueFromSectionKey(const TSTR& strSection, const TSTR& strKey);
	// This method lets you get the number value(int) from the key in the .ini file
	int GetIntFromSectionKey(const TSTR& strSection, const TSTR& strKey);
	// This method lets you get the number value(float) from the key in the .ini file
	float GetFloatFromSectionKey(const TSTR& strSection, const TSTR& strKey);
	// This method lets you add the item(section/key/value) into the items cache
	bool AddSectionKeyValue(const TSTR& strSection, const TSTR& strKey, const TSTR& strValue);
	// return the ini file name
	TSTR& GetIniFileName();

private:
	bool LoadIniFile(const TSTR& strFileName);
	bool TrimLeftInvalidChars(const TCHAR* strLine, TCHAR* strNewLine, int len);
	bool TrimRightInvalidChars(TCHAR* strNewLine);
	bool TrimInvalidChars(const TCHAR* strLine, TCHAR* strNewLine, int len);
	// we need to remove the return/space etc chars at the head and the end of the old string and a new string can be got.
	bool TrimInvalidString(const TSTR& strOld, TSTR& strNew);
	bool IsSectionLine(const TSTR& strLine);
	bool IsKeyValueLine(const TSTR& strLine);
	bool IsWholeLineBuffer(const TCHAR* strLine) const;

	bool GetSectionContent(const TSTR& strLine, TSTR& strSectionContext);
	bool GetKeyValue(const TSTR& strLine, TSTR& strKey, TSTR& strValue);

private:
	TSTR mStrFileName;
	MapSections mMapSections;

};
