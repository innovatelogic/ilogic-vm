#pragma once

#include "IEventManager.h"
#include "game_types.h"
#include <vector>
#include <memory>

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

            void    RegisterFunc(const std::function<void(ESystemEventID, const CObjectAbstract*)> &func) override;

        protected:
        private:
            std::vector<std::function<void(ESystemEventID, const CObjectAbstract*)>> m_funcs;

            CCoreSDK *m_pApi;
        };
    }
}