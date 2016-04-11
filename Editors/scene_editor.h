#pragma once

#include "editor.h"

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

    class DLLEXPORT EditorScene3D : public EditorBase
    {
    public:
        EditorScene3D(CModelViewer *viewer, ICommandBuffer *buffer);
        virtual ~EditorScene3D();

        SRenderContext* GetRenderContext() const override;

        bool    Open(std::wstring &path) override;
        bool    Save(std::wstring &path) override;

        void    InitViewport(void *canvas, size_t width, size_t height) override;
        void    ResizeVeiwport(size_t width, size_t height) override;

        void    Render() override;

        void	MouseMove(float dx, float dy, int ModifKey) override;

        static TIEditor CreateEdtior(const char *name, CActor *root, EEditorType type);

    protected:

    private:
        CModelViewer        *m_pViewer;
    };
}
