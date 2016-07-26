
#pragma once

#include "module.h"
#include "editor.h"

class CActor;
class CCoreSDK;

namespace editors
{
    class DLLEXPORT SceneEditorMain : public EditorBase
    {
    public:
        SceneEditorMain(CCoreSDK *pInstance, ICommandBuffer *buffer);
        virtual ~SceneEditorMain();

        void Initialize() override;

        void Update(float deltaTime) override;

        SRenderContext* GetRenderContext() const override;

        bool    Open(const std::wstring &path) override;
        bool    Save(const std::wstring &path) override;

        void    InitViewport(void *canvas, size_t width, size_t height) override;
        void    ResizeVeiwport(size_t width, size_t height) override;

        void    Render() override;

        bool    GetWireframeMode() const;
        void    SetWireframeMode(bool flag);

        bool    GetObjectBoundsVisible() const override;
        void    SetObjectBoundsVisible(bool flag) override;

        bool    GetSpartialSubdivisionVisible() const override;
        void    SetSpartialSubdivisionVisible(bool flag) override;

        bool    GetGridVisible() const override;
        void    SetGridVisible(bool flag) override;

        bool    GetCollisionDebugVisible() const override;
        void    SetCollisionDebugVisible(bool flag) override;

        EObjEditControlMode		GetEditControlMode() const override;
        void					SetEditControlMode(EObjEditControlMode mode) override;

        IDrawInterface* GetByActor(const CActor *actor) const override;

        void    AddSelected(CActor *actor);
        void    DelSelected(CActor *actor);
        
    protected:
    private:
        CCoreSDK *m_pApi;
    };
}