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
	virtual void AddCommands(ICommandPtrList commands) = 0;

	virtual void Clear() = 0;

	virtual void Undo() = 0;
	virtual void Redo() = 0;
};
}