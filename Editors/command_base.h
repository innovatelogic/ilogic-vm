#pragma once

#include "icommand.h"

namespace editors
{
    class DLLEXPORT CommandBase : public ICommand
    {
    public:
        CommandBase();
        virtual ~CommandBase();

        void ExecuteUndo() override;
        void ExecuteRedo() override;
    protected:
    private:
    };
}