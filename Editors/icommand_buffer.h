#pragma once

#include "icommand.h"
#include <vector>
#include <memory>

namespace editors
{

using ICommandPtr = std::shared_ptr<ICommand>;
using ICommandPtrList = std::vector<ICommandPtr>;

class DLLEXPORT ICommandBuffer
{
public:
	virtual	~ICommandBuffer() = 0 {};
	
	virtual void AddCommand(ICommandPtr command) = 0;
	virtual void AddCommands(ICommandPtrList &commands) = 0;

	virtual void Clear() = 0;

	virtual void Undo() = 0;
	virtual void Redo() = 0;

    /*!
    * returns number of batches in undo stack
    */
    virtual size_t GetSizeUndoStack() const = 0;

    /*!
    * returns number of batches in undo stack
    */
    virtual size_t GetSizeRedoStack() const = 0;

    /*!
    * retreive command batch size of index command in undo stack
    * index - value index
    * return -1 if index is invalid
    */
    virtual size_t GetUndoCommandBatchSize(size_t index) const = 0;

    /*!
    * retreive command batch size of index command in undo stack
    * index - value index
    * return -1 if index is invalid
    */
    virtual size_t GetRedoCommandBatchSize(size_t index) const = 0;
};
}