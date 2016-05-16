
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

        void	MouseMove(size_t dx, size_t dy, int ModifKey) override;

        bool    GetWireframeMode() const;
        void    SetWireframeMode(bool flag);

        bool    GetObjectBoundsVisible() const override;
        void    SetObjectBoundsVisible(bool flag) override;

        void    AddSelected(const CActor *actor);
        void    DelSelected(const CActor *actor);

        CCoreSDK* GetApp() const override { return m_pApi; }

    protected:
    private:
        std::list<const CActor*> m_selectionList;

        CCoreSDK *m_pApi;
    };
}