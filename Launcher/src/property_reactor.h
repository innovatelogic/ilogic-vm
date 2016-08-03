#pragma once

#include "iproperty_reactor.h"
#include "ieditor.h"

//namespace oes
//{
//    namespace nmLauncher
//    {
        /*struct SPropertyGroup
        {
            std::string name;
            TVecPropertyClass vecPropertyClasses; // property inheritance structure

            SPropertyGroup(const std::string &_name)
                : name(_name)
            {
            }
        };*/

        template<class T>
        class PropertyReactor : public IPropertyReactor
        {
        public:
            PropertyReactor(const editors::TIEditor &editor);
            virtual ~PropertyReactor();
            
            void Build() override;
            void Clear() override;

        private:
            editors::TIEditor m_editor;
        };
 //   }
//}

#include "property_reactor.ipp"