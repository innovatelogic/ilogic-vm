#include "../moc_editor.cxx"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::AtLeast;

TEST(EditorTest, TestStart)
{
    editors::MockEditor editor;
    EXPECT_CALL(editor, Undo()).Times(AtLeast(1));
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}