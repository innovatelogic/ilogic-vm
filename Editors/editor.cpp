#include "Actor.h"
#include "editor.h"

namespace editors
{
//----------------------------------------------------------------------------------------------
EditorBase::EditorBase(CActor *actor, ICommandBuffer *buffer)
    : m_CommandBuffer(buffer)
    , m_pEditorRoot(actor)
{

}

//----------------------------------------------------------------------------------------------
EditorBase::~EditorBase()
{

}

//----------------------------------------------------------------------------------------------
bool EditorBase::Undo()
{
    assert(m_CommandBuffer);

    m_CommandBuffer->Undo();

    return true;
}

//----------------------------------------------------------------------------------------------
bool EditorBase::Redo()
{
    assert(m_CommandBuffer);

    m_CommandBuffer->Redo();

    return true;
}

//----------------------------------------------------------------------------------------------
void EditorBase::AddCommand(ICommandPtr command, bool execute /*= true*/)
{
    assert(m_CommandBuffer);

    m_CommandBuffer->AddCommand(std::move(command), execute);
}

//----------------------------------------------------------------------------------------------
void EditorBase::AddCommandBatch(ICommandPtrList &vector)
{
    assert(m_CommandBuffer);

    m_CommandBuffer->AddCommands(vector);
}

//----------------------------------------------------------------------------------------------
size_t EditorBase::GetUndoCommandBatchSize(size_t index) const
{
    assert(m_CommandBuffer);

    return m_CommandBuffer->GetUndoCommandBatchSize(index);
}

//----------------------------------------------------------------------------------------------
size_t EditorBase::GetRedoCommandBatchSize(size_t index) const
{
    assert(m_CommandBuffer);

    return m_CommandBuffer->GetRedoCommandBatchSize(index);
}

//----------------------------------------------------------------------------------------------
const CActor* EditorBase::GetEditorRelatedActor(const CActor *actor)
{
    if (actor && actor->GetExternal())
    {
        while (actor)
        {
            if (!actor->GetExternal()){
                break; // found
            }
            actor = actor->GetParent();
        }
    }
    return actor;
}
}
