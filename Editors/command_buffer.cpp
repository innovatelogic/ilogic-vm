#include "command_buffer.h"

namespace editors
{
    //----------------------------------------------------------------------------------------------
    CommandBuffer::CommandBuffer()
    {

    }
    
    //----------------------------------------------------------------------------------------------
    CommandBuffer::~CommandBuffer()
    {

    }

    //----------------------------------------------------------------------------------------------
    void CommandBuffer::AddCommand(ICommandPtr command)
    {
        SCommandButch batch;

        batch.batch.push_back(std::move(command));

        m_undoStack.push(batch);
    }

    //----------------------------------------------------------------------------------------------
    void CommandBuffer::AddCommands(ICommandPtrList commands)
    {

    }

    //----------------------------------------------------------------------------------------------
    void CommandBuffer::Clear()
    {

    }

    //----------------------------------------------------------------------------------------------
    void CommandBuffer::Undo()
    {

    }

    //----------------------------------------------------------------------------------------------
    void CommandBuffer::Redo()
    {

    }
}