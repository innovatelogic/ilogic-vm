#include "scene_editor_main.h"
#include "RenderSDK.h"
#include "../ViewportInterface.h"
#include "../ViewportManager.h"
#include "../Explorer.h"
#include "../Explorer3D.h"
#include <memory>
#include <algorithm>

namespace editors
{
    //----------------------------------------------------------------------------------------------
    SceneEditorMain::SceneEditorMain(CCoreSDK *pInstance, ICommandBuffer *buffer)
        : EditorBase(static_cast<CActor*>(pInstance->GetRootActor()), buffer)
        , m_pApi(pInstance)
        , m_MousePosPrevX(0)
        , m_MousePosPrevY(0)
    {

    }

    //----------------------------------------------------------------------------------------------
    SceneEditorMain::~SceneEditorMain()
    {

    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::Initialize()
    {
        m_pApi->Deserialize("3d_scene_controller.xml", NULL);
    }

    //----------------------------------------------------------------------------------------------
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

    //----------------------------------------------------------------------------------------------
    SRenderContext* SceneEditorMain::GetRenderContext() const
    {
        return nullptr;
    }

    //----------------------------------------------------------------------------------------------
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

    //----------------------------------------------------------------------------------------------
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

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::InitViewport(void *canvas, size_t width, size_t height)
    {
        m_pApi->SetIsAEditor(true);
        m_pApi->SetIsAGame(false);

        m_pApi->Initialize((HWND)canvas, width, height);
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::ResizeVeiwport(size_t width, size_t height)
    {

    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::Render()
    {
        m_pApi->Render(nullptr); // Render & change buffer
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::MouseMove(int x, int y, const size_t wndx, const size_t wndy, int ModifKey)
    {
        int dx = x - m_MousePosPrevX;
        int dy = y - m_MousePosPrevY;

        if (dx != 0 || dy != 0)
        {
            unsigned int vprtWidth = m_pApi->GetRenderSDK()->GetViewportWidth();
            unsigned int vprtHeight = m_pApi->GetRenderSDK()->GetViewportHeight();

            float xPosRel = (wndx > 0.f) ? x / (float)wndx : 0.f;
            float yPosRel = (wndy > 0.f) ? y / (float)wndy : 0.f;

            float xdRel = (wndx > 0.f) ? ((dx / (float)wndx) * vprtWidth) : 0.f;
            float ydRel = (wndy > 0.f) ? ((dy / (float)wndy) * vprtHeight) : 0.f;

            m_pApi->ProcessMouseMove(xPosRel * vprtWidth, yPosRel * vprtHeight, xdRel, ydRel, ModifKey, GetRenderContext());
            
            m_MousePosPrevX = x;
            m_MousePosPrevY = y;
        }
    }

    //----------------------------------------------------------------------------------------------
    bool SceneEditorMain::GetWireframeMode() const
    {
        return m_pApi->GetRenderSDK()->GetWireframeMode(nullptr);
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::SetWireframeMode(bool flag)
    {
        SRenderContext *context = GetRenderContext();

        class CommandSetWireframe : public ICommand
        {
        public:
            CommandSetWireframe(CCoreSDK *api, bool value)
                : m_value(value)
                , m_api(api) { Execute(); }

            void Execute() override { m_api->SetWireframeMode(nullptr, m_value); }
            void ExecuteUndo() override { m_api->SetWireframeMode(nullptr, !m_value); }
            
        private:
            bool m_value;
            CCoreSDK *m_api;
        };

        AddCommand(std::move(std::shared_ptr<CommandSetWireframe>(new CommandSetWireframe(m_pApi, flag))));
    }

    //----------------------------------------------------------------------------------------------
    bool SceneEditorMain::GetObjectBoundsVisible() const
    {
        return m_pApi->GetObjectBoundsVisible();
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::SetObjectBoundsVisible(bool flag)
    {
        class CommandSetBBox : public ICommand
        {
        public:
            CommandSetBBox(CCoreSDK *api, bool value)
                : m_value(value)
                , m_api(api) {
                Execute();
            }

            void Execute() override { m_api->SetObjectBoundsVisible(m_value); }
            void ExecuteUndo() override { m_api->SetObjectBoundsVisible(!m_value); }

        private:
            bool m_value;
            CCoreSDK *m_api;
        };

        AddCommand(std::move(std::shared_ptr<CommandSetBBox>(new CommandSetBBox(m_pApi, flag))));
    }

    //----------------------------------------------------------------------------------------------
    bool SceneEditorMain::GetSpartialSubdivisionVisible() const
    {
        return m_pApi->GetSparitalSubdivisionVisible();
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::SetSpartialSubdivisionVisible(bool flag)
    {

    }

    //----------------------------------------------------------------------------------------------
    bool SceneEditorMain::GetGridVisible() const
    {
        return m_pApi->GetShowGrid();
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::SetGridVisible(bool flag)
    {
        class CommandSetShowGrid : public ICommand
        {
        public:
            CommandSetShowGrid(CCoreSDK *api, bool value)
                : m_value(value)
                , m_api(api) {
                Execute();
            }

            void Execute() override { m_api->SetShowGrid(m_value); }
            void ExecuteUndo() override { m_api->SetShowGrid(!m_value); }

        private:
            bool m_value;
            CCoreSDK *m_api;
        };

        AddCommand(std::move(std::shared_ptr<CommandSetShowGrid>(new CommandSetShowGrid(m_pApi, flag))));
    }

    //----------------------------------------------------------------------------------------------
    bool SceneEditorMain::GetCollisionDebugVisible() const
    {
        return m_pApi->GetCollisionDebugRender();
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::SetCollisionDebugVisible(bool flag)
    {
        m_pApi->SetCollisionDebugRender(flag);
    }
    
    //----------------------------------------------------------------------------------------------
    EObjEditControlMode	SceneEditorMain::GetEditControlMode() const
    {
        return m_pApi->GetEditControlMode();
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::SetEditControlMode(EObjEditControlMode mode)
    {
        // Command
        class CommandSetShowGrid : public ICommand
        {
        public:
            CommandSetShowGrid(CCoreSDK *api,  EObjEditControlMode vNew, EObjEditControlMode vOld)
                : m_new(vNew)
                , m_old(vOld)
                , m_api(api) {
                Execute();
            }
            void Execute() override { m_api->SetEditControlMode(m_new); }
            void ExecuteUndo() override { m_api->SetEditControlMode(m_old); }

        private:
            EObjEditControlMode m_new;
            EObjEditControlMode m_old;
            CCoreSDK *m_api;
        };
        // End Command

        EObjEditControlMode oldMode = GetEditControlMode();
        if (oldMode != mode)
        {
            AddCommand(std::move(std::shared_ptr<CommandSetShowGrid>(new CommandSetShowGrid(m_pApi, mode, oldMode))));
        }
    }

    //----------------------------------------------------------------------------------------------
    IDrawInterface* SceneEditorMain::GetByActor(const CActor *actor) const
    {
        return m_pApi->GetViewportManager()->GetByActor(actor);
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::AddSelected(CActor *actor)
    {
        std::vector<CActor*>::const_iterator iterFind = std::find(m_selectionList.begin(), m_selectionList.end(), actor);

        if (iterFind == m_selectionList.end())
        {
            m_selectionList.push_back(actor);
        }
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::DelSelected(CActor *actor)
    {
        std::vector<CActor*>::iterator iterFind = std::find(m_selectionList.begin(), m_selectionList.end(), actor);

        if (iterFind != m_selectionList.end())
        {
            m_selectionList.erase(iterFind);
        }
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::SelectActors(const std::vector<CActor*> &actors)
    {
        class CommandSetSelectActors : public ICommand
        {
        public:
            CommandSetSelectActors(core_sdk_api::CViewportManager *manager,
                core_sdk_api::ViewportInterface *ivprt,
                const std::vector<CActor*> &actors,
                const std::vector<CActor*> &old_actors)
                : m_manager(manager)
                , m_ivprt(ivprt)
            {
                for each(auto *actor in actors) {
                    m_new.push_back(CActor::GetFullPathID(actor)); // fill new selection
                }
                for each(auto *actor in old_actors) {
                    m_old.push_back(CActor::GetFullPathID(actor)); // fill old selection
                }
                Execute();
            }
            void Execute() override { m_manager->SetSelect(m_new, m_ivprt); }
            void ExecuteUndo() override { m_manager->SetSelect(m_old, m_ivprt); }

        private:
            std::vector<std::string> m_new;
            std::vector<std::string> m_old;
            core_sdk_api::CViewportManager *m_manager;
            core_sdk_api::ViewportInterface *m_ivprt;
        };

        // skip if nothing to select and deselect
        if (!m_selectionList.empty() || !actors.empty())
        {
            // hack
            Explorer *root = reinterpret_cast<Explorer*>(m_pApi->GetRootActor());

            core_sdk_api::CViewportManager *manager = m_pApi->GetViewportManager();
            core_sdk_api::ViewportInterface *ivprt = manager->GetVeiwportInterface(root->GetExplorer3D());

            AddCommand(std::move(std::shared_ptr<CommandSetSelectActors>(new CommandSetSelectActors(manager, ivprt, actors, m_selectionList))));

            m_selectionList = actors;
        }
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::DeselectAll()
    {
        SelectActors({});
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::InputMouse(Event event, MouseCode code, int x, int y, int ModifKey /*= 0*/)
    {
        m_pApi->ProcessInputMouse(event, code, x, y, ModifKey, GetRenderContext());
    }
}