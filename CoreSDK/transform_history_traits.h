#pragma once

#include "game_types.h"

namespace core_sdk_api
{
    class CORESDK_API transform_history_traits
    {
    public:
        void SaveState();

        void CommitState();

    };
}