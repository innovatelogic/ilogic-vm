#include "../command_buffer.h"
#include <gmock/gmock.h>

namespace editors
{
    class DLLEXPORT MockCommandBuffer : public CommandBuffer
    {
    public:
        MOCK_METHOD0(Undo, void());
        MOCK_METHOD0(Redo, void());

        MOCK_METHOD0(GetSizeUndoStack, size_t());
        MOCK_METHOD0(GetSizeRedoStack, size_t());

        MOCK_METHOD1(GetUndoCommandBatchSize, int(size_t index));
        MOCK_METHOD1(GetRedoCommandBatchSize, int(size_t index));

    protected:
    private:
    };
}
