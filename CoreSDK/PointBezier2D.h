#ifndef __pint2d_bezier_h__
#define __pint2d_bezier_h__

#ifdef _WIN32
#pragma once
#endif

#include "Point2D.h"

class PointBezier2D : public Point2D
{
	DECLARE_CLASS_SIMPLE(PointBezier2D, Point2D);

public:
	PointBezier2D(const CObjectAbstract * Parent);
	virtual ~PointBezier2D();

	virtual void	DoDraw();
	virtual void	DrawBounds() const {}

	virtual Vector	GetPositionKey0();
	virtual Vector	GetPositionKey1();

	virtual Matrix	GetLTMKey0();
	virtual Matrix	GetLTMKey1();

	virtual bool	KeyUpdated(const CActor * Sender, ESystemEventID EventId);

	virtual const Point2D* GetPoint1() { return Point1; }
	virtual const Point2D* GetPoint2() { return Point2; }

protected:
private:
	class Point2D* Point1;
	class Point2D* Point2;
};

#endif//__pint2d_bezier_h__