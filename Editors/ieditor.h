#pragma once

#include "icommand.h"
#include "icommand_buffer.h"

namespace editors
{
class DLLEXPORT IEditor
{
public:
	virtual ~IEditor() = 0 {};

    virtual bool Undo() = 0;
    virtual bool Redo() = 0;

    virtual void AddCommand(ICommandPtr command) = 0;
    virtual void AddCommandBatch(ICommandPtrList &vector) = 0;

    virtual size_t GetUndoCommandBatchSize(size_t index) const = 0;
    virtual size_t GetRedoCommandBatchSize(size_t index) const = 0;

protected:
private:
};
}