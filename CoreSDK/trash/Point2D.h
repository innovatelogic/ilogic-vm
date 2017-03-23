#ifndef __point_2d__
#define __point_2d__

#ifdef _WIN32
#pragma once
#endif

#include "UIScreenObject.h"

class Point2D : public UIScreenObject
{
	DECLARE_CLASS_SIMPLE(Point2D, UIScreenObject);

public:
	Point2D(const CObjectAbstract * Parent);
	virtual ~Point2D();

	virtual bool ProcessUserController(const MouseMoveInputData& InputData);
	virtual bool HitTest(const Vector2f &Position);

	virtual void DoDraw();
	virtual void DrawBounds() const {}

	virtual void PreRelease();
	virtual bool DoEventReleased(const MouseInputData& InputData);

protected:
private:
};

#endif//__point_2d__