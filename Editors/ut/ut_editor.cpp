#include "../moc/moc_editor.cxx"
#include "../moc/moc_command_buffer.cxx"
#include "../moc/moc_command_base.cxx"
#include "../command_base.h"
#include "../command_buffer.h"
#include "../editor.h"
//#include "scene_editor.h"

#include "Actor.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::AtLeast;
using ::testing::Return;

using namespace editors;
//----------------------------------------------------------------------------------------------
/*TEST(EditorTest, TestUndoRedoCall)
{
    MockCommandBuffer buffer;
    editors::EditorScene3D editor(nullptr, &buffer);

    EXPECT_CALL(*reinterpret_cast<editors::MockCommandBuffer*>(editor.GetCommandBuffer()), Undo()).Times(AtLeast(1));
    EXPECT_CALL(*reinterpret_cast<editors::MockCommandBuffer*>(editor.GetCommandBuffer()), Redo()).Times(AtLeast(1));

    EXPECT_TRUE(editor.Undo());
    EXPECT_TRUE(editor.Redo());
}

//----------------------------------------------------------------------------------------------
TEST(EditorTest, TestCommandAdd)
{
    const int NUM_COMMANDS = 3;

    CommandBuffer buffer;
    EditorScene3D editor(nullptr, &buffer);

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

    CommandBuffer buffer;
    EditorScene3D editor(nullptr, &buffer);

    std::vector<std::shared_ptr<editors::CommandBase>> commands;

    std::shared_ptr<MockCommandBase> command0(new MockCommandBase());
    std::shared_ptr<MockCommandBase> command1(new MockCommandBase());
    std::shared_ptr<MockCommandBase> command2(new MockCommandBase());

    editor.AddCommand(command0);
    editor.AddCommand(command1);
    editor.AddCommand(command2);

    ICommandBuffer *ibuffer = editor.GetCommandBuffer();

    EXPECT_CALL(*command0.get(), ExecuteUndo()).Times(1);
    EXPECT_CALL(*command1.get(), ExecuteUndo()).Times(1);
    EXPECT_CALL(*command2.get(), ExecuteUndo()).Times(1);

    EXPECT_CALL(*command0.get(), ExecuteRedo()).Times(1);
    EXPECT_CALL(*command1.get(), ExecuteRedo()).Times(1);
    EXPECT_CALL(*command2.get(), ExecuteRedo()).Times(1);
    
    // run undo's
    for (int i = 1; i <= NUM_COMMANDS; ++i)
    {
        editor.Undo();

        size_t undoSize = ibuffer->GetSizeUndoStack();
        size_t redoSize = ibuffer->GetSizeRedoStack();

        EXPECT_TRUE(undoSize == NUM_COMMANDS - i);
        EXPECT_TRUE(redoSize == i);
    }

    EXPECT_TRUE(editor.GetCommandBuffer()->GetSizeRedoStack() == NUM_COMMANDS);

    // run redo's
    for (int i = 1; i <= NUM_COMMANDS; ++i)
    {
        editor.Redo();

        size_t undoSize = ibuffer->GetSizeUndoStack();
        size_t redoSize = ibuffer->GetSizeRedoStack();

        EXPECT_TRUE(redoSize == NUM_COMMANDS - i);
        EXPECT_TRUE(undoSize == i);
    }

    EXPECT_TRUE(editor.GetCommandBuffer()->GetSizeUndoStack() == NUM_COMMANDS);
}

//----------------------------------------------------------------------------------------------
TEST(EditorTest, TestCommandBatchAdd)
{
    const size_t NUM_COMMANDS = 3;

    CommandBuffer buffer;
    EditorScene3D editor(nullptr, &buffer);

    ICommandPtrList command_batch0 = {
        std::shared_ptr<MockCommandBase>(new MockCommandBase()),
        std::shared_ptr<MockCommandBase>(new MockCommandBase()),
        std::shared_ptr<MockCommandBase>(new MockCommandBase()),
    };

    ICommandPtrList command_batch1 = {
        std::shared_ptr<MockCommandBase>(new MockCommandBase()),
        std::shared_ptr<MockCommandBase>(new MockCommandBase()),
    };

    std::shared_ptr<MockCommandBase> command_simple(new MockCommandBase());

    editor.AddCommandBatch(command_batch0);
    editor.AddCommandBatch(command_batch1);
    editor.AddCommand(command_simple);

    EXPECT_TRUE(editor.GetUndoCommandBatchSize(0) == 1);
    EXPECT_TRUE(editor.GetUndoCommandBatchSize(1) == command_batch1.size());
    EXPECT_TRUE(editor.GetUndoCommandBatchSize(2) == command_batch0.size());

    for (int i = 0; i < NUM_COMMANDS; ++i) {
        editor.Undo();
    }

    EXPECT_TRUE(editor.GetRedoCommandBatchSize(0) == command_batch0.size());
    EXPECT_TRUE(editor.GetRedoCommandBatchSize(1) == command_batch1.size());
    EXPECT_TRUE(editor.GetRedoCommandBatchSize(2) == 1);
}

//----------------------------------------------------------------------------------------------
int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
	::testing::InitGoogleMock(&argc, argv);

	return RUN_ALL_TESTS();
}*/