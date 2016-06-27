#pragma once

#include "icommand.h"
#include <functional>

namespace editors
{
    class DLLEXPORT CommandBase_ : public ICommand
    {
    public:
        CommandBase_();
        CommandBase_(const std::function<void()> &op, const std::function<void()> &undo);
        virtual ~CommandBase_();

        void Execute() override;
        void ExecuteUndo() override;
        
    protected:
    private:
        std::function<void()>   m_fOp;
        std::function<void()>   m_fUndo;
    };
}