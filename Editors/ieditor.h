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

    virtual void AddCommand(ICommand *command) = 0;

protected:
private:
};
}