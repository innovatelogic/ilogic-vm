#pragma once

#include "icommand_buffer.h"
#include "icommand.h"
#include <memory>
#include <stack>
#include <vector>

namespace editors
{
class DLLEXPORT CommandBuffer : public ICommandBuffer
{
    typedef std::unique_ptr<ICommand> ICommandPtr;

    struct SCommandButch
    {
        std::vector<ICommandPtr> batch;
    };

public:
    CommandBuffer();
    virtual ~CommandBuffer();

    void AddCommand(ICommand *command);
    void AddCommands(std::vector<ICommand*> *commands) override;

    void Clear() override;

    void Undo() override;
    void Redo() override;

protected:
private:
    //std::stack< std::unique_ptr<SCommandButch> > m_undo;
    //std::stack< std::unique_ptr<SCommandButch> > m_redo;
};
}