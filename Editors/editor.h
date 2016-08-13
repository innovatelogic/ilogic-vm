
#pragma once

#include "ieditor.h"
#include "icommand.h"
#include "icommand_buffer.h"
#include "command_buffer.h"
#include "selection_container.h"

class CActor;

namespace editors
{
class DLLEXPORT EditorBase : public IEditor
{
public:
    using TVecConstActor = std::vector<const CActor*>;
    using TMapActorVec = std::map<const CActor*, std::vector<CActor*>>;

    EditorBase(CCoreSDK *pInstance, CActor *actor, ICommandBuffer *buffer);
	virtual ~EditorBase();

    bool Undo() override;
    bool Redo() override;

    void AddCommand(ICommandPtr command, bool execute = true) override;
    void AddCommandBatch(ICommandPtrList &vector) override;
	
    ICommandBuffer* GetCommandBuffer() const { return m_CommandBuffer; }

    size_t GetUndoCommandBatchSize(size_t index) const override;
    size_t GetRedoCommandBatchSize(size_t index) const override;

    /*!
    * Set callback selection
    */
    void SetNotifySelectFunc(const std::function<void()> &func) override { m_notifySelectFunc = func; }

    std::vector<const CActor*> GetSelected() const override { return m_selection.Values(); }

    void SelectActors(const std::vector<CActor*> &actors);
    void DeselectAll() override;

    // input
    void	InputMouse(Event event, MouseCode code, int x, int y, int modifKey = 0) override;
    void	MouseMove(int x, int y, const size_t wndx, const size_t wndy, int modifKey = 0) override;
    void	MouseWheel(float ds, int x, int y) override;
    void	InputKey(const EventInput &input) override;

    std::string GetProperty(const CObjectAbstract* object, const Property_Base *prop) const override;

protected:
    /*!
     *  Returns a lowest editor-root related to input actor
     */
    const CActor* GetEditorRelatedActor(const CActor *actor);

    /*!
    * return correspondence map where a key actor is in root's space explicitly,
    * values in vector arbitrary implicit scope of selected actors
    */
    TMapActorVec AdjustActorsToEditorRoot(const std::vector<CActor*> &actors);

    CActor* RootEntity() const { return m_pEditorRoot; }

    CCoreSDK* GetApp() const override { return m_pApi; }

protected:
    std::function<void()>   m_notifySelectFunc;

    oes::editors::SelectionContainer<CActor> m_selection;

private:
    unsigned int m_MousePosPrevX;
    unsigned int m_MousePosPrevY;
    bool m_bShiftPressed;
    bool m_bCtrlPressed;

    ICommandBuffer *m_CommandBuffer;

    CActor *m_pEditorRoot;

    CCoreSDK *m_pApi;
};
}
