#include "scene_editor.h"
#include "ModelViewer.h"
#include "Camera.h"
#include "CoreSDK.h"
#include "RenderSDK.h"
#include "CameraManager.h"

namespace editors
{
    //----------------------------------------------------------------------------------------------
    EditorScene3D::EditorScene3D(CModelViewer *viewer, ICommandBuffer *buffer)
        : EditorBase(static_cast<CActor*>(viewer), buffer)
        , m_pViewer(viewer)
    {

    }

    //----------------------------------------------------------------------------------------------
    EditorScene3D::~EditorScene3D()
    {

    }

    //----------------------------------------------------------------------------------------------
    SRenderContext* EditorScene3D::GetRenderContext() const
    {
        assert(m_pViewer);
        return m_pViewer->GetRenderContext();
    }

    //----------------------------------------------------------------------------------------------
    bool EditorScene3D::Open(const std::wstring &path)
    {
        assert(m_pViewer);
        return m_pViewer->GenerateObjectView(path.c_str());
    }

    //----------------------------------------------------------------------------------------------
    bool EditorScene3D::Save(const std::wstring &path)
    {
        assert(m_pViewer);
        return m_pViewer->Save(path.c_str());
    }

    //----------------------------------------------------------------------------------------------
    void EditorScene3D::InitViewport(void *canvas, size_t width, size_t height)
    {
        D3DDriver *pDriver = m_pViewer->GetAppMain()->GetRenderSDK()->GetRenderDriver();
        assert(pDriver);

        CCamera *pCamera = m_pViewer->GetCamera();
        assert(pCamera);

        SRenderContext *context = pCamera->GetRenderContext();
        assert(context);

        m_pViewer->GetAppMain()->GetRenderSDK()->InitCanvas(canvas, width, height, context);
    }

    //----------------------------------------------------------------------------------------------
    void EditorScene3D::ResizeVeiwport(size_t width, size_t height)
    {
        CCamera *pCamera = m_pViewer->GetCamera();
        assert(pCamera);

        SRenderContext *context = pCamera->GetRenderContext();
        assert(context);

        m_pViewer->GetAppMain()->GetRenderSDK()->ResizeWindow(width, height, context);
        m_pViewer->GetAppMain()->GetCameraManager()->ViewportResized(context);
    }

    //----------------------------------------------------------------------------------------------
    void EditorScene3D::Render()
    {
        CCamera *pCamera = m_pViewer->GetCamera();
        assert(pCamera);

        SRenderContext *context = pCamera->GetRenderContext();
        assert(context);

        m_pViewer->GetAppMain()->Render(context); // Render & change buffer
    }

    //----------------------------------------------------------------------------------------------
    void EditorScene3D::MouseMove(size_t dx, size_t dy, int ModifKey)
    {

    }

    //----------------------------------------------------------------------------------------------
    TIEditor EditorScene3D::CreateEdtior(const char *name, CActor *root, EEditorType type)
    {
        CModelViewer *actor = nullptr;
        {
            NEW_OBJECT_TRANSIENT_CHILD(actor, CModelViewer, name, root);

            actor->Initialize();
        }
        return std::move(TIEditor(new editors::EditorScene3D(actor, new CommandBuffer)));
    }
}
