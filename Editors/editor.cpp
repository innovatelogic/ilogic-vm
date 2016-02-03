#include "editor.h"
#include <assert.h>
#include "command_buffer.h"

#ifdef USE_MOCK
#include "moc/moc_command_buffer.cxx"
#endif//USE_MOCK

namespace editors
{
//----------------------------------------------------------------------------------------------
EditorBase::EditorBase(ICommandBuffer *buffer)
    : m_CommandBuffer(std::move(buffer))
{
/*#ifndef USE_MOCK
    m_CommandBuffer.reset(std::move(new CommandBuffer()));
#else
    m_CommandBuffer.reset(std::move(new MockCommandBuffer()));
#endif*/
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
void EditorBase::AddCommand(ICommandPtr command)
{
    assert(m_CommandBuffer);

    m_CommandBuffer->AddCommand(std::move(command));
}

//----------------------------------------------------------------------------------------------
void EditorBase::AddCommandBatch(ICommandPtrList &vector)
{
    assert(m_CommandBuffer);

    m_CommandBuffer->AddCommands(vector);
}

//----------------------------------------------------------------------------------------------
int EditorBase::GetUndoCommandBatchSize(size_t index) const
{
    assert(m_CommandBuffer);

    return m_CommandBuffer->GetUndoCommandBatchSize(index);
}

//----------------------------------------------------------------------------------------------
int EditorBase::GetRedoCommandBatchSize(size_t index) const
{
    assert(m_CommandBuffer);

    return m_CommandBuffer->GetRedoCommandBatchSize(index);
}
}