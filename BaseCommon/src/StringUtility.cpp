#include "mathlib.h"
#include "StringUtility.h"
#include <string>
#include <windows.h>

namespace oes
{
    namespace common_base
    {
        //----------------------------------------------------------------------------------------------
        //  split formatted string to components
        //----------------------------------------------------------------------------------------------
        CStringUtility<int>::CStringUtility(const std::string& str, std::string splitter)
        {
            size_t start = 0;
            size_t end = str.find(splitter, start);

            m_vector.clear();

            while (end != std::string::npos)
            {
                std::string substring = str.substr(start, end - start);

                int value = atoi(substring.c_str());

                m_vector.push_back(value);

                start = end + 1;
                end = str.find(splitter, start);

                if (start != str.size() && end == std::string::npos) // in case if last symbol not splitter character
                {													 // need to parse remains range 
                    std::string last_substring = str.substr(start);
                    m_vector.push_back(atoi(last_substring.c_str()));
                }
            }
        }

        //----------------------------------------------------------------------------------------------
        CStringUtility<size_t>::CStringUtility(const std::string& str, std::string splitter)
        {
            size_t start = 0;
            size_t end = str.find(splitter, start);

            m_vector.clear();

            while (end != std::string::npos)
            {
                std::string substring = str.substr(start, end - start);

                int value = atoi(substring.c_str());

                m_vector.push_back(value);

                start = end + 1;
                end = str.find(splitter, start);

                if (start != str.size() && end == std::string::npos) // in case if last symbol not splitter character
                {													 // need to parse remains range 
                    std::string last_substring = str.substr(start);
                    m_vector.push_back(atoi(last_substring.c_str()));
                }
            }
        }
        //----------------------------------------------------------------------------------------------
        CStringUtility<float>::CStringUtility(const std::string& str, std::string splitter)
        {
            size_t start = 0;
            size_t end = str.find(splitter, start);

            m_vector.clear();

            while (end != std::string::npos)
            {
                std::string substring = str.substr(start, end - start);

                float value = (float)atof(substring.c_str());

                m_vector.push_back(value);

                start = end + 1;
                end = str.find(splitter, start);

                if (start != str.size() && end == std::string::npos) // in case if last symbol not splitter character
                {													 // need to parse remains range 
                    std::string last_substring = str.substr(start);
                    m_vector.push_back((float)atof(last_substring.c_str()));
                }
            }
        }

        //----------------------------------------------------------------------------------------------
        CStringUtility<double>::CStringUtility(const std::string& str, std::string splitter)
        {
            size_t start = 0;
            size_t end = str.find(splitter, start);

            m_vector.clear();

            while (end != std::string::npos)
            {
                std::string substring = str.substr(start, end - start);

                double value = atof(substring.c_str());

                m_vector.push_back(value);

                start = end + 1;
                end = str.find(splitter, start);

                if (start != str.size() && end == std::string::npos) // in case if last symbol not splitter character
                {													 // need to parse remains range 
                    std::string last_substring = str.substr(start);
                    m_vector.push_back(atof(last_substring.c_str()));
                }
            }
        }

