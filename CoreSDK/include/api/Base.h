#pragma once

#include "../../game_types.h"

namespace core_sdk_api
{
    class CORESDK_API Base
    {
    public:
        virtual void    release() = 0;
    };
}