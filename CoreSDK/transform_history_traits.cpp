#include "Actor.h"
#include "transform_history_traits.h"

namespace core_sdk_api
{
    //----------------------------------------------------------------------------------------------
    void transform_history_traits::SaveState(const TVecObjects &vec)
    {
        m_state.clear();

        for each (auto &item in vec)
        {
            const CActor *actor = item->GetNode()->key();

            assert(actor);

            SState state;

            state.LTM = item->GetLTM_();
            state.STM = item->GetSTM_();
            state.YPR = item->GetRotator();

            m_state.insert(std::make_pair(actor->GetUID(), state));
        }
    }

    //----------------------------------------------------------------------------------------------
    void transform_history_traits::CommitState(const TVecObjects &vec)
    {
        TMapState m_stateNew;

        for each (auto &item in vec)
        {
            const CActor *actor = item->GetNode()->key();

            assert(actor);

            SState state;

            state.LTM = item->GetLTM_();
            state.STM = item->GetSTM_();
            state.YPR = item->GetRotator();

            m_stateNew.insert(std::make_pair(actor->GetUID(), state));
        }

        m_transform_callback(m_state, m_stateNew);
    }
}