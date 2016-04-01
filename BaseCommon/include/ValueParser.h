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

#ifndef __valueparser_h__
#define __valueparser_h__

#ifdef _WIN32
#pragma once
#endif

#include "StringUtility.h"

class COMMON_BASE_EXPORT ValueParser
{
public:
	typedef std::map<std::string, std::string> MMapStr; 

	ValueParser(const std::string & InStr) { Parse(InStr); }

	//------------------------------------------------------------------------
	bool IsValue(const std::string &Key) const
	{
		MMapStr::const_iterator Iter = ValueMap.find(Key);

		if (Iter != ValueMap.end())
		{
			return true;
		}
		return false;
	}

	//------------------------------------------------------------------------
	bool GetValueBOOL(const std::string &Key) const
	{ 
		MMapStr::const_iterator Iter = ValueMap.find(Key);

		if (Iter != ValueMap.end())
		{
			if (Iter->second == "1" || Iter->second == "TRUE" || Iter->second == "true")
				return true;
		}
		return false;
	}

	//------------------------------------------------------------------------
	std::string GetValueString(const std::string &Key) const
	{ 
		MMapStr::const_iterator Iter = ValueMap.find(Key);

		if (Iter != ValueMap.end())
		{
			return Iter->second;
		}
		return "";
	}

	//------------------------------------------------------------------------
	std::wstring GetValueStringUTF8(const std::string& Key) const
	{ 
		MMapStr::const_iterator Iter = ValueMap.find(Key);

		if (Iter != ValueMap.end())
		{
			int nLen = MultiByteToWideChar(CP_UTF8, 0, Iter->second.c_str(), -1, nullptr, 0);

			wchar_t *lpszW = new wchar_t[nLen];

			MultiByteToWideChar(CP_UTF8, 0, Iter->second.c_str(), -1, lpszW, nLen);

			std::wstring UTF8Text(lpszW);

			delete [] lpszW;

			return UTF8Text;
		}

		std::wstring empty_wstr;
		return empty_wstr;
	}

	//------------------------------------------------------------------------
	float GetValueFloat(const std::string& Key) const
	{ 
		MMapStr::const_iterator Iter = ValueMap.find(Key);

		if (Iter != ValueMap.end())
		{
			return (float)atof(Iter->second.c_str());  
		}
		return 0;
	}

	//------------------------------------------------------------------------
	unsigned int GetValueINT(const std::string& Key) const
	{ 
		MMapStr::const_iterator Iter = ValueMap.find(Key);

		if (Iter != ValueMap.end())
		{
			return (unsigned int)atoi(Iter->second.c_str()); 
		}
		return 0;
	}

	//------------------------------------------------------------------------
	unsigned int GetValueUINT(const std::string& Key) const
	{ 
		MMapStr::const_iterator Iter = ValueMap.find(Key);

		if (Iter != ValueMap.end())
		{
			return strtoul(Iter->second.c_str(),nullptr,0);
		}
		return 0;
	}

	//------------------------------------------------------------------------
	/**  Get Vector2f value */
	Vector2f GetValueVector2f(const std::string& Key) const
	{
		Vector2f OutValue(0.f,0.f);

		MMapStr::const_iterator Iter = ValueMap.find(Key);

		if (Iter != ValueMap.end())
		{
			CStringUtility<float> ParseString(Iter->second, ";");

			if (ParseString.m_vector.size() == 2)
			{
				OutValue.x = ParseString.m_vector[0];
				OutValue.y = ParseString.m_vector[1];
			}
		}
		return OutValue;
	}

	//------------------------------------------------------------------------
	/**  Get Vector2f value */
	Vector GetValueVector(const std::string& Key) const
	{
		Vector OutValue(0.f,0.f,0.f);

		MMapStr::const_iterator Iter = ValueMap.find(Key);

		if (Iter != ValueMap.end())
		{
			CStringUtility<float> ParseString(Iter->second, ";");

			if (ParseString.m_vector.size() == 3)
			{
				OutValue.x = ParseString.m_vector[0];
				OutValue.y = ParseString.m_vector[1];
				OutValue.z = ParseString.m_vector[2];
			}
		}
		return OutValue;
	}
	//------------------------------------------------------------------------
	/**  Get Vector4f value */
	Vector4f GetValueVector4f(const std::string& Key) const
	{
		Vector4f OutValue(0.f,0.f,0.f,0.f);

		MMapStr::const_iterator Iter = ValueMap.find(Key);

		if (Iter != ValueMap.end())
		{
			CStringUtility<float> ParseString(Iter->second, ";");

			if (ParseString.m_vector.size() == 4)
			{
				OutValue.x = ParseString.m_vector[0];
				OutValue.y = ParseString.m_vector[1];
				OutValue.z = ParseString.m_vector[2];
				OutValue.w = ParseString.m_vector[3];
			}
		}
		return OutValue;
	}

