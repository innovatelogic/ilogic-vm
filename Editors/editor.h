
#pragma once

#include "ieditor.h"
#include "icommand.h"
#include "icommand_buffer.h"
#include "command_buffer.h"
#include "selection_container.h"

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
    /*!
     *  Returns a lowest editor-root related to input actor
     */
    const CActor* GetEditorRelatedActor(const CActor *actor);

protected:
    std::function<void()>   m_notifyFunc;

    oes::editors::SelectionContainer<CActor> m_selection;
    std::vector<CActor*> m_selectionList; // TODO replace with new

private:
    ICommandBuffer *m_CommandBuffer;

    CActor *m_pEditorRoot;
};
}
