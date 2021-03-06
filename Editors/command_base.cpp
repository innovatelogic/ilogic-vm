#include "command_base.h"

namespace editors
{
    //----------------------------------------------------------------------------------------------
    CommandBase_::CommandBase_()
    {

    }

    //----------------------------------------------------------------------------------------------
    CommandBase_::CommandBase_(const std::function<void()> &op, const std::function<void()> &undo)
        : m_fOp(op)
        , m_fUndo(undo)
    {

    }

    //----------------------------------------------------------------------------------------------
    CommandBase_::~CommandBase_()
    {

    }

    //----------------------------------------------------------------------------------------------
    void CommandBase_::Execute()
    {
        if (m_fOp)
        {
            m_fOp();
        }
    }

    //----------------------------------------------------------------------------------------------
    void CommandBase_::ExecuteUndo()
    {
        if (m_fUndo)
        {
            m_fUndo();
        }
    }
}