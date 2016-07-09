#pragma once

#include "module.h"

namespace editors
{
    class DLLEXPORT ICommand
    {
       // ICommand(const ICommand &) = delete;
       // ICommand& operator=(const ICommand&) = delete;

    public:
        virtual ~ICommand() = 0 {};

        virtual void Execute() = 0;
        virtual void ExecuteUndo() = 0;

    protected:
    private:
    };
}