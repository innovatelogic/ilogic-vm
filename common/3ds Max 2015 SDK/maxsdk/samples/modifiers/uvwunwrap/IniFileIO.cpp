//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//


#include "IniFileIO.h"
#include <shlwapi.h>

namespace
{
	const int MAX_LINE_BUF_SIZE = 100;     
}


IniFileIO::IniFileIO()
{}

IniFileIO::~IniFileIO()
{}

bool IniFileIO::InitCacheFromIniFile(const TSTR& strFileName)
{
	if((!strFileName.isNull()) && (::PathFileExists(strFileName.data())))
	{
		bool bResult = LoadIniFile(strFileName);
		return bResult;
	}
	return false;
}

bool IniFileIO::IsWholeLineBuffer(const TCHAR* strLine) const
{
	if(NULL == strLine)
	{
		return false;
	}
	size_t lengthLine = _tcscnlen(strLine, MAX_LINE_BUF_SIZE);
	if(0 == lengthLine)
	{
		return false;
	}
	else
	{
		if(strLine[lengthLine-1] == _T('\n'))
		{
			return true;
		}
	}
	return false;
}

bool IniFileIO::LoadIniFile(const TSTR& strFileName)
{
	if(strFileName.isNull())
	{
		return false;
	}

	mStrFileName = strFileName;

	FILE* pFile = _tfopen(strFileName.data(), _T("rt, ccs=UTF-16LE"));
	if(NULL == pFile)
	{
		return false;
	}

	TCHAR strReadLineBuf[MAX_LINE_BUF_SIZE] = {0};
	TSTR strWholeLineReadFromFile;
	TSTR strNewLineBuffer;
	TSTR strCurSection;
	TSTR strKey;
	TSTR strValue;
	if(NULL == _fgetts(strReadLineBuf, MAX_LINE_BUF_SIZE, pFile))
	{
		fclose(pFile);
		return false;
	}
	while(NULL != _fgetts(strReadLineBuf, MAX_LINE_BUF_SIZE, pFile))
	{
		if(!IsWholeLineBuffer(strReadLineBuf))
		{
			strWholeLineReadFromFile.Append(strReadLineBuf);
			continue;
		}
		strWholeLineReadFromFile.Append(strReadLineBuf);
		bool bTrimLineResult = TrimInvalidString(strWholeLineReadFromFile, strNewLineBuffer);
		if(!bTrimLineResult)
		{
			return false;
		}
		strWholeLineReadFromFile.Resize(0);

		if(IsSectionLine(strNewLineBuffer))
		{
			GetSectionContent(strNewLineBuffer, strCurSection);
		}
		else if(IsKeyValueLine(strNewLineBuffer))
		{
			GetKeyValue(strNewLineBuffer, strKey, strValue);
			TSTR strNewSection;
			TSTR strNewKey;
			TSTR strNewValue;
			bool bTrimSectionResult = TrimInvalidString(strCurSection, strNewSection);
			bool bTrimKeyResult = TrimInvalidString(strKey, strNewKey);
			bool bTrimValueResult = TrimInvalidString(strValue, strNewValue);
			if(!bTrimKeyResult || !bTrimValueResult || !bTrimSectionResult)
			{
				return false;
			}

			mMapSections[strNewSection][strNewKey] = strNewValue;
		}
	}
	fclose(pFile);
	return true;
}

bool IniFileIO::IsSectionLine(const TSTR& strLine)
{
	if(strLine.isNull())
	{
		return false;
	}

	int lengthOfLine = strLine.length();
	if(strLine[0] == _T('[') && strLine[lengthOfLine-1] == _T(']'))
	{
		return true;
	}

	return false;
}

bool IniFileIO::IsKeyValueLine(const TSTR& strLine)
{
	if(strLine.isNull())
	{
		return false;
	}
	const TCHAR* pStrLine = strLine.data();
	return (NULL != _tcschr(pStrLine, _T('=')));
}

bool IniFileIO::GetSectionContent(const TSTR& strLine, TSTR& strSectionContent)
{
	if(strLine.isNull())
	{
		return false;
	}

	strSectionContent = strLine.Substr(1, strLine.length()-2);
	return true;
}

bool IniFileIO::GetKeyValue(const TSTR& strLine, TSTR& strKey, TSTR& strValue)
{
	if(strLine.isNull())
	{
		return false;
	}

	strKey.Resize(0);
	strValue.Resize(0);
	const TCHAR* pStrLine = strLine.data();

	const TCHAR* pEqualPos = _tcschr(strLine.data(), _T('='));
	if(NULL == pEqualPos)
	{
		return false;
	}

	int nKeyLen = pEqualPos - pStrLine; 
	if(0 == nKeyLen)
	{
		return false;
	}
	strKey = strLine.Substr(0, nKeyLen);
	int nValueLen = strLine.length() - nKeyLen - 1;
	if(0 == nValueLen)
	{
		return false;
	}
	strValue = strLine.Substr(nKeyLen+1, nValueLen);

	return true;
}

