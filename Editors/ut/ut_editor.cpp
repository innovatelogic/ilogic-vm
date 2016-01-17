#include "../moc/moc_editor.cxx"
#include "../moc/moc_command_buffer.cxx"
#include "../moc/moc_command_base.cxx"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::AtLeast;

//----------------------------------------------------------------------------------------------
TEST(EditorTest, TestUndoRedoCall)
{
    editors::Editor editor;

    EXPECT_CALL(*reinterpret_cast<editors::MockCommandBuffer*>(editor.GetCommandBuffer()), Undo()).Times(AtLeast(1));
    EXPECT_CALL(*reinterpret_cast<editors::MockCommandBuffer*>(editor.GetCommandBuffer()), Redo()).Times(AtLeast(1));

    EXPECT_TRUE(editor.Undo());
    EXPECT_TRUE(editor.Redo());
}

//----------------------------------------------------------------------------------------------
TEST(EditorTest, TestCommandAdd)
{
    editors::Editor editor;



}


//----------------------------------------------------------------------------------------------
int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
	::testing::InitGoogleMock(&argc, argv);

	return RUN_ALL_TESTS();
}