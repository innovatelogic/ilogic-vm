#ifndef __bezier_splineagregator_h__
#define __bezier_splineagregator_h__

#ifdef _WIN32
#pragma once
#endif

#include "SplineAgregator.h"

class Bezier_SplineAgregator : public SplineAgregator
{
	DECLARE_CLASS_SIMPLE(Bezier_SplineAgregator, SplineAgregator);
public:
	/**constructor destructor*/
	Bezier_SplineAgregator(const CObjectAbstract * Parent);
	Bezier_SplineAgregator(const Bezier_SplineAgregator& Source);
	virtual ~Bezier_SplineAgregator();

	virtual void				BuildSplineList(TVecIndexedPoint2f& OutSpline);
	virtual bool				IsChildFocused();
	virtual bool				InsertPoint(const Vector2f& MousePos, const UIScreenObject* Inserter);

protected:
private:
};

#endif//__bezier_splineagregator_h__