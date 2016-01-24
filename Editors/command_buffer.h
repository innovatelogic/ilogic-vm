#pragma once

#include "icommand_buffer.h"
#include "icommand.h"
#include <memory>
#include <vector>
#include <stack>

namespace editors
{

class DLLEXPORT CommandBuffer : public ICommandBuffer
{
    struct SCommandBatch
    {
        ICommandPtrList batch;

        /*SCommandBatch operator= (const SCommandButch &other)
        {
            return *this;
        }*/
    };

public:
    CommandBuffer();
    virtual ~CommandBuffer();

    CommandBuffer(const CommandBuffer& other) = delete;
    CommandBuffer operator= (const CommandBuffer& other) = delete;

    void AddCommand(ICommandPtr command);
    void AddCommands(ICommandPtrList &commands) override;

    void Clear() override;

    /*!
    * execute Undo batch and push batch to redo's
    */
    void Undo() override;

    /*!
    * execute Redo batch and push batch to undo's
    */
    void Redo() override;

    /*!
    * returns number of batches in undo stack
    */
    size_t GetSizeUndoStack()  const override { return m_undoStack.size(); }

    /*!
    * returns number of batches in undo stack
    */
    size_t GetSizeRedoStack() const override { return m_redoStack.size(); }

    /*!
    * retreive command batch size of index command in undo stack
    * index - value index
    * return -1 if index is invalid
    */
    int GetUndoCommandBatchSize(size_t index) const override;

    /*!
    * retreive command batch size of index command in undo stack
    * index - value index
    * return -1 if index is invalid
    */
    int GetRedoCommandBatchSize(size_t index) const override;

protected:
private:
    std::stack< SCommandBatch > m_undoStack;
    std::stack< SCommandBatch > m_redoStack;
};
}