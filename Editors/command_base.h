#pragma once

#include "icommand.h"

namespace editors
{
    class DLLEXPORT CommandBase : public ICommand
    {
    public:
        CommandBase();
        virtual ~CommandBase();

        void Execute() override;
        void ExecuteUndo() override;
        
    protected:
    private:
    };
}