#pragma once

#include "game_types.h"

class CObjectAbstract;

namespace core_sdk_api
{
    class CORESDK_API IEventInterface
    {
    public:
        IEventInterface(const CObjectAbstract *parent);
        ~IEventInterface();

    protected:
    private:
    };
}