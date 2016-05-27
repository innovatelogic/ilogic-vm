#include "ut_treemap_fixture.h"

//----------------------------------------------------------------------------------------------
TEST_F(TestMapTest, ClearMapCheckSize)
{
    m_map.Clear();

    EXPECT_EQ(m_map.begin(), nullptr);
    EXPECT_EQ(m_map.end(), nullptr);
}

//----------------------------------------------------------------------------------------------
TEST_F(TestMapTest, DeepthFirstTraverseTest)
{
    if (TestTreeNode *node = m_map.begin())
    {
        int k = 0;
        do
        {
            EXPECT_EQ(node->key(), m_deepth_first_traverse[k++]);

            node = m_map.next(node);
        } while (node);
    }
}

//----------------------------------------------------------------------------------------------
TEST_F(TestMapTest, BreathFirstTraverseTest)
{

}