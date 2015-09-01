#include <iostream>
#include <gtest/gtest.h>

#include "../vector.h"

//----------------------------------------------------------------------------------------------
TEST(MathTest, AddTwoVectors) 
{
	Vector one(1.5f, 2.5f, 3.5f);
	Vector two(1.f, 1.f, 1.f);

	Vector res = one + two;

	EXPECT_EQ(res.x, 1.5f);
	EXPECT_EQ(res.y, 3.5f);
	EXPECT_EQ(res.z, 4.5f);
}

//----------------------------------------------------------------------------------------------
TEST(MathTest, SubstractTwoVectors)
{
	Vector one(1.5f, 2.5f, 3.5f);
	Vector two(1.f, 1.f, 1.f);

	Vector res = one - two;

	EXPECT_EQ(res.x, 0.5f);
	EXPECT_EQ(res.y, 1.5f);
	EXPECT_EQ(res.z, 2.5f);
}