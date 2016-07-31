#include "Actor.h"
#include "editor.h"
#include "ViewportInterface.h"
#include "ViewportManager.h"
#include "command_base.h"

namespace editors
{
//----------------------------------------------------------------------------------------------
EditorBase::EditorBase(CCoreSDK *pInstance, CActor *actor, ICommandBuffer *buffer)
    : m_MousePosPrevX(0)
    , m_MousePosPrevY(0)
    , m_bShiftPressed(false)
    , m_bCtrlPressed(false)
    , m_CommandBuffer(buffer)
    , m_pEditorRoot(actor)
    , m_pApi(pInstance)
{

}

//----------------------------------------------------------------------------------------------
EditorBase::~EditorBase()
{

}

//----------------------------------------------------------------------------------------------
bool EditorBase::Undo()
{
    assert(m_CommandBuffer);

    m_CommandBuffer->Undo();

    return true;
}

//----------------------------------------------------------------------------------------------
bool EditorBase::Redo()
{
    assert(m_CommandBuffer);

    m_CommandBuffer->Redo();

    return true;
}

//----------------------------------------------------------------------------------------------
void EditorBase::AddCommand(ICommandPtr command, bool execute /*= true*/)
{
    assert(m_CommandBuffer);

    m_CommandBuffer->AddCommand(std::move(command), execute);
}

//----------------------------------------------------------------------------------------------
void EditorBase::AddCommandBatch(ICommandPtrList &vector)
{
    assert(m_CommandBuffer);

    m_CommandBuffer->AddCommands(vector);
}

//----------------------------------------------------------------------------------------------
size_t EditorBase::GetUndoCommandBatchSize(size_t index) const
{
    assert(m_CommandBuffer);

    return m_CommandBuffer->GetUndoCommandBatchSize(index);
}

//----------------------------------------------------------------------------------------------
size_t EditorBase::GetRedoCommandBatchSize(size_t index) const
{
    assert(m_CommandBuffer);

    return m_CommandBuffer->GetRedoCommandBatchSize(index);
}

//----------------------------------------------------------------------------------------------
void EditorBase::SelectActors(const std::vector<CActor*> &actors)
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

                for each (auto value in item.second) {
                    m_selection.AddItem(item.first, value);
                }
            }
            manager->SetSelect(ids, ivprt);

            m_notifySelectFunc();
        },
                [&, manager, ivprt, old]() {

            m_selection.Empty();

            std::vector<std::string> ids;

            auto keys = old.Keys();
            for each(auto item in keys) {
                ids.push_back(CActor::GetFullPathID(item)); // fill new selection
            }

            manager->SetSelect(ids, ivprt);
            m_selection = old;

            m_notifySelectFunc();
        }))
            ));
    }
}

//----------------------------------------------------------------------------------------------
void EditorBase::DeselectAll()
{
    SelectActors({});
}

//----------------------------------------------------------------------------------------------
void EditorBase::InputMouse(Event event, MouseCode code, int x, int y, int ModifKey /*= 0*/)
{
    MouseInputData input;
    input.Code = code;
    input.event = event;
    input.MousePos = Vector2f((float)x, (float)y);
    input.ModifKey = ModifKey;
    input.pRenderContext = GetRenderContext();

    core_sdk_api::CViewportManager *manager = m_pApi->GetViewportManager();
    core_sdk_api::TIViewport *ivprt = manager->GetVeiwportInterface(RootEntity());

    manager->InputMouse(input, ivprt);
}

//----------------------------------------------------------------------------------------------
void EditorBase::MouseMove(int x, int y, const size_t wndx, const size_t wndy, int ModifKey)
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

        core_sdk_api::CViewportManager *manager = m_pApi->GetViewportManager();
        core_sdk_api::TIViewport *ivprt = manager->GetVeiwportInterface(RootEntity());

        manager->InputMouse(input, ivprt);

        m_MousePosPrevX = x;
        m_MousePosPrevY = y;
    }
}

//----------------------------------------------------------------------------------------------
void EditorBase::MouseWheel(float ds, int x, int y)
{
    core_sdk_api::CViewportManager *manager = m_pApi->GetViewportManager();
    core_sdk_api::TIViewport *ivprt = manager->GetVeiwportInterface(RootEntity());

    manager->InputMouseWheel(ds, x, y, GetRenderContext());
}

//----------------------------------------------------------------------------------------------
void EditorBase::InputKey(const EventInput &InputData)
{
    m_pApi->ProcessInputKey(InputData);
}

//----------------------------------------------------------------------------------------------
const CActor* EditorBase::GetEditorRelatedActor(const CActor *actor)
{
    if (actor && actor->GetExternal())
    {
        while (actor)
        {
            if (!actor->GetExternal()){
                break; // found
            }
            actor = actor->GetParent();
        }
    }
    return actor;
}

//----------------------------------------------------------------------------------------------
EditorBase::TMapActorVec EditorBase::AdjustActorsToEditorRoot(const std::vector<CActor*> &actors)
{
    std::map<const CActor*, std::vector<CActor*>> out;

    for each (auto actor in actors)
    {
        const auto *key = GetEditorRelatedActor(actor);

        assert(key);

        const auto iterFind = out.find(key);
        if (iterFind == out.end())
        {
            out[key] = { actor };
        }
        else
        {
            std::vector<CActor*> &refVec = iterFind->second;

            if (std::find(refVec.begin(), refVec.end(), actor) == refVec.end())
            {
                refVec.push_back(actor);
            }
        }
    }
    return out;
}
}
