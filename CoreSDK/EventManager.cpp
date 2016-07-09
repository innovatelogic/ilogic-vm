#include "EventManager.h"
#include "CoreSDK.h"

namespace oes
{
    namespace core_sdk_api
    {
        //----------------------------------------------------------------------------------------------
        EventManager::EventManager(CCoreSDK *api)
            : m_pApi(api)
        {

        }
        
        //----------------------------------------------------------------------------------------------
        EventManager::~EventManager()
        {

        }

        //----------------------------------------------------------------------------------------------
        void EventManager::BroadcastEvent(ESystemEventID Id)
        {

        }

        //----------------------------------------------------------------------------------------------
        void EventManager::BroadcastEvent(ESystemEventID Id, const CObjectAbstract *pParam)
        {

        }
    }
}