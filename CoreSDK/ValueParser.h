#ifndef __valueparser_h__
#define __valueparser_h__

#ifdef _WIN32
#pragma once
#endif

#include "platform_specific.h"
#include "StringUtility.h"

class ValueParser
{ 
public:
	typedef std::map<std::string,std::string> MMapStr; 

	ValueParser(const std::string & InStr) { Parse(InStr); }

//------------------------------------------------------------------------
	bool IsValue(const std::string& Key)
	{
		MMapStr::iterator Iter = ValueMap.find(Key);

        if (Iter != ValueMap.end())
		{
            return true;
		}
		return false;
	}

//------------------------------------------------------------------------
	 bool GetValueBOOL(const std::string& Key)
	 { 
		 MMapStr::iterator Iter = ValueMap.find(Key);

		 if (Iter != ValueMap.end())
		 {
			 if (Iter->second == "1" || Iter->second == "TRUE" || Iter->second == "true")
			    return true;
		 }
		 return false;
	 }

//------------------------------------------------------------------------
	 std::string GetValueString(const std::string& Key)
	 { 
		 MMapStr::iterator Iter = ValueMap.find(Key);

		 if (Iter != ValueMap.end())
		 {
			 return Iter->second;
		 }
		 return "";
	 }

//------------------------------------------------------------------------
	 std::wstring GetValueStringUTF8(const std::string& Key)
	 { 
		 MMapStr::iterator Iter = ValueMap.find(Key);

		 if (Iter != ValueMap.end())
		 {
			 int nLen = MultiByteToWideChar(CP_UTF8, 0, Iter->second.c_str(), -1, NULL, NULL);

			 wchar_t *lpszW = new TCHAR[nLen];
			 
			 MultiByteToWideChar(CP_UTF8, 0, Iter->second.c_str(), -1, lpszW, nLen);

			 std::wstring UTF8Text(lpszW);
			 
			 delete [] lpszW;
			 		 
			 return UTF8Text;
		 }
		 std::wstring empty_wstr;
		 return empty_wstr;
	 }

//------------------------------------------------------------------------
	 float GetValueFloat(const std::string& Key)
	 { 
		 MMapStr::iterator Iter = ValueMap.find(Key);

		 if (Iter != ValueMap.end())
		 {
			 return (float)atof(Iter->second.c_str());  
		 }
		 return 0;
	 }

//------------------------------------------------------------------------
	 unsigned int GetValueINT(const std::string& Key)
	 { 
		 MMapStr::iterator Iter = ValueMap.find(Key);

		 if (Iter != ValueMap.end())
		 {
			 return (unsigned int)atoi(Iter->second.c_str()); 
		 }
		 return 0;
	 }

//------------------------------------------------------------------------
	 unsigned int GetValueUINT(const std::string& Key)
	 { 
		 MMapStr::iterator Iter = ValueMap.find(Key);

		 if (Iter != ValueMap.end())
		 {
			 return strtoul(Iter->second.c_str(),NULL,0);
		 }
		 return 0;
	 }

//------------------------------------------------------------------------
     /**  Get Vector2f value */
	 Vector2f GetValueVector2f(const std::string& Key)
	 {
        Vector2f OutValue(0.f,0.f);

		MMapStr::iterator Iter = ValueMap.find(Key);

		if (Iter != ValueMap.end())
		{
			CStringUtility<float> ParseString(Iter->second, ";");

			if (ParseString.m_vector.size() >= 2)
			{
               OutValue.x = ParseString.m_vector[0];
               OutValue.y = ParseString.m_vector[1];
			}
		}
        return OutValue;
	 }

 //------------------------------------------------------------------------
     /**  Get Vector4f value */
	 Vector4f GetValueVector4f(const std::string& Key)
	 {
		 Vector4f OutValue(0.f,0.f,0.f,0.f);

		 MMapStr::iterator Iter = ValueMap.find(Key);

		 if (Iter != ValueMap.end())
		 {
			 CStringUtility<float> ParseString(Iter->second, ";");

			 if (ParseString.m_vector.size() >= 4)
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
	 Matrix GetValueMatrix(const std::string& Key)
	 {
		 Matrix OutValue;

		 MMapStr::iterator Iter = ValueMap.find(Key);

		 if (Iter != ValueMap.end())
		 {
			 CStringUtility<float> ParseString(Iter->second, ";");

			 if (ParseString.m_vector.size() >= 16)
			 {
				 for (int Index = 0; Index < 16; Index++){
				     OutValue.m[Index] = ParseString.m_vector[Index];
				 }
			 }
		 }
		 return OutValue;
	 }

 protected:
	void Parse(const std::string & InStr)
	{
  		std::string SourceString = InStr + " "; // add separate space 

		while (SourceString.find("\n") != std::string::npos)
		{
			size_t EofLIndex = SourceString.find("\n");

			if (EofLIndex != std::string::npos)
			{
			  SourceString.erase(EofLIndex,1);
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

public:
 	MMapStr ValueMap;
};

#endif//__valueparser_h__