#pragma once

#include "game_types.h"

#include "mathlib.h"
#include "IDrawInterface.h"
#include <functional>
#include <vector>
#include <map>

namespace core_sdk_api
{
    struct SState
    {
        oes::common_base::Matrix LTM;
        oes::common_base::Matrix3f STM;
        oes::common_base::Vector YPR;
    };
    using TMapState = std::map<long, SState>;

    //----------------------------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------------------------
    class CORESDK_API transform_history_traits
    {
    public:
        using TVecObjects = std::vector<const IDrawInterface*>;
        
        void SaveState(const TVecObjects &vec);

        void CommitState(const TVecObjects &vec);

        void SetTransformCallback(const std::function<void(TMapState &prevState, TMapState &newState)> &fn) { m_transform_callback = fn; }

    private:
        std::function<void(TMapState &prevState, TMapState &newState)>  m_transform_callback;

        TMapState m_state;
    };
}