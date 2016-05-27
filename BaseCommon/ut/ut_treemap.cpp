#include "ut_treemap_fixture.h"
#include <list>

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
        size_t i = 0;
        do
        {
            EXPECT_EQ(node->key(), m_deepth_first_traverse[i++]);

            node = m_map.next(node);
        } while (node);

       EXPECT_EQ(i, m_deepth_first_traverse.size());
    }
}

//----------------------------------------------------------------------------------------------
TEST_F(TestMapTest, BreathFirstTraverseTest)
{
    if (TestTreeNode *node = m_map.begin())
    {
        std::list<TestTreeNode*> stackx;

        while (node) // fill top elements first
        {
            stackx.push_back(node);
            node = node->GetPlainNext();
        }

        int i = 0;
        while (!stackx.empty())
        {
            TestTreeNode *top = stackx.front();

            EXPECT_EQ(top->key(), m_breath_traverse[i++]);

            stackx.pop_front();

            if (top->GetNumChilds()) // append each childs to back
            {
                TestTreeNode *child = top->GetNext();

                assert(child);

                while (child)
                {
                    stackx.push_back(child);
                    child = child->GetPlainNext();
                }
            }
        }

        EXPECT_EQ(i, m_breath_traverse.size());
    }
}