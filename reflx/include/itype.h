#pragma once

namespace oes
{
    namespace rflex
    {
        struct itype
        {
            virtual void *instance() = 0;
            virtual const char* GetName() const = 0;
        };
    }
}