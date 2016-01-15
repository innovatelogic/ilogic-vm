#include "editor.h"
#include <gmock.h>

namespace editors
{
    class MockEditor : public Editor
    {
    public:
        MOCK_METHOD0(Undo, void());
        MOCK_METHOD0(Redo, void());

    protected:
    private:
    };
}
