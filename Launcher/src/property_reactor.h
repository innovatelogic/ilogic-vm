#pragma once

#include "ieditor.h"

namespace oes
{
    namespace nmLauncher
    {
        struct SPropertyGroup
        {
            std::string name;
            TVecPropertyClass vecPropertyClasses; // property inheritance structure

            SPropertyGroup(const std::string &_name)
                : name(_name)
            {
            }
        };


        class IPropertyReactor
        {
        public:
            virtual ~IPropertyReactor() = 0 {};

            virtual void Build() = 0;
            virtual void Clear() = 0;
        };

        template<class T>
        class PropertyReactor : public IPropertyReactor
        {
        public:
            PropertyReactor();
            virtual ~PropertyReactor();
            
            void Build() override;
            void Clear() override;

        private:
            //editors::TIEditor m_editor;
        };
    }
}

#include "property_reactor.ipp"