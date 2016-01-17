#include "../command_buffer.h"
#include <gmock.h>

namespace editors
{
    class DLLEXPORT MockCommandBuffer : public CommandBuffer
    {
    public:
        MOCK_METHOD0(Undo, void());
        MOCK_METHOD0(Redo, void());

    protected:
    private:
    };
}
