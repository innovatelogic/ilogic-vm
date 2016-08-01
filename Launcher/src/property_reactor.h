#pragma once

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

        };

        template<class T>
        class PropertyReactor : public IPropertyReactor
        {

        };
    }
}

#include "property_reactor.ipp"