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

        virtual void RegisterEventInterface(const CObjectAbstract *src, const CObjectAbstract *parent);
        virtual void UnregisterEventInterface();

    protected:
    private:
    };
}