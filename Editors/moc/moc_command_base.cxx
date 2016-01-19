#pragma once

#include "../command_base.h"
#include "gmock/gmock.h"

namespace editors
{
    class DLLEXPORT MockCommandBase : public CommandBase
    {
    public:
        MOCK_METHOD0(Execute, void());
    };
}