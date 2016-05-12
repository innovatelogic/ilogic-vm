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

        m_undoStack.push_front(cmd);
    }

    //----------------------------------------------------------------------------------------------
    void CommandBuffer::AddCommands(ICommandPtrList &commands)
    {
        SCommandBatch cmd;

        cmd.batch = commands;

        m_undoStack.push_front(cmd);
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
            SCommandBatch top = m_undoStack.front();

            for (auto &command : top.batch)
            {
                command->ExecuteUndo();
            }

            m_redoStack.push_front(top);

            m_undoStack.pop_front();
        }
    }

    //----------------------------------------------------------------------------------------------
    void CommandBuffer::Redo()
    {
        if (!m_redoStack.empty())
        {
            SCommandBatch top = m_redoStack.front();

            for (auto &command : top.batch)
            {
                command->Execute();
            }

            m_undoStack.push_front(top);

            m_redoStack.pop_front();
        }
    }

    //----------------------------------------------------------------------------------------------
    size_t CommandBuffer::GetUndoCommandBatchSize(size_t index) const
    {
        return m_undoStack.at(index).batch.size();
    }

    //----------------------------------------------------------------------------------------------
    size_t CommandBuffer::GetRedoCommandBatchSize(size_t index) const
    {
        return m_redoStack.at(index).batch.size();
    }
}