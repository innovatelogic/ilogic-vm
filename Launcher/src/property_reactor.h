#pragma once

#include "iproperty_reactor.h"
#include "ieditor.h"

//namespace oes
//{
    namespace nmLauncher
    {
        template<class T>
        class PropertyReactor : public IPropertyReactor
        {
        public:
            PropertyReactor(const editors::TIEditor &editor);
            virtual ~PropertyReactor();
            
            void Build() override;
            void Clear() override;

            void FetchGroups(std::vector<std::string> &groups) override;
            void FetchProperties(const std::string &group, TMapClassData &out) override;

        private:
            void BuildObject(const T *object);

        private:
            editors::TIEditor m_editor;
        
            TMapClassData m_propertyClasses;
        };
    }
//}

#include "property_reactor.ipp"