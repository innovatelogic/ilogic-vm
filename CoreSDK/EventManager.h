#pragma once

#include "IEventManager.h"
#include "game_types.h"

class CCoreSDK;

namespace oes
{
    namespace core_sdk_api
    {
        class CORESDK_API EventManager : public oes::foundation::IEventManager
        {
        public:
            EventManager(CCoreSDK *api);
            virtual ~EventManager();

            void	BroadcastEvent(ESystemEventID id) override;
            void    BroadcastEvent(ESystemEventID id, const CObjectAbstract *pParam) override;

        protected:
        private:
            CCoreSDK *m_pApi;
        };
    }
}