#include "ut_treemap_fixture.h"

TEST_F(TestMapTest, ClearMapCheckSize)
{
    m_map.Clear();

    EXPECT_EQ(m_map.begin(), nullptr);
    EXPECT_EQ(m_map.end(), nullptr);
}