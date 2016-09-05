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
            PropertyReactor();
            virtual ~PropertyReactor();
            
            void Build(std::vector<const T*> &objects);
            void Clear() override;

            const TMapClassData& GetClasses() const override { return m_propertyClasses; }
            TMapClassData& GetClasses() override { return m_propertyClasses; }

            void FetchGroups(std::vector<std::string> &groups) override;
            void FetchProperties(const std::string &group, TMapClassData &out) override;

        private:
            void BuildObject(const T *object);

            void FillClassProperties(const ClassNode *classNode, int shift, TMapClassData &out);

        private:
            editors::TIEditor m_editor;
        
            TMapClassData m_propertyClasses;
        };
    }
//}

#include "property_reactor.ipp"