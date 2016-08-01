#pragma once

namespace oes
{
    namespace nmLauncher
    {
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