#ifndef __splinemovementcontroller_h__
#define __splinemovementcontroller_h__

#ifdef _WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class SplineMovementController : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(SplineMovementController, ActorAllocator);

public:
	SplineMovementController(const CObjectAbstract * Parent);
	virtual ~SplineMovementController();

protected:
private:
};

#endif//__splinemovementcontroller_h__