#include "../moc/moc_editor.cxx"
#include "../moc/moc_command_buffer.cxx"
#include "../moc/moc_command_base.cxx"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::AtLeast;
using ::testing::Return;

using namespace editors;
//----------------------------------------------------------------------------------------------
TEST(EditorTest, TestUndoRedoCall)
{
    editors::Editor editor(new MockCommandBuffer());

    EXPECT_CALL(*reinterpret_cast<editors::MockCommandBuffer*>(editor.GetCommandBuffer()), Undo()).Times(AtLeast(1));
    EXPECT_CALL(*reinterpret_cast<editors::MockCommandBuffer*>(editor.GetCommandBuffer()), Redo()).Times(AtLeast(1));

    EXPECT_TRUE(editor.Undo());
    EXPECT_TRUE(editor.Redo());
}

//----------------------------------------------------------------------------------------------
TEST(EditorTest, TestCommandAdd)
{
    const int NUM_COMMANDS = 3;

    Editor editor(new MockCommandBuffer());

    std::shared_ptr<MockCommandBase> command0(new MockCommandBase());
    std::shared_ptr<MockCommandBase> command1(new MockCommandBase());
    std::shared_ptr<MockCommandBase> command2(new MockCommandBase());

    editor.AddCommand(std::move(command0));
    editor.AddCommand(std::move(command1));
    editor.AddCommand(std::move(command2));

    EXPECT_TRUE(editor.GetCommandBuffer()->GetSizeUndoStack() == NUM_COMMANDS);
}

//----------------------------------------------------------------------------------------------
TEST(EditorTest, TestCommandUndoRedo)
{
    const int NUM_COMMANDS = 3;

    Editor editor(new MockCommandBuffer);

    std::vector<std::shared_ptr<MockCommandBase>> commands;

    std::shared_ptr<MockCommandBase> command0(new MockCommandBase());
    std::shared_ptr<MockCommandBase> command1(new MockCommandBase());
    std::shared_ptr<MockCommandBase> command2(new MockCommandBase());

    editor.AddCommand(std::move(command0));
    editor.AddCommand(std::move(command1));
    editor.AddCommand(std::move(command2));

    ICommandBuffer *ibuffer = editor.GetCommandBuffer();

    EXPECT_CALL(*reinterpret_cast<editors::MockCommandBuffer*>(ibuffer), Undo())
        .Times(NUM_COMMANDS)
        .WillRepeatedly(testing::InvokeWithoutArgs([&](){
            static_cast<CommandBuffer*>(ibuffer)->CommandBuffer::Undo();
        }));

    for (int i = 1; i <= NUM_COMMANDS; ++i)
    {
        editor.Undo();

        size_t undoSize = ibuffer->GetSizeUndoStack();
        size_t redoSize = ibuffer->GetSizeRedoStack();

        EXPECT_TRUE(undoSize == NUM_COMMANDS - i);
        EXPECT_TRUE(redoSize == i);
    }

    EXPECT_TRUE(editor.GetCommandBuffer()->GetSizeRedoStack() == NUM_COMMANDS);
}
//----------------------------------------------------------------------------------------------
int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
	::testing::InitGoogleMock(&argc, argv);

	return RUN_ALL_TESTS();
}