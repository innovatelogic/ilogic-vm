#pragma once

#include "../icommand.h"
#include <gmock/gmock.h>

namespace editors
{
    class DLLEXPORT MockCommandBase : public ICommand
    {
    public:
        MOCK_METHOD0(ExecuteUndo, void());
        MOCK_METHOD0(ExecuteRedo, void());
    };
}