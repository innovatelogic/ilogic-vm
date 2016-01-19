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
    struct SCommandButch
    {
        ICommandPtrList batch;

        SCommandButch operator= (const SCommandButch &other)
        {
            return *this;
        }
    };

public:
    CommandBuffer();
    virtual ~CommandBuffer();

    void AddCommand(ICommandPtr command);
    void AddCommands(ICommandPtrList commands) override;

    void Clear() override;

    void Undo() override;
    void Redo() override;

    /*!
    * returns number of batches in undo stack
    */
    size_t GetSizeUndoStack()  const override { return m_undoStack.size(); }

    /*!
    * returns number of batches in undo stack
    */
    virtual size_t GetSizeRedoStack() const override { return m_redoStack.size(); }

protected:
private:
    std::stack< SCommandButch > m_undoStack;
    std::stack< SCommandButch > m_redoStack;
};
}