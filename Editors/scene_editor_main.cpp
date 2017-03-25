
#include "D3DDriver.h"
//#include "RenderSDK.h"
#include "ViewportInterface.h"
#include "ViewportManager.h"
#include "CameraManager.h"
#include "Explorer.h"
#include "Explorer3D.h"
#include "command_base.h"
#include "transform_history_traits.h"
#include "CoreSDK.h"
#include <memory>
#include <algorithm>
#include "scene_editor_main.h"

namespace editors
{
    //----------------------------------------------------------------------------------------------
    SceneEditorMain::SceneEditorMain(CCoreSDK *pInstance, ICommandBuffer *buffer)
        : EditorBase(pInstance, static_cast<CActor*>(pInstance->GetExplorerInstance()->GetExplorer3D()), buffer)
        , m_pApi(pInstance)
    {

    }

    //----------------------------------------------------------------------------------------------
    SceneEditorMain::~SceneEditorMain()
    {

    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::Initialize()
    {
        // HACK: set default context
        D3DDriver *pDriver = m_pApi->GetRenderSDK()->GetRenderDriver();
        pDriver->PushContext(GetRenderContext());

        m_pApi->Deserialize("3d_scene_controller.xml", nullptr);

        pDriver->PopContext();

        core_sdk_api::CViewportManager *manager = m_pApi->GetViewportManager();
        Explorer *root = reinterpret_cast<Explorer*>(m_pApi->GetRootActor());
        core_sdk_api::TIViewport *ivprt = manager->GetVeiwportInterface(root->GetExplorer3D());

        ivprt->SetTransformCallback([&](core_sdk_api::TMapState &prevState, core_sdk_api::TMapState &newState)
        {
            AddCommand(std::move(
                std::shared_ptr<CommandBase_>(new CommandBase_(
                    [&, newState]() { m_pApi->GetViewportManager()->ApplyStateCast(newState); },
                    [&, prevState]() { m_pApi->GetViewportManager()->ApplyStateCast(prevState); }))
                ), false);
        });
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
        return m_pApi->GetRenderSDK()->GetRenderDriver()->GetDefaultContext();
    }

    //----------------------------------------------------------------------------------------------
    bool SceneEditorMain::Open(const std::wstring &path)
    {
        bool bResult = false;

        CHAR chFileName[MAX_PATH] = "";
        if (ConvertWideStringToAnsiCch(chFileName, path.c_str(), MAX_PATH))
        {
            // HACK: set default context
            D3DDriver *pDriver = m_pApi->GetRenderSDK()->GetRenderDriver();
            pDriver->PushContext(GetRenderContext());

            m_pApi->Deserialize(chFileName, nullptr);
            
            pDriver->PopContext();

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
        m_pApi->GetRenderSDK()->ResizeWindow(width, height);
        m_pApi->GetCameraManager()->ViewportResized(GetRenderContext());
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::Render()
    {
        m_pApi->Render(nullptr); // Render & change buffer
    }

    //----------------------------------------------------------------------------------------------
    /*void SceneEditorMain::MouseWheel(float ds, int x, int y)
    {
        core_sdk_api::CViewportManager *manager = m_pApi->GetViewportManager();
        core_sdk_api::TIViewport *ivprt = manager->GetVeiwportInterface(RootEntity());

        manager->InputMouseWheel(ds, x, y);
    }*/

   

    //----------------------------------------------------------------------------------------------
    bool SceneEditorMain::GetWireframeMode() const
    {
        return m_pApi->GetRenderSDK()->GetWireframeMode(nullptr);
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::SetWireframeMode(bool flag)
    {
        AddCommand(std::move(
            std::shared_ptr<CommandBase_>(new CommandBase_(
                [&, flag]() { m_pApi->SetWireframeMode(nullptr, flag); },
                [&, flag]() { m_pApi->SetWireframeMode(nullptr,!flag); }))
        ));
    }

    //----------------------------------------------------------------------------------------------
    bool SceneEditorMain::GetObjectBoundsVisible() const
    {
        return m_pApi->GetObjectBoundsVisible();
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::SetObjectBoundsVisible(bool flag)
    {
        AddCommand(std::move(
            std::shared_ptr<CommandBase_>(new CommandBase_(
                [&, flag]() { m_pApi->SetObjectBoundsVisible(flag); },
                [&, flag]() { m_pApi->SetObjectBoundsVisible(!flag); }))
            ));
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
        AddCommand(std::move(
            std::shared_ptr<CommandBase_>(new CommandBase_(
                [&, flag]() { m_pApi->SetShowGrid(flag); },
                [&, flag]() { m_pApi->SetShowGrid(!flag); }))
        ));
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
        EObjEditControlMode oldMode = GetEditControlMode();
        if (oldMode != mode)
        {
            AddCommand(std::move(
                std::shared_ptr<CommandBase_>(new CommandBase_(
                    [&, mode]() { m_pApi->SetEditControlMode(mode); },
                    [&, oldMode]() { m_pApi->SetShowGrid(!oldMode); }))
            ));
        }
    }

    //----------------------------------------------------------------------------------------------
    IDrawInterface* SceneEditorMain::GetByActor(const IObjectAbstract *actor) const
    {
        return m_pApi->GetViewportManager()->GetByActor(actor);
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::AddSelected(CActor *actor)
    {
        assert(false);
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::DelSelected(CActor *actor)
    {
        assert(false);
    }

    //----------------------------------------------------------------------------------------------
   /* void SceneEditorMain::SelectActors(const std::vector<CActor*> &actors)
    {
        // skip if nothing to select and deselect
        if (!m_selection.IsEmpty() || !actors.empty())
        {
            // hack
            core_sdk_api::CViewportManager *manager = m_pApi->GetViewportManager();
            core_sdk_api::TIViewport *ivprt = manager->GetVeiwportInterface(RootEntity());

            oes::editors::SelectionContainer<CActor> old = m_selection;

            AddCommand(std::move(
                std::shared_ptr<CommandBase_>(new CommandBase_(
            [&, manager, ivprt, actors]()
            {
                TMapActorVec mapActors = AdjustActorsToEditorRoot(actors);

                m_selection.Empty();

                std::vector<std::string> ids;
                for each(auto item in mapActors) 
                {
                    ids.push_back(CActor::GetFullPathID(item.first)); // fill new selection

                    for each (auto value in item.second){
                        m_selection.AddItem(item.first, value);
                    }
                }
                manager->SetSelect(ids, ivprt);
 
                m_notifyFunc();
            },
            [&, manager, ivprt, old](){
            
                m_selection.Empty();

                std::vector<std::string> ids;

                auto keys = old.Keys();
                for each(auto item in keys){
                    ids.push_back(CActor::GetFullPathID(item)); // fill new selection
                }

                manager->SetSelect(ids, ivprt);
                m_selection = old;

                m_notifyFunc();
            }))
            ));
        }
    }*/



    //----------------------------------------------------------------------------------------------
 /*   void SceneEditorMain::InputMouse(Event event, MouseCode code, int x, int y, int ModifKey)
    {
        MouseInputData input;
        input.Code = code;
        input.event = event;
        input.MousePos = Vector2f((float)x, (float)y);
        input.ModifKey = ModifKey;
        input.pRenderContext = GetRenderContext();

        Explorer *root = reinterpret_cast<Explorer*>(m_pApi->GetRootActor());
        core_sdk_api::CViewportManager *manager = m_pApi->GetViewportManager();
        core_sdk_api::TIViewport *ivprt = manager->GetVeiwportInterface(root->GetExplorer3D());

        manager->InputMouse(input, ivprt);
    }

    //----------------------------------------------------------------------------------------------
    void SceneEditorMain::MouseMove(int x, int y, const size_t wndx, const size_t wndy, int ModifKey)
    {
        const int dx = x - m_MousePosPrevX;
        const int dy = y - m_MousePosPrevY;

        if (dx != 0 || dy != 0)
        {
            const unsigned int vprtWidth = m_pApi->GetRenderSDK()->GetViewportWidth();
            const unsigned int vprtHeight = m_pApi->GetRenderSDK()->GetViewportHeight();

            const float xPosRel = (wndx > 0.f) ? x / (float)wndx : 0.f;
            const float yPosRel = (wndy > 0.f) ? y / (float)wndy : 0.f;

            const float xdRel = (wndx > 0.f) ? ((dx / (float)wndx) * vprtWidth) : 0.f;
            const float ydRel = (wndy > 0.f) ? ((dy / (float)wndy) * vprtHeight) : 0.f;

            MouseMoveInputData input;

            input.MousePos = Vector2f((float)x, (float)y);
            input.DeltaPos = Vector2f((float)dx, (float)dy);
            input.ModifKey = ModifKey;
            input.pRenderContext = GetRenderContext();

            // hack
            Explorer *root = reinterpret_cast<Explorer*>(m_pApi->GetRootActor());
            core_sdk_api::CViewportManager *manager = m_pApi->GetViewportManager();
            core_sdk_api::TIViewport *ivprt = manager->GetVeiwportInterface(root->GetExplorer3D());

            manager->InputMouse(input, ivprt);

            m_MousePosPrevX = x;
            m_MousePosPrevY = y;
        }
    }*/
}