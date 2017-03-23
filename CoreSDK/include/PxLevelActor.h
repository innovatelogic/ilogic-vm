#pragma once

#include "BaseActor.h"

namespace core_sdk_api
{
    class CORESDK_API PxLevelActor : public BaseActor
    {
    public:
        virtual bool	Load(const char *url) = 0;
        virtual void	Clear() = 0;
    };

}