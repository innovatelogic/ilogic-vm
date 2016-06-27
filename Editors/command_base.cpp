#include "command_base.h"

namespace editors
{
    //----------------------------------------------------------------------------------------------
    CommandBase::CommandBase()
    {

    }

    //----------------------------------------------------------------------------------------------
    CommandBase::CommandBase(const std::function<void()> &op, const std::function<void()> &undo)
        : m_fOp(op)
        , m_fUndo(undo)
    {

    }

    //----------------------------------------------------------------------------------------------
    CommandBase::~CommandBase()
    {

    }

    //----------------------------------------------------------------------------------------------
    void CommandBase::Execute()
    {
        if (m_fOp)
        {
            m_fOp();
        }
    }

    //----------------------------------------------------------------------------------------------
    void CommandBase::ExecuteUndo()
    {
        if (m_fUndo)
        {
            m_fUndo();
        }
    }
}