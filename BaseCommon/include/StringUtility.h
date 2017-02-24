#pragma once

#include "OEMBase.h"

namespace oes
{
    namespace common_base
    {

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
                    OutVal = m_vector[Index];
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
            CStringDataStoreMarkup(const std::string& str, std::string open_tag = "<", std::string close_tag = ">");

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

    }
}