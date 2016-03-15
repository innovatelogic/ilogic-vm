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


#ifndef _STR_UTILITY_
#define _STR_UTILITY_

#ifdef _WIN32
#pragma once
#endif

#include "OEMBase.h"

//------------------------------------------------------------------------
//  split formatted string to components
//------------------------------------------------------------------------
template <class T> 
class COMMON_BASE_EXPORT CStringUtility
{
	typedef std::vector<T> TVector; 

public:
	CStringUtility(const std::string& str, std::string splitter = " ");
	CStringUtility(const std::string& str, std::vector<std::string> &OutArray, std::string splitter = " ");
	
	bool GetValue(size_t Index, T& OutVal)
	{ 
		bool bResult = false;

		if (Index < m_vector.size())
		{
            OutVal  = m_vector[Index];
			bResult = true;
		}
		return bResult;
	}

	bool IsEmpty() { return (m_vector.size() == 0); }
	TVector& GetVectorRef() { return m_vector; }

    TVector m_vector;
};

//------------------------------------------------------------------------
//  DataStore markup string checker
//------------------------------------------------------------------------  
class COMMON_BASE_EXPORT CStringDataStoreMarkup
{
public:
	CStringDataStoreMarkup(const std::string& str,std::string open_tag = "<",std::string close_tag = ">");

	std::string& GetPathString() { return m_PathString; }

private:
	std::string m_PathString;
};

// Utility functions
COMMON_BASE_EXPORT bool		   ConvertWideStringToAnsiCch(char *strDestination, const wchar_t *wstrSource, int cchDestChar);
COMMON_BASE_EXPORT std::string  ConvertWideStringToString(const std::wstring& strSource);
COMMON_BASE_EXPORT std::wstring ConvertStringToWideString(const std::string& strSource);

COMMON_BASE_EXPORT bool hasEnding(std::string const &fullString, std::string const &ending);
COMMON_BASE_EXPORT std::wstring getPathFileName(const std::wstring &fullString);

#endif
