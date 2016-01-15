#pragma once

#include "module.h"

namespace editors
{
class DLLEXPORT IEditor
{
public:
	virtual ~IEditor() = 0 {};

    virtual void Undo() = 0;
    virtual void Redo() = 0;

protected:
private:
};
}