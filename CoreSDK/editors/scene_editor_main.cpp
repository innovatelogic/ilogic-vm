#include "scene_editor_main.h"
#include "RenderSDK.h"
#include <memory>

namespace editors
{
    SceneEditorMain::SceneEditorMain(CCoreSDK *pInstance, ICommandBuffer *buffer)
        : EditorBase(static_cast<CActor*>(pInstance->GetRootActor()), buffer)
        , m_pApi(pInstance)
    {

    }

    SceneEditorMain::~SceneEditorMain()
    {

    }

    void SceneEditorMain::Initialize()
    {
        m_pApi->Deserialize("3d_scene_controller.xml", NULL);
    }

    void SceneEditorMain::Update(float deltaTime)
    {
        m_pApi->ElapsedTime = deltaTime;

        m_pApi->GetRenderSDK()->SetDeltaTime(deltaTime);

        m_pApi->ProcessUpdate(deltaTime);

        m_pApi->Draw(); // draw new frame to back buffer

        m_pApi->SimulatePhysics(deltaTime);

        /*{
            static float LastTimeUpdate = 0.f;
            static float DeltaCall = 0.f;

            float DeltaTime = AppTimePassed - LastTimeUpdate;
            LastTimeUpdate = AppTimePassed;

            DeltaCall += DeltaTime;

            //if (DeltaCall > 0.001f)
            {
                GRootProcess->SimulatePhysics(DeltaCall * 1.4f);
                DeltaCall = 0.f;
            }
        }*/
    }

    SRenderContext* SceneEditorMain::GetRenderContext() const
    {
        return nullptr;
    }

    bool SceneEditorMain::Open(const std::wstring &path)
    {
        bool bResult = false;

        CHAR chFileName[MAX_PATH] = "";
        if (ConvertWideStringToAnsiCch(chFileName, path.c_str(), MAX_PATH))
        {
            m_pApi->Deserialize(chFileName, nullptr);
            bResult = true;
        }
        return bResult;
    }

    bool SceneEditorMain::Save(const std::wstring &path)
    {
        bool bResult = false;

        CHAR chFileName[MAX_PATH] = "";
        if (ConvertWideStringToAnsiCch(chFileName, path.c_str(), MAX_PATH))
        {
            m_pApi->Serialize(chFileName);
            bResult = true;
        }
        return bResult;
    }

    void SceneEditorMain::InitViewport(void *canvas, size_t width, size_t height)
    {
        m_pApi->SetIsAEditor(true);
        m_pApi->SetIsAGame(false);

        m_pApi->Initialize((HWND)canvas, width, height);
    }

    void SceneEditorMain::ResizeVeiwport(size_t width, size_t height)
    {

    }

    void SceneEditorMain::Render()
    {
        m_pApi->Render(nullptr); // Render & change buffer
    }

    void SceneEditorMain::MouseMove(size_t dx, size_t dy, int ModifKey)
    {

    }

    bool SceneEditorMain::GetWireframeMode() const
    {
        return m_pApi->GetRenderSDK()->GetWireframeMode();
    }

    void SceneEditorMain::SetWireframeMode(bool flag)
    {
        class CommandSetWireframe : public ICommand
        {
        public:
            CommandSetWireframe(CCoreSDK *api, bool value)
                : m_value(value)
                , m_api(api) { Execute(); }

            void Execute() { m_api->GetRenderSDK()->SetWireframeMode(m_value); }
            void ExecuteUndo() { m_api->GetRenderSDK()->SetWireframeMode(!m_value); }
            
        private:
            bool m_value;
            CCoreSDK *m_api;
        };

        AddCommand(std::move(std::shared_ptr<CommandSetWireframe>(new CommandSetWireframe(m_pApi, flag))));
    }

}