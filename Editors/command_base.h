#pragma once

#include "icommand.h"
#include <functional>

namespace editors
{
    class DLLEXPORT CommandBase : public ICommand
    {
    public:
        CommandBase();
        CommandBase(const std::function<void()> &op, const std::function<void()> &undo);
        virtual ~CommandBase();

        void Execute() override;
        void ExecuteUndo() override;
        
    protected:
    private:
        std::function<void()>   m_fOp;
        std::function<void()>   m_fUndo;
    };
}