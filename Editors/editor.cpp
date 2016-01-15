#include "editor.h"
#include <assert.h>
#include "command_buffer.h"

namespace editors
{
//----------------------------------------------------------------------------------------------
Editor::Editor()
{
    
}

//----------------------------------------------------------------------------------------------
Editor::~Editor()
{

}

//----------------------------------------------------------------------------------------------
void Editor::SetCommandBuffer(ICommandBuffer *buffer)
{
    m_CommandBuffer.reset(buffer);
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
void Editor::AddCommand(ICommand *command)
{
    m_CommandBuffer->AddCommand(command);
}
}