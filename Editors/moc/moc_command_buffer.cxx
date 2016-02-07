#include "../icommand_buffer.h"
#include <gmock/gmock.h>

namespace editors
{
    class DLLEXPORT MockCommandBuffer : public ICommandBuffer
    {
    public:
        MOCK_METHOD1(AddCommand, void(ICommandPtr command));
        MOCK_METHOD1(AddCommands, void(ICommandPtrList &commands));

        MOCK_METHOD0(Undo, void());
        MOCK_METHOD0(Redo, void());

        MOCK_METHOD0(Clear, void());

        MOCK_CONST_METHOD0(GetSizeUndoStack, size_t());
        MOCK_CONST_METHOD0(GetSizeRedoStack, size_t());

        MOCK_CONST_METHOD1(GetUndoCommandBatchSize, size_t(size_t index));
        MOCK_CONST_METHOD1(GetRedoCommandBatchSize, size_t(size_t index));


    protected:
    private:
    };
}
