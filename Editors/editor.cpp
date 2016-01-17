#include "editor.h"
#include <assert.h>
#include "command_buffer.h"

#ifdef USE_MOCK
#include "moc/moc_command_buffer.cxx"
#endif//USE_MOCK

namespace editors
{
//----------------------------------------------------------------------------------------------
Editor::Editor()
{
#ifndef USE_MOCK
    m_CommandBuffer.reset(std::move(new CommandBuffer()));
#else
    m_CommandBuffer.reset(std::move(new MockCommandBuffer()));
#endif
}

//----------------------------------------------------------------------------------------------
Editor::~Editor()
{

}

//----------------------------------------------------------------------------------------------
bool Editor::Undo()
{
    bool bResult = false;

    if (m_CommandBuffer)
    {
        m_CommandBuffer->Undo();
        bResult = true;
    }

    return true;
}

//----------------------------------------------------------------------------------------------
bool Editor::Redo()
{
    bool bResult = false;

    if (m_CommandBuffer)
    {
        m_CommandBuffer->Redo();
        bResult = true;
    }

    return bResult;
}

//----------------------------------------------------------------------------------------------
void Editor::AddCommand(ICommandPtr command)
{
    m_CommandBuffer->AddCommand(command);
}
}