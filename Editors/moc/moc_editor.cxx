#include "../ieditor.h"
#include <gmock/gmock.h>

namespace editors
{
    class MockEditor : public IEditor
    {
    public:
        MOCK_METHOD0(Undo, bool());
        MOCK_METHOD0(Redo, bool());

    protected:
    private:
    };
}