	//------------------------------------------------------------------------
	/**  Get Matrix value */
	Matrix GetValueMatrix(const std::string& Key) const
	{
		Matrix OutValue;

		MMapStr::const_iterator Iter = ValueMap.find(Key);

		if (Iter != ValueMap.end())
		{
			CStringUtility<float> ParseString(Iter->second, ";");

			if (ParseString.m_vector.size() == 16)
			{
				for (int Index = 0; Index < 16; ++Index){
					OutValue.m[Index] = ParseString.m_vector[Index];
				}
			}
		}
		return OutValue;
	}

	//------------------------------------------------------------------------
	/**  Get Bounds3f value */
	Bounds3f GetValueBounds3f(const std::string& Key) const
	{
		Bounds3f OutValue;

		MMapStr::const_iterator Iter = ValueMap.find(Key);

		if (Iter != ValueMap.end())
		{
			CStringUtility<float> ParseString(Iter->second, ";");

			if (ParseString.m_vector.size() == 6)
			{
				OutValue.bound_min.x = ParseString.m_vector[0];
				OutValue.bound_min.y = ParseString.m_vector[1];
				OutValue.bound_min.z = ParseString.m_vector[2];

				OutValue.bound_max.x = ParseString.m_vector[3];
				OutValue.bound_max.y = ParseString.m_vector[4];
				OutValue.bound_max.z = ParseString.m_vector[5];
			}
		}
		return OutValue;
	}

	//------------------------------------------------------------------------
	void Parse(const std::string & InStr)
	{
		std::string SourceString = InStr + " "; // add separate space 

		while (SourceString.find("\n") != std::string::npos)
		{
			size_t EofLIndex = SourceString.find("\n");

			if (EofLIndex != std::string::npos)
			{
				SourceString.erase(EofLIndex, 1); // remove line break 
			}
		}

		while (SourceString.find(" ") != std::string::npos)
		{
			size_t BrakeIndex = SourceString.find(" ");

			if (BrakeIndex > 0)
			{
				std::string CompileNode = SourceString.substr(0,  BrakeIndex);

				size_t KeyBrakeIndex1 = SourceString.find(":");
				size_t KeyBrakeIndex2 = SourceString.find("=");
				size_t KeyBrakeIndex = (KeyBrakeIndex1 < KeyBrakeIndex2) ? KeyBrakeIndex1 : KeyBrakeIndex2;

				std::string key = CompileNode.substr(0, KeyBrakeIndex);
				std::string value = CompileNode.substr(KeyBrakeIndex+1);

				ValueMap.insert(std::make_pair(key, value));
			}
			SourceString = SourceString.substr(BrakeIndex + 1);
		}
	}

	//------------------------------------------------------------------------
	void ParseCommandLine(const std::string & InStr)
	{
		std::string SourceString = InStr + " "; // add separate space 

		while (SourceString.find("\n") != std::string::npos)
		{
			size_t EofLIndex = SourceString.find("\n");

			if (EofLIndex != std::string::npos){
				SourceString.erase(EofLIndex, 1); // remove line break 
			}
		}

		// push separators & remove '-' char
		while (SourceString.find("-") != std::string::npos)
		{
			size_t EofLIndex = SourceString.find("-");

			if (EofLIndex != std::string::npos)
			{
				size_t TrailingSpaceIndex = SourceString.find(" ", EofLIndex);

				if (TrailingSpaceIndex != std::string::npos){
					SourceString[TrailingSpaceIndex] = ':'; //replace separator
				}
				SourceString.erase(EofLIndex, 1); // remove line break 
			}
		}

		Parse(SourceString); // parse as standard string
	}

	MMapStr ValueMap;
};

#endif//__valueparser_h__