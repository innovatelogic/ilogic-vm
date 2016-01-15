#include "../editor.h"
#include <gmock.h>

namespace editors
{
    class MockEditor : public Editor
    {
    public:
        MOCK_METHOD0(Undo, bool());
        MOCK_METHOD0(Redo, bool());

    protected:
    private:
    };
}