TSTR& IniFileIO::GetIniFileName()
{
	return mStrFileName;
}

bool IniFileIO::SaveAs(const TSTR& strFileName)
{
	if(strFileName.isNull())
	{
		return false;
	}

	FILE *pFile = _tfopen(strFileName.data(), _T("wt, ccs=UTF-16LE"));
	if(NULL == pFile)
	{
		return false;
	}

	MapSections::iterator itSection = mMapSections.begin();
	for(; itSection != mMapSections.end(); itSection++)
	{
		MapKeyValues& keyValueMap = itSection->second;
		_ftprintf(pFile, _T("[%s]\n"), itSection->first.data());
		MapKeyValues::iterator itKeyValue = keyValueMap.begin();
		for(; itKeyValue != keyValueMap.end(); itKeyValue++)
		{
			_ftprintf(pFile, _T("%s=%s\n"), itKeyValue->first.data(), itKeyValue->second.data());
		}
	}
	fclose(pFile);

	return true;
}

bool IniFileIO::TrimLeftInvalidChars(const TCHAR* strLine, TCHAR* strNewLine, int len)
{
	if(NULL == strLine || NULL == strNewLine) return false;

	TCHAR* pChar = NULL;
	pChar = const_cast<TCHAR*>(strLine);
	while(*pChar == _T(' ') || *pChar == _T('\t') || *pChar == _T(''))
	{
		pChar++;
	}
	int resultCpy = _tcscpy_s(strNewLine, len, pChar);
	if( 0 == resultCpy )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IniFileIO::TrimRightInvalidChars(TCHAR* strLine)
{
	if(NULL == strLine) return false;
	size_t posLastCharOfString = 0;
	posLastCharOfString = _tcslen(strLine) - 1;
	while((posLastCharOfString >= 0) && 
		(strLine[posLastCharOfString] == _T(' ') || 
		strLine[posLastCharOfString] == _T('\t') ||
		strLine[posLastCharOfString] == _T('')   ||
		strLine[posLastCharOfString] == _T('\n') ||
		strLine[posLastCharOfString] == _T('\r') ))
	{
		posLastCharOfString--;
	}
	strLine[posLastCharOfString+1] = _T('\0');
	return true;
}


bool IniFileIO::TrimInvalidChars(const TCHAR* strOld, TCHAR* strNew, int len)
{
	if(NULL == strOld || NULL == strNew) return false;
	if(TrimLeftInvalidChars(strOld, strNew, len))
	{
		TrimRightInvalidChars(strNew);
		return true;
	}
	return false;
}

bool IniFileIO::TrimInvalidString(const TSTR& strOld, TSTR& strNew)
{
	if(strOld.isNull()) return false;
	int lengthOfString = strOld.length() + 1;
	TCHAR* pStrNew = new TCHAR[lengthOfString];
	if(NULL == pStrNew) return false;
	bool bResult = TrimInvalidChars(strOld.data(), pStrNew, lengthOfString);
	if(!bResult) 
	{
		delete[] pStrNew;
		pStrNew = NULL;
		return false;
	}
	strNew = pStrNew;
	delete[] pStrNew;
	pStrNew = NULL;
	return true;
}

TSTR IniFileIO::GetValueFromSectionKey(const TSTR& strSection, const TSTR& strKey)
{
	TSTR strNewSection;
	TSTR strNewKey;
	TrimInvalidString(strSection, strNewSection);
	TrimInvalidString(strKey, strNewKey);
	MapSections::iterator itSections = mMapSections.find(strNewSection);
	if(itSections == mMapSections.end())
	{
		return _T("");
	}

	MapKeyValues& keyValueMap = itSections->second;
	MapKeyValues::iterator itKeyValue = keyValueMap.find(strNewKey);
	if(itKeyValue != keyValueMap.end())
	{
		return itKeyValue->second;
	}
	return _T("");
}

int IniFileIO::GetIntFromSectionKey(const TSTR& strSection, const TSTR& strKey)
{
	TSTR str = GetValueFromSectionKey(strSection, strKey);
	return _tstoi(str.data());
}

float IniFileIO::GetFloatFromSectionKey(const TSTR& strSection, const TSTR& strKey)
{
	TSTR str = GetValueFromSectionKey(strSection, strKey);
	return _tstof(str.data());
}

bool IniFileIO::AddSectionKeyValue(const TSTR& strSection, const TSTR& strKey, const TSTR& strValue)
{
	if( strSection.length() == 0 || strKey.length() == 0 || strValue.length() == 0 )
	{
		return false;
	}

	TSTR strNewSection;
	TSTR strNewKey;
	TSTR strNewValue;
	TrimInvalidString(strSection, strNewSection);
	TrimInvalidString(strKey, strNewKey);
	TrimInvalidString(strValue, strNewValue);

	mMapSections[strNewSection][strNewKey] = strNewValue;
			
	return true;
}