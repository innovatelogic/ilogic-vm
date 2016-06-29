
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

    void AddCommand(ICommandPtr command, bool execute = true) override;
    void AddCommandBatch(ICommandPtrList &vector) override;
	
    ICommandBuffer* GetCommandBuffer() const { return m_CommandBuffer; }

    size_t GetUndoCommandBatchSize(size_t index) const override;
    size_t GetRedoCommandBatchSize(size_t index) const override;

    void SetNotifyFunc(const std::function<void()> &func) override { m_notifyFunc = func; }

    const std::vector<CActor*>& GetSelected() const override { return m_selectionList; }

protected:
    std::function<void()>   m_notifyFunc;

    std::vector<CActor*> m_selectionList;

private:
    ICommandBuffer *m_CommandBuffer;

    CActor *m_pEditorRoot;
};
}
