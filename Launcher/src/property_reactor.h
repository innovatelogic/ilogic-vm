#pragma once

#include "iproperty_reactor.h"
#include "ieditor.h"

//namespace oes
//{
    namespace nmLauncher
    {
        struct SClassNode
        {
            std::string			name;
            TVecPropertyBase	inheritProperties;
            int					nOverrideByteShift; // overrides byte shift for interface 
         
            SClassNode(const std::string &_name, int shift = -1)
                : name(_name)
                , nOverrideByteShift(shift)
            {
            }
        };

        template<class T>
        class PropertyReactor : public IPropertyReactor
        {
        public:
            PropertyReactor(const editors::TIEditor &editor);
            virtual ~PropertyReactor();
            
            void Build() override;
            void Clear() override;

        private:
            void BuildObject(const T *object);

        private:
            editors::TIEditor m_editor;
        
            std::map<std::string, SClassNode> m_propertyClasses;
        };
    }
//}

#include "property_reactor.ipp"