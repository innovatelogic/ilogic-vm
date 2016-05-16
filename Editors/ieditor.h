#pragma once

#include <memory>
#include <assert.h>
#include "icommand.h"
#include "icommand_buffer.h"

class SRenderContext;
class CCoreSDK;

namespace editors
{
class DLLEXPORT IEditor
{
public:
	virtual ~IEditor() = 0 {};

    virtual void    Initialize() = 0;

    virtual bool    Undo() = 0;
    virtual bool    Redo() = 0;

    virtual void    AddCommand(ICommandPtr command) = 0;
    virtual void    AddCommandBatch(ICommandPtrList &vector) = 0;

    virtual size_t  GetUndoCommandBatchSize(size_t index) const = 0;
    virtual size_t  GetRedoCommandBatchSize(size_t index) const = 0;

    virtual SRenderContext* GetRenderContext() const = 0;

    virtual bool    Open(const std::wstring &path) = 0;
    virtual bool    Save(const std::wstring &path) = 0;

    virtual void    InitViewport(void *canvas, size_t width, size_t height) = 0;
    virtual void    ResizeVeiwport(size_t width, size_t height) = 0;

    virtual void    Render() = 0;

    virtual void	MouseMove(size_t dx, size_t dy, int ModifKey) = 0;

    virtual CCoreSDK* GetApp() const = 0;

    virtual void Update(float deltaTime) = 0;

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

protected:
private:
};

using TIEditor = std::shared_ptr<IEditor>;
}