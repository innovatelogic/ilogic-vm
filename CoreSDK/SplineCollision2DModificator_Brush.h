#ifndef __splinecollision2dmodificator_brush_h__
#define __splinecollision2dmodificator_brush_h__

#ifdef _WIN32
#pragma once
#endif

#include "Brush_AbstractInterface.h"
#include "SplineCollision2DModificator.h"

class SplineCollision2DModificator_Brush : public SplineCollision2DModificator, public Brush_AbstractInterface
{
	DECLARE_CLASS_SIMPLE(SplineCollision2DModificator_Brush, SplineCollision2DModificator);

public:
	SplineCollision2DModificator_Brush(const CObjectAbstract * Parent);
	SplineCollision2DModificator_Brush(const SplineCollision2DModificator_Brush& Source);
	virtual ~SplineCollision2DModificator_Brush();

protected:
private:
};

#endif//__splinecollision2dmodificator_brush_h__