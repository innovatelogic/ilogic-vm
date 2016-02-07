
#ifdef __EDITORS_EDITOR_H__

namespace editors
{
//----------------------------------------------------------------------------------------------
template<typename TCLASS_BUFFER>
EditorBase<TCLASS_BUFFER>::EditorBase()
    : m_CommandBuffer(std::move(new TCLASS_BUFFER))
{

}

//----------------------------------------------------------------------------------------------
template<typename TCLASS_BUFFER>
EditorBase<TCLASS_BUFFER>::~EditorBase()
{

}

//----------------------------------------------------------------------------------------------
template<typename TCLASS_BUFFER>
bool EditorBase<TCLASS_BUFFER>::Undo()
{
    assert(m_CommandBuffer);

    m_CommandBuffer->Undo();

    return true;
}

//----------------------------------------------------------------------------------------------
template<typename TCLASS_BUFFER>
bool EditorBase<TCLASS_BUFFER>::Redo()
{
    assert(m_CommandBuffer);

    m_CommandBuffer->Redo();

    return true;
}

//----------------------------------------------------------------------------------------------
template<typename TCLASS_BUFFER>
void EditorBase<TCLASS_BUFFER>::AddCommand(ICommandPtr command)
{
    assert(m_CommandBuffer);

    m_CommandBuffer->AddCommand(std::move(command));
}

//----------------------------------------------------------------------------------------------
template<typename TCLASS_BUFFER>
void EditorBase<TCLASS_BUFFER>::AddCommandBatch(ICommandPtrList &vector)
{
    assert(m_CommandBuffer);

    m_CommandBuffer->AddCommands(vector);
}

//----------------------------------------------------------------------------------------------
template<typename TCLASS_BUFFER>
size_t EditorBase<TCLASS_BUFFER>::GetUndoCommandBatchSize(size_t index) const
{
    assert(m_CommandBuffer);

    return m_CommandBuffer->GetUndoCommandBatchSize(index);
}

//----------------------------------------------------------------------------------------------
template<typename TCLASS_BUFFER>
size_t EditorBase<TCLASS_BUFFER>::GetRedoCommandBatchSize(size_t index) const
{
    assert(m_CommandBuffer);

    return m_CommandBuffer->GetRedoCommandBatchSize(index);
}
}

#endif//__EDITORS_EDITOR_H__