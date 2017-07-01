#include "property.h"
#include "ObjectFactory.h"
#include "ClassTree.h"
#include "StringUtility.h"

#include <windows.h>

namespace oes
{
    namespace rflex
    {

        //----------------------------------------------------------------------------------------------
        Property_Base::Property_Base(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= NON_SERIALIZABLE*/,
            bool bCommon/* = NON_COMMON_PROP*/,
            bool bExtProp/* = INT_PROP*/,
            Property_Base *prev/* = 0*/,
            Property_Base *next/* = 0*/)
            : m_Name(name)
            , m_ClassName(classname)
            , m_GroupName(group)
            , m_bSerializable(serializable)
            , m_Policy(policy)
            , m_Ctrl(ctrl)
            , m_bCommonValue(bCommon)
            , m_bExternalProp(bExtProp)
            , m_ElementDim(1)
            , m_ElementSize(0)
            , m_MemOffset(offset)
            , m_pNodePrev(prev)
            , m_pNodeNext(next)
        {
        }

        //----------------------------------------------------------------------------------------------
        PropertyINT::PropertyINT(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev/* = 0*/,
            Property_Base *next /*= 0*/,
            const int *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {

        }

        //----------------------------------------------------------------------------------------------
        char* PropertyINT::GetProperty(const void *Ptr, char *OutBuffer) const
        {
            size_t *Dest = (size_t*)((char*)Ptr + m_MemOffset);
            return itoa(*Dest, OutBuffer, 10);
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyINT::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                itoa(*m_pDefaultValue, pOutBuffer, 10);
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyINT::DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            int ValueINT = atoi(Value);
            size_t *Dest = (size_t*)((char*)Ptr + m_MemOffset + byteOffset);
            *(size_t*)(Dest) = *(size_t*)&ValueINT;

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        PropertyUINT::PropertyUINT(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const unsigned int *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyUINT::GetProperty(const void * Ptr, char *OutBuffer) const
        {
            unsigned int *Dest = (unsigned int*)((char*)Ptr + m_MemOffset);
            return itoa(*Dest, OutBuffer, 10);
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyUINT::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                itoa(*m_pDefaultValue, pOutBuffer, 10);
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyUINT::DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this))
                return;

            unsigned int ValueINT = strtoul(Value, NULL, 0); //atol(Value);
            unsigned int *Dest = (unsigned int*)((char*)Ptr + m_MemOffset + byteOffset);
            *(unsigned int*)(Dest) = *(unsigned int*)&ValueINT;

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        PropertyFLOAT::PropertyFLOAT(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev/*= 0*/,
            Property_Base *next/* = 0*/,
            const float *pDefaultValue /*= 0.f*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyFLOAT::GetProperty(const void *Ptr, char *OutBuffer) const
        {
            float *Dest = (float*)((char*)Ptr + m_MemOffset);
            sprintf(OutBuffer, "%f", *Dest);
            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyFLOAT::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                sprintf(pOutBuffer, "%f", *m_pDefaultValue);
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyFLOAT::DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree & CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this))
                return;

            float ValueFLOAT = (float)atof(Value);
            float *Dest = (float*)((char*)Ptr + m_MemOffset + byteOffset);

            *(float*)(Dest) = *(float*)&ValueFLOAT;

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        PropertyBYTE::PropertyBYTE(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const char *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyBYTE::GetProperty(const void * Ptr, char * OutBuffer) const
        {
            char *Dest = (char*)((char*)Ptr + m_MemOffset);
            *OutBuffer = (char)*Dest;
            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyBYTE::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                *pOutBuffer = (char)*m_pDefaultValue;
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyBYTE::DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this))
                return;

            char *Dest = (char*)((char*)Ptr + m_MemOffset + byteOffset);
            *(char*)(Dest) = *(char*)Value;

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        PropertyBOOL::PropertyBOOL(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const bool *pDefaultValue /*= false*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
            PredefinedValues.push_back("true");
            PredefinedValues.push_back("false");
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyBOOL::GetProperty(const void *Ptr, char *OutBuffer) const
        {
            bool *Dest = (bool*)((char*)Ptr + m_MemOffset);
            sprintf(OutBuffer, "%s", *Dest ? "true" : "false");
            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyBOOL::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                sprintf(pOutBuffer, "%s", *m_pDefaultValue ? "true" : "false");
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyBOOL::DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            bool bFlag = (!stricmp(Value, "true") || !strcmp(Value, "1")) ? true : false;

            bool *Dest = (bool*)((char*)Ptr + m_MemOffset + byteOffset);
            *(bool*)Dest = bFlag;

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        PropertyString::PropertyString(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const char *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyString::GetProperty(const void * Ptr, char * OutBuffer) const
        {
            std::string *Dest = (std::string*)((char*)Ptr + m_MemOffset);
            strcpy(OutBuffer, Dest->c_str());
            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyString::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                pOutBuffer = const_cast<char*>(m_pDefaultValue);
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyString::DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            std::string *Dest = (std::string*)((uint8_t*)Ptr + m_MemOffset + byteOffset);
            *Dest = Value;

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        PropertyStringW::PropertyStringW(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const wchar_t *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
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
                cchDestChar * sizeof(char), nullptr, nullptr);
            strDestination[cchDestChar - 1] = 0;

            if (nResult == 0)
                return false;
            return true;
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyStringW::GetProperty(const void * Ptr, char * OutBuffer) const
        {
            std::wstring *Dest = (std::wstring*)((char*)Ptr + m_MemOffset);

            int Length = (int)(*Dest).length() + 1;
            ConvertWideStringToAnsiCch(OutBuffer, (*Dest).c_str(), Length);
            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyStringW::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                //pOutBuffer = const_cast<wchar_t*>(m_pDefaultValue);
                //bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyStringW::DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            std::wstring *Dest = (std::wstring*)((char*)Ptr + m_MemOffset + byteOffset);
            *Dest = ConvertStringToWideString(std::string(Value));

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        PropertyVector2f::PropertyVector2f(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const Vector2f *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyVector2f::GetProperty(const void * Ptr, char * OutBuffer) const
        {
            Vector2f *Dest = (Vector2f*)((char*)Ptr + m_MemOffset);
            sprintf(OutBuffer, "%0.4f;%0.4f;", Dest->x, Dest->y);
            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyVector2f::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                sprintf(pOutBuffer, "%0.4f;%0.4f;", m_pDefaultValue->x, m_pDefaultValue->y);
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyVector2f::DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            CStringUtility<float> ParseString(Value, ";");
            if (ParseString.m_vector.size() == 2)
            {
                Vector2f * Dest = (Vector2f*)((char*)Ptr + m_MemOffset + byteOffset);
                Dest->x = ParseString.m_vector[0];
                Dest->y = ParseString.m_vector[1];
            }

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        PropertyVector::PropertyVector(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const Vector *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyVector::GetProperty(const void * Ptr, char * OutBuffer) const
        {
            const unsigned int size_float = sizeof(float);
            Vector *Dest = (Vector*)((char*)Ptr + m_MemOffset);
            sprintf(OutBuffer, "%0.4f;%0.4f;%0.4f;", Dest->x, Dest->y, Dest->z);
            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyVector::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                sprintf(pOutBuffer, "%0.4f;%0.4f;%0.4f;", m_pDefaultValue->x, m_pDefaultValue->y, m_pDefaultValue->z);
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyVector::DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            const unsigned int size_float = sizeof(float);
            CStringUtility<float> ParseString(Value, ";");
            if (ParseString.m_vector.size() == 3)
            {
                Vector * Dest = (Vector*)((char*)Ptr + m_MemOffset + byteOffset);
                Dest->x = ParseString.m_vector[0];
                Dest->y = ParseString.m_vector[1];
                Dest->z = ParseString.m_vector[2];
            }

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        PropertyVector4f::PropertyVector4f(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const Vector4f *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyVector4f::GetProperty(const void * Ptr, char * OutBuffer) const
        {
            const unsigned int size_float = sizeof(float);
            Vector4f *Dest = (Vector4f*)((char*)Ptr + m_MemOffset);
            sprintf(OutBuffer,
                "%0.4f;%0.4f;%0.4f;%0.4f;",
                Dest->x,
                Dest->y,
                Dest->z,
                Dest->w);
            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyVector4f::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                sprintf(pOutBuffer, "%0.4f;%0.4f;%0.4f;%0.4f;", m_pDefaultValue->x, m_pDefaultValue->y, m_pDefaultValue->z, m_pDefaultValue->w);
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyVector4f::DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            const unsigned int size_float = sizeof(float);
            CStringUtility<float> ParseString(Value, ";");
            if (ParseString.m_vector.size() == 4)
            {
                Vector4f * Dest = (Vector4f*)((char*)Ptr + m_MemOffset + byteOffset);
                Dest->x = ParseString.m_vector[0];
                Dest->y = ParseString.m_vector[1];
                Dest->z = ParseString.m_vector[2];
                Dest->w = ParseString.m_vector[3];
            }

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }


        //----------------------------------------------------------------------------------------------
        PropertyMatrix3x3::PropertyMatrix3x3(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const Matrix3f *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyMatrix3x3::GetProperty(const void *Ptr, char *OutBuffer) const
        {
            Matrix3f *Dest = (Matrix3f*)((char*)Ptr + m_MemOffset);
            sprintf(OutBuffer,
                "%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;",
                Dest->_11,
                Dest->_12,
                Dest->_13,
                Dest->_21,
                Dest->_22,
                Dest->_23,
                Dest->_31,
                Dest->_32,
                Dest->_33);

            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyMatrix3x3::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                sprintf(pOutBuffer,
                    "%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;",
                    m_pDefaultValue->_11,
                    m_pDefaultValue->_12,
                    m_pDefaultValue->_13,
                    m_pDefaultValue->_21,
                    m_pDefaultValue->_22,
                    m_pDefaultValue->_23,
                    m_pDefaultValue->_31,
                    m_pDefaultValue->_32,
                    m_pDefaultValue->_33);
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyMatrix3x3::DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &tree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = tree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = tree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            CStringUtility<float> ParseString(Value, ";");
            if (ParseString.m_vector.size() == 9)
            {
                Matrix3f * Dest = (Matrix3f*)((char*)Ptr + m_MemOffset + byteOffset);
                Dest->_11 = ParseString.m_vector[0];
                Dest->_12 = ParseString.m_vector[1];
                Dest->_13 = ParseString.m_vector[2];
                Dest->_21 = ParseString.m_vector[3];
                Dest->_22 = ParseString.m_vector[4];
                Dest->_23 = ParseString.m_vector[5];
                Dest->_31 = ParseString.m_vector[6];
                Dest->_32 = ParseString.m_vector[7];
                Dest->_33 = ParseString.m_vector[8];
            }

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        PropertyMatrix::PropertyMatrix(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const Matrix *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyMatrix::GetProperty(const void * Ptr, char * OutBuffer) const
        {
            Matrix *Dest = (Matrix*)((char*)Ptr + m_MemOffset);
            sprintf(OutBuffer,
                "%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;",
                Dest->_11,
                Dest->_12,
                Dest->_13,
                Dest->_14,
                Dest->_21,
                Dest->_22,
                Dest->_23,
                Dest->_24,
                Dest->_31,
                Dest->_32,
                Dest->_33,
                Dest->_34,
                Dest->_41,
                Dest->_42,
                Dest->_43,
                Dest->_44);

            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyMatrix::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                sprintf(pOutBuffer,
                    "%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;",
                    m_pDefaultValue->_11,
                    m_pDefaultValue->_12,
                    m_pDefaultValue->_13,
                    m_pDefaultValue->_14,
                    m_pDefaultValue->_21,
                    m_pDefaultValue->_22,
                    m_pDefaultValue->_23,
                    m_pDefaultValue->_24,
                    m_pDefaultValue->_31,
                    m_pDefaultValue->_32,
                    m_pDefaultValue->_33,
                    m_pDefaultValue->_34,
                    m_pDefaultValue->_41,
                    m_pDefaultValue->_42,
                    m_pDefaultValue->_43,
                    m_pDefaultValue->_44);
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyMatrix::DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            const unsigned int size_float = sizeof(float);
            CStringUtility<float> ParseString(Value, ";");
            if (ParseString.m_vector.size() == 16)
            {
                Matrix * Dest = (Matrix*)((char*)Ptr + m_MemOffset + byteOffset);
                Dest->_11 = ParseString.m_vector[0];
                Dest->_12 = ParseString.m_vector[1];
                Dest->_13 = ParseString.m_vector[2];
                Dest->_14 = ParseString.m_vector[3];
                Dest->_21 = ParseString.m_vector[4];
                Dest->_22 = ParseString.m_vector[5];
                Dest->_23 = ParseString.m_vector[6];
                Dest->_24 = ParseString.m_vector[7];
                Dest->_31 = ParseString.m_vector[8];
                Dest->_32 = ParseString.m_vector[9];
                Dest->_33 = ParseString.m_vector[10];
                Dest->_34 = ParseString.m_vector[11];
                Dest->_41 = ParseString.m_vector[12];
                Dest->_42 = ParseString.m_vector[13];
                Dest->_43 = ParseString.m_vector[14];
                Dest->_44 = ParseString.m_vector[15];
            }

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        PropertyBounds3f::PropertyBounds3f(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const Bounds3f *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyBounds3f::GetProperty(const void * Ptr, char * OutBuffer) const
        {
            const unsigned int size_float = sizeof(float);
            Bounds3f *Dest = (Bounds3f*)((char*)Ptr + m_MemOffset);
            sprintf(OutBuffer,
                "%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;",
                Dest->bound_min.x,
                Dest->bound_min.y,
                Dest->bound_min.z,
                Dest->bound_max.x,
                Dest->bound_max.y,
                Dest->bound_max.z);
            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyBounds3f::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                sprintf(pOutBuffer,
                    "%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;%0.4f;",
                    m_pDefaultValue->bound_min.x,
                    m_pDefaultValue->bound_min.y,
                    m_pDefaultValue->bound_min.z,
                    m_pDefaultValue->bound_max.x,
                    m_pDefaultValue->bound_max.y,
                    m_pDefaultValue->bound_max.z);
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyBounds3f::DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            const unsigned int size_float = sizeof(float);
            CStringUtility<float> ParseString(Value, ";");
            if (ParseString.m_vector.size() == 6)
            {
                Bounds3f * Dest = (Bounds3f*)((char*)Ptr + m_MemOffset + byteOffset);
                Dest->bound_min.x = ParseString.m_vector[0];
                Dest->bound_min.y = ParseString.m_vector[1];
                Dest->bound_min.z = ParseString.m_vector[2];
                Dest->bound_max.x = ParseString.m_vector[3];
                Dest->bound_max.y = ParseString.m_vector[4];
                Dest->bound_max.z = ParseString.m_vector[5];
            }

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        PropertyPixelTransform::PropertyPixelTransform(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const int *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
            PredefinedValues.push_back("PixelOwner_Transform");
            PredefinedValues.push_back("PixelViewport_Transform");
            PredefinedValues.push_back("PersentageOwner_Transform");
            PredefinedValues.push_back("PersentageViewport_Transform");
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyPixelTransform::GetProperty(const void * Ptr, char * OutBuffer) const
        {
            int *Dest = (int*)((char*)Ptr + m_MemOffset);

            if (*Dest == PixelOwner_Transform) {
                strcpy(OutBuffer, "PixelOwner_Transform");
            }
            else if (*Dest == PixelViewport_Transform) {
                strcpy(OutBuffer, "PixelViewport_Transform");
            }
            else if (*Dest == PersentageOwner_Transform) {
                strcpy(OutBuffer, "PersentageOwner_Transform");
            }
            else {
                strcpy(OutBuffer, "PersentageViewport_Transform");
            }
            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyPixelTransform::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                if (*m_pDefaultValue == PixelOwner_Transform) {
                    strcpy(pOutBuffer, "PixelOwner_Transform");
                }
                else if (*m_pDefaultValue == PixelViewport_Transform) {
                    strcpy(pOutBuffer, "PixelViewport_Transform");
                }
                else if (*m_pDefaultValue == PersentageOwner_Transform) {
                    strcpy(pOutBuffer, "PersentageOwner_Transform");
                }
                else {
                    strcpy(pOutBuffer, "PersentageViewport_Transform");
                }
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyPixelTransform::DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            int iValue = 0;
            if (strcmp(Value, "PixelOwner_Transform") == 0) {
                iValue = PixelOwner_Transform;
            }
            else if (strcmp(Value, "PixelViewport_Transform") == 0) {
                iValue = PixelViewport_Transform;
            }
            else if (strcmp(Value, "PersentageOwner_Transform") == 0) {
                iValue = PersentageOwner_Transform;
            }
            else {
                iValue = PersentageViewport_Transform;
            }

            int *Dest = (int*)((char*)Ptr + m_MemOffset + byteOffset);
            *(int*)Dest = iValue;

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }
        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        PropertySceneInputmode::PropertySceneInputmode(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const int *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
            PredefinedValues.push_back("INPUTMODE_None");
            PredefinedValues.push_back("INPUTMODE_ActiveOnly");
        }

        //----------------------------------------------------------------------------------------------
        char* PropertySceneInputmode::GetProperty(const void * Ptr, char * OutBuffer) const
        {
            int *Dest = (int*)((char*)Ptr + m_MemOffset);

            if (*Dest == INPUTMODE_None) {
                strcpy(OutBuffer, "INPUTMODE_None");
            }
            else if (*Dest == INPUTMODE_ActiveOnly) {
                strcpy(OutBuffer, "INPUTMODE_ActiveOnly");
            }
            else {
                strcpy(OutBuffer, "INPUTMODE_None");
            }
            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertySceneInputmode::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                if (*m_pDefaultValue == INPUTMODE_None) {
                    strcpy(pOutBuffer, "INPUTMODE_None");
                }
                else if (*m_pDefaultValue == INPUTMODE_ActiveOnly) {
                    strcpy(pOutBuffer, "INPUTMODE_ActiveOnly");
                }
                else {
                    strcpy(pOutBuffer, "INPUTMODE_None");
                }
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertySceneInputmode::DoSetProperty(const void  *Ptr, const char *Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            int iValue = 0;
            if (!strcmp(Value, "INPUTMODE_None")) {
                iValue = INPUTMODE_None;
            }
            else if (!strcmp(Value, "INPUTMODE_ActiveOnly")) {
                iValue = INPUTMODE_ActiveOnly;
            }
            else {
                iValue = INPUTMODE_None; // @TODO Warning
            }
            int *Dest = (int*)((char*)Ptr + m_MemOffset + byteOffset);
            *(int*)Dest = iValue;

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        PropertyEHandleInputKey::PropertyEHandleInputKey(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const int *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
            PredefinedValues.push_back("HIK_HandleInputKey");
            PredefinedValues.push_back("HIK_IgnoreInputKey");
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyEHandleInputKey::GetProperty(const void *Ptr, char *pOutBuffer) const
        {
            int *Dest = (int*)((char*)Ptr + m_MemOffset);

            if (*Dest == HIK_HandleInputKey) {
                strcpy(pOutBuffer, "HIK_HandleInputKey");
            }
            else if (*Dest == HIK_IgnoreInputKey) {
                strcpy(pOutBuffer, "HIK_IgnoreInputKey");
            }
            else {
                assert(false);
            }
            return pOutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyEHandleInputKey::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                if (*m_pDefaultValue == HIK_HandleInputKey) {
                    strcpy(pOutBuffer, "HIK_HandleInputKey");
                }
                else if (*m_pDefaultValue == HIK_IgnoreInputKey) {
                    strcpy(pOutBuffer, "HIK_IgnoreInputKey");
                }
                else {
                    assert(false);
                }
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyEHandleInputKey::DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            int iValue = 0;
            if (!strcmp(Value, "HIK_HandleInputKey")) {
                iValue = HIK_HandleInputKey;
            }
            else if (!strcmp(Value, "HIK_IgnoreInputKey")) {
                iValue = HIK_IgnoreInputKey;
            }
            else {
                assert(false);
            }
            int *Dest = (int*)((char*)Ptr + m_MemOffset + byteOffset);
            *(int*)Dest = iValue;

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        PropertyEHandleInputMouse::PropertyEHandleInputMouse(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/,
            const int *pDefaultValue /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
            , m_pDefaultValue(pDefaultValue)
        {
            PredefinedValues.push_back("HIK_HandleInputMouse");
            PredefinedValues.push_back("HIK_IgnoreInputMouse");
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyEHandleInputMouse::GetProperty(const void *Ptr, char *pOutBuffer) const
        {
            int *Dest = (int*)((char*)Ptr + m_MemOffset);

            if (*Dest == HIM_HandleInputMouse) {
                strcpy(pOutBuffer, "HIM_HandleInputMouse");
            }
            else if (*Dest == HIM_IgnoreInputMouse) {
                strcpy(pOutBuffer, "HIM_IgnoreInputMouse");
            }
            else {
                assert(false);
            }
            return pOutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        bool PropertyEHandleInputMouse::GetDefaultValue(char *pOutBuffer) const
        {
            bool bResult = false;
            if (m_pDefaultValue && pOutBuffer)
            {
                if (*m_pDefaultValue == HIM_HandleInputMouse) {
                    strcpy(pOutBuffer, "HIM_HandleInputMouse");
                }
                else if (*m_pDefaultValue == HIM_IgnoreInputMouse) {
                    strcpy(pOutBuffer, "HIM_IgnoreInputMouse");
                }
                else {
                    assert(false);
                }
                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyEHandleInputMouse::DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

            ClassNode *pClassNode = CTree.Find(ClassName());
            if (!pClassNode) {
                pClassNode = CTree.FindInterface(ClassName());
            }

            if (!bSilent && pClassNode && !pClassNode->PrePropertyChange(Ptr, this)) {
                return;
            }

            int iValue = 0;
            if (!strcmp(Value, "HIM_IgnoreInputMouse")) {
                iValue = HIM_IgnoreInputMouse;
            }
            else if (!strcmp(Value, "HIM_IgnoreInputMouse")) {
                iValue = HIM_IgnoreInputMouse;
            }
            else {
                assert(false);
            }
            int *Dest = (int*)((char*)Ptr + m_MemOffset + byteOffset);
            *(int*)Dest = iValue;

            if (!bSilent && pClassNode) {
                pClassNode->PostPropertyChange(Ptr, this);
            }
        }

        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        PropertyArrayStatic::PropertyArrayStatic(const char *name,
            int offset,
            const char *classname,
            const char *group,
            Property_Base *source,
            unsigned int elementSize,
            unsigned int elementDim,
            int policy/* = READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
        {
            m_ElementSize = elementSize;
            m_ElementDim = elementDim;

            /*	if (source)
               {
                   source->m_ClassNodePtr = m_ClassNodePtr;

                   // go through child nodes
                   Property_Base * ChildProp = source->GetNext();

                   while (ChildProp)
                   {
                       ChildProp->m_ClassNodePtr = m_ClassNodePtr;
                       ChildProp = ChildProp->GetNext();
                   }
               }*/
        }

        //----------------------------------------------------------------------------------------------
        char* PropertyArrayStatic::GetProperty(const void * Ptr, char * OutBuffer) const
        {
            int TempElementDim = m_ElementDim;
            int PropMemoryOffset = m_MemOffset;

            while (TempElementDim)
            {
                // go through child nodes
                Property_Base * ChildProp = GetNext();

                while (ChildProp)
                {
                    char Buff[256] = { 0 };
                    ChildProp->GetProperty((char*)Ptr + PropMemoryOffset, Buff);

                    Property_Base * NextProp = ChildProp->GetNext();

                    bool bEnd = !NextProp && TempElementDim == 1;

                    strcat(OutBuffer, strcat(Buff, (bEnd ? "" : ","))); // do not put comma separator to end
                    ChildProp = NextProp;
                }

                --TempElementDim;
                PropMemoryOffset = m_MemOffset + GetElementSize() * (GetSize() - TempElementDim);
            }
            return OutBuffer;
        }

        //----------------------------------------------------------------------------------------------
        void PropertyArrayStatic::DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {
            int TempElementDim = GetSize();
            int PropMemoryOffset = m_MemOffset + byteOffset;
            char *pValueBegin = const_cast<char*>(Value);

            while (TempElementDim)
            {
                // go through child nodes
                Property_Base * ChildProp = GetNext();

                while (ChildProp)
                {
                    char * pch = strchr(pValueBegin, ',');

                    if (pch)
                    {
                        int len = pch ? pch - pValueBegin : 0;
                        char * SubValue = new char[len + 1];
                        SubValue[len] = '\0';

                        strncpy(SubValue, pValueBegin, len);

                        ChildProp->SetProperty((char*)Ptr + PropMemoryOffset, SubValue, 0, bSilent);

                        pValueBegin += len + 1;

                        delete[] SubValue;
                    }
                    else
                    {
                        ChildProp->SetProperty((char*)Ptr + PropMemoryOffset, pValueBegin, 0, bSilent);
                    }

                    ChildProp = ChildProp->GetNext();
                }

                --TempElementDim;
                PropMemoryOffset = m_MemOffset + GetElementSize() * (GetSize() - TempElementDim);
            }
        }

        //----------------------------------------------------------------------------------------------
        PropertyArray::PropertyArray(const char *name,
            int offset,
            const char *classname,
            const char *group,
            int policy /*= READ_WRITE*/,
            int ctrl /*= CTRL_EDIT*/,
            bool serializable /*= false*/,
            bool bCommon /*= false*/,
            bool bExtProp /*= INT_PROP*/,
            Property_Base *prev /*= 0*/,
            Property_Base *next /*= 0*/)
            : Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp, prev, next)
        {

        }

        //----------------------------------------------------------------------------------------------
        char* PropertyArray::GetProperty(const void * Ptr, char *OutBuffer) const
        {
            return "";
        }

        //----------------------------------------------------------------------------------------------
        void PropertyArray::DoSetProperty(const void *Ptr, const char* Value, unsigned int byteOffset /*= 0*/, bool bSilent /*= false*/)
        {

        }

    }
}