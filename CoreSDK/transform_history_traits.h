#pragma once

#include "game_types.h"

#include "mathlib.h"
#include "IDrawInterface.h"
#include <functional>
#include <vector>
#include <map>

namespace core_sdk_api
{
    class CORESDK_API transform_history_traits
    {
        struct SState
        {
            oes::common_base::Matrix LTM;
            oes::common_base::Matrix3f STM;
            oes::common_base::Vector YPR;
        };
    
    public:
        using TVecObjects = std::vector<const IDrawInterface*>;
        using TMapState = std::map<long, SState>;

    public:
        void SaveState(const TVecObjects &vec);

        void CommitState();

        void SetTransformCallback(const std::function<void()> &fn) { m_transform_callback = fn; }

    private:
        std::function<void()>   m_transform_callback;

        TMapState m_state;
    };
}