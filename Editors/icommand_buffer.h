#pragma once

#include "icommand.h"
#include <vector>

namespace editors
{
class DLLEXPORT ICommandBuffer
{
public:
	virtual	~ICommandBuffer() = 0 {};
	
	virtual void AddCommand(ICommand *command) = 0;
	virtual void AddCommands(std::vector<ICommand*> *commands) = 0;

	virtual void Clear() = 0;

	virtual void Undo() = 0;
	virtual void Redo() = 0;
};
}