#include "../selection_container.h"
#include <vector>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::AtLeast;
using ::testing::Return;

//----------------------------------------------------------------------------------------------
TEST(SelectionContainer, AddRemoveCheckSize)
{
    int keys[2] = { 1, 2 };
    int values1[3] = { 11, 12, 13 };
    int values2[2] = { 21, 22 };

    oes::editors::SelectionContainer<int> container;

    EXPECT_EQ(container.Size(), 0);

    // first queue
    EXPECT_TRUE(container.AddItem(&keys[0], &values1[0]));
    EXPECT_TRUE(container.AddItem(&keys[0], &values1[1]));
    EXPECT_TRUE(container.AddItem(&keys[0], &values1[2]));
    
    EXPECT_EQ(container.Size(), 3);

    // add second queue
    EXPECT_TRUE(container.AddItem(&keys[1], &keys[1]));
    EXPECT_TRUE(container.AddItem(&keys[1], &values2[0]));
    EXPECT_TRUE(container.AddItem(&keys[1], &values2[1]));

    EXPECT_EQ(container.Size(), 6);

    // remove step-by-step
    EXPECT_TRUE(container.RemoveItem(&keys[0], &values1[0]));
    EXPECT_EQ(container.Size(), 5);
    EXPECT_EQ(container.Keys().size(), 2);

    EXPECT_TRUE(container.RemoveItem(&keys[0], &values1[1]));
    EXPECT_EQ(container.Size(), 4);
    EXPECT_EQ(container.Keys().size(), 2);

    EXPECT_TRUE(container.RemoveItem(&keys[0], &values1[2]));
    EXPECT_EQ(container.Size(), 3);
    EXPECT_EQ(container.Keys().size(), 1);

    EXPECT_TRUE(container.RemoveItem(&keys[1]));
    EXPECT_EQ(container.Size(), 0);
    EXPECT_EQ(container.Keys().size(), 0);
}

//----------------------------------------------------------------------------------------------
TEST(SelectionContainer, AddRemoveTwiceCorrectness)
{
    int keys[2] = { 1, 2 };
    int values1[2] = { 11, 12 };
    int values2[2] = { 21, 22 };

    oes::editors::SelectionContainer<int> container;

    // first queue
    EXPECT_TRUE(container.AddItem(&keys[0], &keys[0]));
    EXPECT_TRUE(container.AddItem(&keys[0], &values1[0]));
    EXPECT_TRUE(container.AddItem(&keys[0], &values1[1]));
    
    EXPECT_FALSE(container.AddItem(&keys[0], &keys[0]));
    EXPECT_FALSE(container.AddItem(&keys[0], &values1[0]));
    EXPECT_FALSE(container.AddItem(&keys[0], &values1[1]));

    EXPECT_EQ(container.Size(), 3);
    EXPECT_EQ(container.Keys().size(), 1);

    // second queue
    EXPECT_TRUE(container.AddItem(&keys[1], &keys[1]));
    EXPECT_TRUE(container.AddItem(&keys[1], &values2[0]));
    EXPECT_TRUE(container.AddItem(&keys[1], &values2[1]));

    EXPECT_FALSE(container.AddItem(&keys[1], &keys[1]));
    EXPECT_FALSE(container.AddItem(&keys[1], &values2[0]));
    EXPECT_FALSE(container.AddItem(&keys[1], &values2[1]));

    EXPECT_EQ(container.Size(), 6);
    EXPECT_EQ(container.Keys().size(), 2);

    // remove first queue
    EXPECT_TRUE(container.RemoveItem(&keys[0], &keys[0]));
    EXPECT_TRUE(container.RemoveItem(&keys[0], &values1[0]));
    EXPECT_TRUE(container.RemoveItem(&keys[0]));

    EXPECT_FALSE(container.RemoveItem(&keys[0], &keys[0]));
    EXPECT_FALSE(container.RemoveItem(&keys[0], &values1[0]));
    EXPECT_FALSE(container.RemoveItem(&keys[0], &values1[1]));

    EXPECT_EQ(container.Size(), 3);
    EXPECT_EQ(container.Keys().size(), 1);

    // remove second queue
    EXPECT_TRUE(container.RemoveItem(&keys[1], &keys[1]));
    EXPECT_TRUE(container.RemoveItem(&keys[1], &values2[0]));
    EXPECT_TRUE(container.RemoveItem(&keys[1], &values2[1]));

    EXPECT_FALSE(container.RemoveItem(&keys[1], &keys[1]));
    EXPECT_FALSE(container.RemoveItem(&keys[1], &values2[0]));
    EXPECT_FALSE(container.RemoveItem(&keys[1], &values2[1]));

    EXPECT_EQ(container.Size(), 0);
    EXPECT_EQ(container.Keys().size(), 0);
}

