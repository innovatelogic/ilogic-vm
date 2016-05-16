#pragma once

#include "editor.h"
#include "../game_types.h" 

class SRenderContext;
class CComp_EnvSceneInfo;
class CCamera;
class CModelViewer;
class CActor;

namespace editors
{
    enum class EEditorType
    {
        EEditorDefault = 0,
        EEditorScene2D,
    };

    class CORESDK_API EditorScene3D : public EditorBase
    {
    public:
        EditorScene3D(CModelViewer *viewer, ICommandBuffer *buffer);
        virtual ~EditorScene3D();

        void Initialize() override;

        void Update(float deltaTime) override;

        CCoreSDK* GetApp() const override;

        SRenderContext* GetRenderContext() const override;

        bool    Open(const std::wstring &path) override;
        bool    Save(const std::wstring &path) override;

        void    InitViewport(void *canvas, size_t width, size_t height) override;
        void    ResizeVeiwport(size_t width, size_t height) override;

        void    Render() override;

        void	MouseMove(size_t dx, size_t dy, int ModifKey) override;

        static TIEditor CreateEdtior(const char *name, CActor *root, EEditorType type);

        bool    GetWireframeMode() const override;
        void    SetWireframeMode(bool flag) override;

        bool    GetObjectBoundsVisible() const override;
        void    SetObjectBoundsVisible(bool flag) override;

    protected:
    private:
        CModelViewer        *m_pViewer;
    };
}
