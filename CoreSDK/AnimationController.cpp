#include "coresdkafx.h"

REGISTER_CLASS(AnimationController, ActorAllocator);

//----------------------------------------------------------------------------------
AnimationController::AnimationController(const CObjectAbstract * Parent /*= NULL*/)
: Super(Parent)
{
	;//
}
//----------------------------------------------------------------------------------
AnimationController::AnimationController(const AnimationController& Source)
: Super(Source)
{
	if (this != &Source)
	{
		;//
	}
}
//----------------------------------------------------------------------------------
AnimationController::~AnimationController()
{
	;//
}

//----------------------------------------------------------------------------------
bool AnimationController::SetAnimationSet(size_t Track, const std::string& AnimName)
{
	return true;
}