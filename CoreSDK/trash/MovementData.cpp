#include "MovementData.h"
#include <vector>
/*

std::vector<float> DEBUG_DATA; 
//------------------------------------------------------------------------
MovementData::MovementData()
: bJump(false)
, fV0(0.f)
, fJumpTime(0.f)
, vDisplace(0.f, 0.f, 0.f)
, CharacterSpeed(5.f)
, Height(0.f)
{
	;//
}

//------------------------------------------------------------------------
void MovementData::StartJump(float v0)
{
	if (bJump) { return; }
	fJumpTime = 0.f;
	fV0 = v0;
	bJump = true;
}
//------------------------------------------------------------------------
void MovementData::StopJump()
{
	if (!bJump) { return; }
	bJump = false;
}
//------------------------------------------------------------------------
float MovementData::GetHeight(float elapsedTime, const NxVec3 & Gravity)
{
	
	//a(t) = cte = g
	//v(t) = g*t + v0
	//y(t) = g*t^2 + v0*t + y0
	
	float  G = -98.1f;//-50.0f; 
	if (!bJump) { return 0.f; }

 	fJumpTime += elapsedTime;
	float h = G * fJumpTime * fJumpTime + fV0 * fJumpTime;
	Height = (h - Gravity.y) * elapsedTime;

	//DEBUG_DATA.push_back(Height);

	return Height;
}*/