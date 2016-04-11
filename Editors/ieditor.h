#pragma once

#include <memory>
#include <assert.h>
#include "icommand.h"
#include "icommand_buffer.h"

class SRenderContext;

namespace editors
{
class DLLEXPORT IEditor
{
public:
	virtual ~IEditor() = 0 {};

    virtual bool    Undo() = 0;
    virtual bool    Redo() = 0;

    virtual void    AddCommand(ICommandPtr command) = 0;
    virtual void    AddCommandBatch(ICommandPtrList &vector) = 0;

    virtual size_t  GetUndoCommandBatchSize(size_t index) const = 0;
    virtual size_t  GetRedoCommandBatchSize(size_t index) const = 0;

    virtual SRenderContext* GetRenderContext() const = 0;

    virtual bool    Open(std::wstring &path) = 0;
    virtual bool    Save(std::wstring &path) = 0;

    virtual void    InitViewport(void *canvas, size_t width, size_t height) = 0;
    virtual void    ResizeVeiwport(size_t width, size_t height) = 0;

    virtual void    Render() = 0;

    virtual void	MouseMove(float dx, float dy, int ModifKey) = 0;

protected:
private:
};

using TIEditor = std::shared_ptr<IEditor>;
}