        //----------------------------------------------------------------------------------------------
        CStringUtility<std::string>::CStringUtility(const std::string& str, std::string splitter)
        {
            if (!str.empty())
            {
                size_t start = 0;
                size_t end = str.find(splitter, start);

                // push whole by defaults
                m_vector.push_back(str);

                if (end != std::string::npos)
                {
                    m_vector.clear();

                    while (end != std::string::npos)
                    {
                        std::string substring = str.substr(start, end - start);

                        m_vector.push_back(substring);

                        start = end + 1;
                        end = str.find(splitter, start);

                        if (start != str.size() && end == std::string::npos)
                        {
                            std::string last_substring = str.substr(start);
                            m_vector.push_back(last_substring);
                        }
                    }
                }
            }
        }
        //----------------------------------------------------------------------------------------------
        CStringUtility<std::string>::CStringUtility(const std::string& str, std::vector<std::string> &OutArray, std::string splitter /*= " "*/)
        {
            if (str.empty())
            {
                size_t start = 0;
                size_t end = str.find(splitter, start);

                // push whole by defaults
                m_vector.push_back(str);
                OutArray.push_back(str);

                if (end != std::string::npos)
                {
                    m_vector.clear();
                    OutArray.clear();

                    while (end != std::string::npos)
                    {
                        std::string substring = str.substr(start, end - start);

                        m_vector.push_back(substring);
                        OutArray.push_back(substring);

                        start = end + 1;
                        end = str.find(splitter, start);

                        if (end == std::string::npos)
                        {
                            std::string last_substring = str.substr(start, str.length());
                            m_vector.push_back(last_substring);
                            OutArray.push_back(last_substring);
                        }
                    }
                }
            }
        }
        //----------------------------------------------------------------------------------------------
        CStringUtility<Vector>::CStringUtility(const std::string& str, std::string splitter)
        {
            CStringUtility<float> parser(str);

            std::vector<float> &vec = parser.GetVectorRef();

            for (size_t i = 0; i < vec.size(); i += 3)
            {
                Vector point;

                for (int j = 0; j < 3; j++)
                {
                    if (j == 0) point.x = vec[i + j];
                    if (j == 1) point.y = vec[i + j];
                    if (j == 2) point.z = vec[i + j];
                }

                m_vector.push_back(point);
            }
        }

        //----------------------------------------------------------------------------------------------
        //  DataStore markup string checker
        //----------------------------------------------------------------------------------------------  
        CStringDataStoreMarkup::CStringDataStoreMarkup(const std::string& str,
            std::string open_tag /* = "<"*/,
            std::string close_tag /* = ">"*/)
        {
            m_PathString = "";


            //std::string::iterator _iter = str.begin();
            //@todo parse string heare
        }


        //----------------------------------------------------------------------------------------------
        // Name: ConvertWideStringToAnsi()
        // Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
        //       char string. 
        //       cchDestChar is the size in TCHARs of strDestination
        //----------------------------------------------------------------------------------------------
        bool ConvertWideStringToAnsiCch(char *strDestination, const wchar_t *wstrSource, int cchDestChar)
        {
            if (strDestination == nullptr || wstrSource == nullptr || cchDestChar < 1)
                return false;

            int nResult = WideCharToMultiByte(CP_ACP, 0, wstrSource, -1, strDestination,
                cchDestChar*sizeof(char), nullptr, nullptr);
            strDestination[cchDestChar - 1] = 0;

            if (nResult == 0)
                return false;
            return true;
        }

        //----------------------------------------------------------------------------------------------
        std::string ConvertWideStringToString(const std::wstring& strSource)
        {
            int Length = (int)strSource.length() + 1;
            char* buf = new char[Length];
            ConvertWideStringToAnsiCch(buf, strSource.c_str(), Length);
            std::string r(buf);
            delete[] buf;
            return r;
        }

        //----------------------------------------------------------------------------------------------
        // Name: ConvertStringToWideString()
        // Desc: 
        //----------------------------------------------------------------------------------------------
        std::wstring ConvertStringToWideString(const std::string& strSource)
        {
            int Length;
            int slength = (int)strSource.length() + 1;
            Length = MultiByteToWideChar(CP_ACP, 0, strSource.c_str(), slength, 0, 0);
            wchar_t* buf = new wchar_t[Length];
            MultiByteToWideChar(CP_ACP, 0, strSource.c_str(), -1, buf, Length);
            std::wstring r(buf);
            delete[] buf;
            return r;
        }

        //----------------------------------------------------------------------------------------------
        bool hasEnding(std::string const &fullString, std::string const &ending)
        {
            if (fullString.length() >= ending.length()) {
                return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
            }
            else {
                return false;
            }
        }

        //----------------------------------------------------------------------------------------------
        std::wstring getPathFileName(const std::wstring &fullString)
        {
            size_t idx_find = fullString.find_last_of(L"/\\");

            if (idx_find != std::wstring::npos)
            {
                return fullString.substr(idx_find + 1);
            }
            return fullString;
        }
    }
}