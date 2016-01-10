#include "coresdkafx.h"

REGISTER_CLASS(AnimationController2D, BaseModificator);

//----------------------------------------------------------------------------------
AnimationController2D::AnimationController2D(const CObjectAbstract * Parent /*= NULL*/)
: Super(Parent)
{
	;//
}
//----------------------------------------------------------------------------------
AnimationController2D::AnimationController2D(const AnimationController2D& Source)
: Super(Source)
{
	if (this != &Source)
	{
		;//
	}
}
//----------------------------------------------------------------------------------
AnimationController2D::~AnimationController2D()
{
	;//
}

//----------------------------------------------------------------------------------
bool AnimationController2D::SetAnimationSet(size_t Track, const std::string& TrackName)
{
	std::vector<AnimationSequerence*>::iterator IterFind =
		std::find_if(Sequerences.begin(), Sequerences.end(), boost::bind<bool>(&CObjectAbstract::IsNameEqual, _1, TrackName.c_str()));

	if (IterFind != Sequerences.end()){
		//TODO
	}
	return true;
}