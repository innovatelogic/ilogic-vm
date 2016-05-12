
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

        void Initialize();

        void Update(float deltaTime);

        SRenderContext* GetRenderContext() const override;

        bool    Open(const std::wstring &path) override;
        bool    Save(const std::wstring &path) override;

        void    InitViewport(void *canvas, size_t width, size_t height) override;
        void    ResizeVeiwport(size_t width, size_t height) override;

        void    Render() override;

        void	MouseMove(size_t dx, size_t dy, int ModifKey) override;

        bool    GetWireframeMode() const;
        void    SetWireframeMode(bool flag);

    protected:
    private:
        CCoreSDK *m_pApi;
    };
}