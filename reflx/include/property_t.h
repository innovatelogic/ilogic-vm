#pragma once

#include "property.h"

namespace oes
{
    namespace rflex
    {
        template<typename T, typename B>
        class REFLX_EXPORT TProperty : public Property_Base
        {
        public:
            TProperty(const char *name, const char *classname, const char *group, 
                        std::function<void(const void *ptr, const char*)> fset,
                        std::function<void(const void *ptr, char*)> fget)
                : Property_Base(name, 0, classname, group, READ_WRITE, CTRL_EDIT_RESOURCE, SERIALIZABLE, NON_COMMON_PROP, INT_PROP)
                , m_fset(fset)
                , m_fget(fget)
            {}

            char* GetProperty(const void *ptr, char *pout) const override 
            { 
                //const char **out;
                m_fget(ptr, pout);
                return const_cast<char*>(pout);
            }

            bool GetDefaultValue(char *pOutBuffer) const override { return false; }

        private:
            void DoSetProperty(const void *ptr, const char *value, unsigned int byteOffset = 0, bool bSilent = false) override 
            {
                m_fset(ptr, value);
            }

        protected:
        private:
            const std::function<void(const void *ptr, const char*)> m_fset;
            const std::function<void(const void *ptr, char*)> m_fget;
        };
    }
}