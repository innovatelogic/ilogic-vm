#ifndef __modificatorcollision2d_h__
#define __modificatorcollision2d_h__

#ifdef _WIN32
#pragma once
#endif

#include "BaseModificator.h"

class SplineCollision2DModificator : public BaseModificator
{
	DECLARE_CLASS_SIMPLE(SplineCollision2DModificator, BaseModificator);

public:
	SplineCollision2DModificator(const CObjectAbstract * Parent);
	SplineCollision2DModificator(const SplineCollision2DModificator &Source);
	virtual ~SplineCollision2DModificator();

	//virtual void Serialize(std::ofstream& Stream);
	virtual void Initialize();

	virtual void ApplyModificator(const CActor * Sender);
	virtual bool Apply(const CActor * Sender, ESystemEventID EventId);

protected:
};

#endif//__basemodificator_h__