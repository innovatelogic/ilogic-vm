#ifndef __animationcontroller2d_h__
#define __animationcontroller2d_h__

#ifdef _WIN32
#pragma once
#endif

#include "BaseModificator.h"

class AnimationController2D : public BaseModificator
{
	DECLARE_CLASS_SIMPLE(AnimationController2D, BaseModificator);

public:
	AnimationController2D(const CObjectAbstract * Parent);
	AnimationController2D(const AnimationController2D& Source);
	virtual ~AnimationController2D();

	//virtual bool AddChildNode(const CActor * actor, int Index = INDEX_NONE);

	virtual bool SetAnimationSet(size_t Track, const std::string& TrackName);

protected:
private:
	std::vector<class AnimationSequerence*> Sequerences;
};

#endif //__animationcontroller_h__