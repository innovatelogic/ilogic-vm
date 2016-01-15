
#pragma once

#include "ieditor.h"
#include "icommand.h"
#include "icommand_buffer.h"
#include <memory>

namespace editors
{

class DLLEXPORT Editor : public IEditor
{
public:
	Editor();
	virtual ~Editor();

    void SetCommandBuffer(ICommandBuffer *buffer);

    bool Undo() override;
    bool Redo() override;

    void AddCommand(ICommand *command) override;
	
protected:
private:
    std::unique_ptr<ICommandBuffer> m_CommandBuffer;
};

}

