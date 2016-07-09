#pragma once

#include "platform_specific.h"
#include "StdafxFoundation.h"

class CObjectAbstract;

namespace oes
{
    namespace foundation
    {
        class EXPORT IEventManager
        {
        public:
            virtual ~IEventManager() = 0 {};

            virtual void	BroadcastEvent(ESystemEventID id) = 0;
            virtual void    BroadcastEvent(ESystemEventID id, const CObjectAbstract *pParam) = 0;
            virtual void    RegisterFunc(const std::function<void(ESystemEventID, const CObjectAbstract*)> &func) = 0;
        };
    }
}