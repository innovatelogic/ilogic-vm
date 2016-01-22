
#pragma once

#include "ieditor.h"
#include "icommand.h"
#include "icommand_buffer.h"
#include <memory>

class CActor;

namespace editors
{

class DLLEXPORT Editor : public IEditor
{
public:
	Editor(ICommandBuffer *buffer);
	virtual ~Editor();

    bool Undo() override;
    bool Redo() override;

    void AddCommand(ICommandPtr command) override;
	
#ifdef USE_MOCK
    /*!
    * Only for ut access
    */
    ICommandBuffer* GetCommandBuffer() const { return m_CommandBuffer.get(); }
#endif//USE_MOCK

protected:
private:
    std::unique_ptr<ICommandBuffer> m_CommandBuffer;

    CActor *m_pEditorRoot;
};

}