//----------------------------------------------------------------------------------------------
TEST(SelectionContainer, FetchKeys)
{
    int keys[2] = { 1, 2 };
    int values1[2] = { 11, 12 };
    int values2[2] = { 21, 22 };

    oes::editors::SelectionContainer<int> container;
    
    // first queue
    EXPECT_TRUE(container.AddItem(&keys[0], &keys[0]));
    EXPECT_TRUE(container.AddItem(&keys[0], &values1[0]));
    EXPECT_TRUE(container.AddItem(&keys[0], &values1[1]));

    // second queue
    EXPECT_TRUE(container.AddItem(&keys[1], &keys[1]));
    EXPECT_TRUE(container.AddItem(&keys[1], &values2[0]));
    EXPECT_TRUE(container.AddItem(&keys[1], &values2[1]));

    std::vector<const int*> vkeys = container.Keys();
    
    EXPECT_EQ(vkeys.size(), 2);
    EXPECT_EQ(*vkeys[0], 1);
    EXPECT_EQ(*vkeys[1], 2); 
}

//----------------------------------------------------------------------------------------------
TEST(SelectionContainer, FetchValues)
{
    int keys[2] = { 1, 2 };
    int values1[2] = { 11, 12 };
    int values2[2] = { 21, 22 };

    oes::editors::SelectionContainer<int> container;
    
    // first queue
    EXPECT_TRUE(container.AddItem(&keys[0], &keys[0]));
    EXPECT_TRUE(container.AddItem(&keys[0], &values1[0]));
    EXPECT_TRUE(container.AddItem(&keys[0], &values1[1]));

    // second queue
    EXPECT_TRUE(container.AddItem(&keys[1], &keys[1]));
    EXPECT_TRUE(container.AddItem(&keys[1], &values2[0]));
    EXPECT_TRUE(container.AddItem(&keys[1], &values2[1]));

    std::vector<const int*> values;
    
    container.GetAllElements(values);

    EXPECT_EQ(values.size(), 6);
    EXPECT_EQ(*values[0], 1);
    EXPECT_EQ(*values[1], 11);
    EXPECT_EQ(*values[2], 12);
    EXPECT_EQ(*values[3], 2);
    EXPECT_EQ(*values[4], 21);
    EXPECT_EQ(*values[5], 22);
}

//----------------------------------------------------------------------------------------------
TEST(SelectionContainer, EmptyValues)
{
    int keys[2] = { 1, 2 };
    int values1[2] = { 11, 12 };
    int values2[2] = { 21, 22 };

    oes::editors::SelectionContainer<int> container;

    // first queue
    EXPECT_TRUE(container.AddItem(&keys[0], &keys[0]));
    EXPECT_TRUE(container.AddItem(&keys[0], &values1[0]));
    EXPECT_TRUE(container.AddItem(&keys[0], &values1[1]));

    // second queue
    EXPECT_TRUE(container.AddItem(&keys[1], &keys[1]));
    EXPECT_TRUE(container.AddItem(&keys[1], &values2[0]));
    EXPECT_TRUE(container.AddItem(&keys[1], &values2[1]));

    container.Empty();

    EXPECT_EQ(container.Size(), 0);
    EXPECT_EQ(container.Keys().size(), 0);
}