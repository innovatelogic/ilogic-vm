#include "../moc/moc_editor.cxx"
#include "../moc/moc_command_buffer.cxx"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::AtLeast;

//----------------------------------------------------------------------------------------------
TEST(EditorTest, TestUndoRedoCall)
{
    editors::MockCommandBuffer *buffer = new editors::MockCommandBuffer();

    EXPECT_CALL(*buffer, Undo()).Times(AtLeast(1));
    EXPECT_CALL(*buffer, Redo()).Times(AtLeast(1));

    editors::Editor editor;
    editor.SetCommandBuffer(buffer);

    EXPECT_TRUE(editor.Undo());
    EXPECT_TRUE(editor.Redo());
}

//----------------------------------------------------------------------------------------------
int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
	::testing::InitGoogleMock(&argc, argv);

	return RUN_ALL_TESTS();
}