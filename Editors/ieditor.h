#pragma once

#include "icommand.h"
#include "icommand_buffer.h"
#include "StdafxFoundation.h"
#include <memory>
#include <assert.h>

class SRenderContext;
class CCoreSDK;
class IDrawInterface;

namespace editors
{
class DLLEXPORT IEditor
{
public:
	virtual ~IEditor() = 0 {};

    virtual void    Initialize() = 0;

    virtual bool    Undo() = 0;
    virtual bool    Redo() = 0;

    virtual void    AddCommand(ICommandPtr command, bool execute = true) = 0;
    virtual void    AddCommandBatch(ICommandPtrList &vector) = 0;

    virtual size_t  GetUndoCommandBatchSize(size_t index) const = 0;
    virtual size_t  GetRedoCommandBatchSize(size_t index) const = 0;

    virtual SRenderContext* GetRenderContext() const = 0;

    virtual bool    Open(const std::wstring &path) = 0;
    virtual bool    Save(const std::wstring &path) = 0;

    virtual void    InitViewport(void *canvas, size_t width, size_t height) = 0;
    virtual void    ResizeVeiwport(size_t width, size_t height) = 0;

    virtual void    Render() = 0;

    virtual void	InputMouse(Event event, MouseCode code, int x, int y, int ModifKey = 0) = 0;
    virtual void	MouseMove(int x, int y, const size_t wndx, const size_t wndy, int modifKey = 0) = 0;
    virtual void	MouseWheel(float ds, int x, int y) = 0;
    virtual void	InputKey(const EventInput &InputData) = 0;

    virtual CCoreSDK* GetApp() const = 0;

    virtual void    Update(float deltaTime) = 0;

    virtual bool    GetWireframeMode() const = 0;
    virtual void    SetWireframeMode(bool flag) = 0;

    virtual bool    GetObjectBoundsVisible() const = 0;
    virtual void    SetObjectBoundsVisible(bool flag) = 0;

    virtual bool    GetSpartialSubdivisionVisible() const = 0;
    virtual void    SetSpartialSubdivisionVisible(bool flag) = 0;

    virtual bool    GetGridVisible() const = 0;
    virtual void    SetGridVisible(bool flag) = 0;

    virtual bool    GetCollisionDebugVisible() const = 0;
    virtual void    SetCollisionDebugVisible(bool flag) = 0;

    virtual EObjEditControlMode		GetEditControlMode() const = 0;
    virtual void					SetEditControlMode(EObjEditControlMode mode) = 0;

    virtual IDrawInterface* GetByActor(const CActor *actor) const = 0;
    
    /*!
    * commits selection command. empty vector equal to deselect all
    */
    virtual void    SelectActors(const std::vector<CActor*> &actors) = 0;

    /*!
    * commits deselect command. 
    * does not have effect if no previous actors selected
    */
    virtual void    DeselectAll() = 0;

protected:
private:
};

using TIEditor = std::shared_ptr<IEditor>;
}