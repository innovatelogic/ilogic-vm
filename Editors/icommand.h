#pragma once

#include "module.h"

namespace editors
{
    class DLLEXPORT ICommand
    {
    public:
        virtual ~ICommand() = 0 {};

        virtual void ExecuteUndo() = 0;
        virtual void ExecuteRedo() = 0;

    protected:
    private:
    };
}