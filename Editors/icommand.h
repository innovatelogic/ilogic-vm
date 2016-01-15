#pragma once

#include "module.h"

namespace editors
{
    class DLLEXPORT ICommand
    {
    public:
        virtual ~ICommand() = 0 {};

        virtual void Execute() = 0;
    protected:

    private:
    };
}