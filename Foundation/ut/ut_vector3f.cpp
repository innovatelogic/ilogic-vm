#include <iostream>
#include <gtest/gtest.h>

#include "../vector.h"

//----------------------------------------------------------------------------------------------
TEST(MathTest, AddTwoVectors) 
{
	Vector one(1.f, 1.f, 1.f);
	Vector two(2.f, 2.f, 2.f);

	Vector res = one + two;

	EXPECT_EQ(res.x, 3.f);
	EXPECT_EQ(res.y, 3.f);
	EXPECT_EQ(res.z, 3.f);
}

//----------------------------------------------------------------------------------------------
int main(int argc, char **argv) 
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}