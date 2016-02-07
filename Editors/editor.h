
#pragma once

#include "ieditor.h"
#include "icommand.h"
#include "icommand_buffer.h"
#include "command_buffer.h"
#include <memory>
#include <assert.h>

class CActor;

#ifndef __EDITORS_EDITOR_H__
#define __EDITORS_EDITOR_H__

namespace editors
{
template<typename TCLASS_BUFFER>
class DLLEXPORT EditorBase : public IEditor
{
public:
    EditorBase();
	virtual ~EditorBase();

    bool Undo() override;
    bool Redo() override;

    void AddCommand(ICommandPtr command) override;
    void AddCommandBatch(ICommandPtrList &vector) override;
	
    ICommandBuffer* GetCommandBuffer() const { return m_CommandBuffer.get(); }

    size_t GetUndoCommandBatchSize(size_t index) const override;
    size_t GetRedoCommandBatchSize(size_t index) const override;

protected:
private:
    std::unique_ptr<ICommandBuffer> m_CommandBuffer;

    CActor *m_pEditorRoot;
};
}

#include "editor.cpp"
#endif//__EDITORS_EDITOR_H__
