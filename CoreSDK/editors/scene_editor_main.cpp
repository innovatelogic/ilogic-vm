#include "scene_editor_main.h"

namespace editors
{
    SceneEditorMain::SceneEditorMain(CCoreSDK *pInstance, ICommandBuffer *buffer)
        : EditorBase(static_cast<CActor*>(pInstance->GetRootActor()), buffer)
    {

    }

    SceneEditorMain::~SceneEditorMain()
    {

    }

    SRenderContext* SceneEditorMain::GetRenderContext() const
    {
        return nullptr;
    }

    bool SceneEditorMain::Open(const std::wstring &path)
    {
        return true;
    }

    bool SceneEditorMain::Save(const std::wstring &path)
    {
        return true;
    }

    void SceneEditorMain::InitViewport(void *canvas, size_t width, size_t height)
    {

    }

    void SceneEditorMain::ResizeVeiwport(size_t width, size_t height)
    {

    }

    void SceneEditorMain::Render()
    {

    }

    void SceneEditorMain::MouseMove(size_t dx, size_t dy, int ModifKey)
    {

    }
}