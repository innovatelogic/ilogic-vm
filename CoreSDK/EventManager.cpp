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
        void EventManager::BroadcastEvent(ESystemEventID id)
        {
            assert(false);
        }

        //----------------------------------------------------------------------------------------------
        void EventManager::BroadcastEvent(ESystemEventID id, const CObjectAbstract *pParam)
        {
            for each (auto &item in m_funcs)
            {
                item(id, pParam);
            }
        }

        //----------------------------------------------------------------------------------------------
        void EventManager::RegisterFunc(const std::function<void(ESystemEventID, const CObjectAbstract*)> &func)
        {
            m_funcs.push_back(func);
        }
    }
}