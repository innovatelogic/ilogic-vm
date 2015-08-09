#ifndef __animationcontroller_h__
#define __animationcontroller_h__

#ifdef _WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class AnimationController : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(AnimationController, ActorAllocator);

public:
	AnimationController(const CObjectAbstract * Parent);
	AnimationController(const AnimationController& Source);
	virtual ~AnimationController();

	virtual bool SetAnimationSet(size_t Track, const std::string& AnimName);
protected:
private:
};

#endif //__animationcontroller_h__