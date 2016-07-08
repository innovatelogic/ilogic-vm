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

        protected:
        private:
            CCoreSDK *m_pApi;
        };
    }
}