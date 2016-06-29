
#pragma once

#include "editor.h"
#include "../game_types.h"
#include "../CoreSDK.h"

namespace editors
{
    class CORESDK_API SceneEditorMain : public EditorBase
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

        void	InputMouse(Event event, MouseCode code, int x, int y, int modifKey = 0) override;
        void	MouseMove(int x, int y, const size_t wndx, const size_t wndy, int modifKey = 0) override;
        void	MouseWheel(float ds, int x, int y) override;
        void	InputKey(const EventInput &input) override;

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

        CCoreSDK* GetApp() const override { return m_pApi; }

        void    SelectActors(const std::vector<CActor*> &actors) override;
        void    DeselectAll() override;

    protected:
    private:
        unsigned int m_MousePosPrevX;
        unsigned int m_MousePosPrevY;

        bool m_bShiftPressed;
        bool m_bCtrlPressed;

        CCoreSDK *m_pApi;
    };
}