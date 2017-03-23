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
                        std::function<void(const void *ptr, const T&)> fset)
                : Property_Base(name, 0, classname, group, READ_WRITE, CTRL_EDIT_RESOURCE, SERIALIZABLE, NON_COMMON_PROP, INT_PROP)
                , m_fset(fset)
            {}

            char*	GetProperty(const void *Ptr, char *pOutBuffer) const override { return nullptr; }
            bool	GetDefaultValue(char *pOutBuffer) const override { return false; }

        private:
            void DoSetProperty(const void *ptr, const char *value, unsigned int byteOffset = 0, bool bSilent = false) override 
            {
               // B *ptr_b = const_cast<B*>((const B*)(ptr));
                //assert(ptr_b);
                m_fset(ptr, T(value));
            }

        protected:
        private:
            const std::function<void(const void *ptr, const T&)> m_fset;
        };
    }
}