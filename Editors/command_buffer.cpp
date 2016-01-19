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
        if (!m_undoStack.empty())
        {
            SCommandButch top = m_undoStack.top();

            for (auto &command : top.batch)
            {
                command->Execute();
            }

            m_redoStack.push(top);

            m_undoStack.pop();
        }
    }

    //----------------------------------------------------------------------------------------------
    void CommandBuffer::Redo()
    {

    }
}