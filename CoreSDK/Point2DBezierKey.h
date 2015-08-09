#ifndef __point2dbezierkey_h__
#define __point2dbezierkey_h__

#ifdef _WIN32
#pragma once
#endif

#include "Point2D.h"

class Point2DBezierKey : public Point2D
{
	DECLARE_CLASS_SIMPLE(Point2DBezierKey, Point2D);

public:
	Point2DBezierKey(const CObjectAbstract * Parent);
	Point2DBezierKey(const Point2DBezierKey& Source);
	virtual ~Point2DBezierKey();

	virtual void DoDraw();

	virtual bool DoEventReleased(const MouseInputData& InputData);

protected:
private:
};

#endif//__point2dbezierkey_h__