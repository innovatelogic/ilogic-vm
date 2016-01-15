#pragma once

#include "ieditor.h"

namespace editors
{
class DLLEXPORT Editor : public IEditor
{
public:
	Editor();
	virtual ~Editor();

    void Undo() override;
    void Redo() override;
	
protected:
private:
};

}

