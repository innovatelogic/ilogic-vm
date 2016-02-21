
#pragma once

#include "ieditor.h"
#include "icommand.h"
#include "icommand_buffer.h"
#include "command_buffer.h"

class CActor;

namespace editors
{
class DLLEXPORT EditorBase : public IEditor
{
public:
    EditorBase(CActor *actor, ICommandBuffer *buffer);
	virtual ~EditorBase();

    bool Undo() override;
    bool Redo() override;

    void AddCommand(ICommandPtr command) override;
    void AddCommandBatch(ICommandPtrList &vector) override;
	
    ICommandBuffer* GetCommandBuffer() const { return m_CommandBuffer; }

    size_t GetUndoCommandBatchSize(size_t index) const override;
    size_t GetRedoCommandBatchSize(size_t index) const override;

protected:
private:
    ICommandBuffer *m_CommandBuffer;

    CActor *m_pEditorRoot;
};
}
