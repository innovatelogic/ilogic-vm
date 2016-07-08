#pragma once

#include "platform_specific.h"

namespace oes
{
    namespace foundation
    {
        class EXPORT IEventManager
        {
        public:
            virtual ~IEventManager() = 0 {};
        };
    }
}