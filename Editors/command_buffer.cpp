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
        SCommandBatch cmd;

        cmd.batch.push_back(std::move(command));

        m_undoStack.push(cmd);
    }

    //----------------------------------------------------------------------------------------------
    void CommandBuffer::AddCommands(ICommandPtrList &commands)
    {
        SCommandBatch cmd;

        cmd.batch = commands;

        m_undoStack.push(cmd);
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
            SCommandBatch top = m_undoStack.top();

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
        if (!m_redoStack.empty())
        {
            SCommandBatch top = m_redoStack.top();

            for (auto &command : top.batch)
            {
                command->Execute();
            }

            m_undoStack.push(top);

            m_redoStack.pop();
        }
    }

    //----------------------------------------------------------------------------------------------
    int CommandBuffer::GetUndoCommandBatchSize(size_t index) const
    {
        int outValue = -1;


        return outValue;
    }

    //----------------------------------------------------------------------------------------------
    int CommandBuffer::GetRedoCommandBatchSize(size_t index) const
    {
        int outValue = -1;


        return outValue;
    }
}