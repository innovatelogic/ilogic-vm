#include <iostream>
#include <gtest/gtest.h>

#include "vector.h"

//----------------------------------------------------------------------------------------------
TEST(TestMath, AddTwoVectors)
{
    oes::common_base::Vector one(1.5f, 2.5f, 3.5f);
    oes::common_base::Vector two(1.f, 1.f, 1.f);

    oes::common_base::Vector res = one + two;

	EXPECT_EQ(res.x, 2.5f);
	EXPECT_EQ(res.y, 3.5f);
	EXPECT_EQ(res.z, 4.5f);
}

//----------------------------------------------------------------------------------------------
TEST(TestMath, SubstractTwoVectors)
{
    oes::common_base::Vector one(1.5f, 2.5f, 3.5f);
    oes::common_base::Vector two(1.f, 1.f, 1.f);

    oes::common_base::Vector res = one - two;

	EXPECT_EQ(res.x, 0.5f);
	EXPECT_EQ(res.y, 1.5f);
	EXPECT_EQ(res.z, 2.5f);
}