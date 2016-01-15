#pragma once

#include "icommand_buffer.h"

namespace editors
{
class DLLEXPORT CommandBuffer : public ICommandBuffer
{
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
};
}