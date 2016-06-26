#pragma once

#include "game_types.h"
#include <functional>

namespace core_sdk_api
{
    class CORESDK_API transform_history_traits
    {
    public:
        void SaveState();

        void CommitState();

        void    SetTransformCallback(const std::function<void()> &fn) { m_transform_callback = fn; }

    private:
        std::function<void()>   m_transform_callback;
    };
}