#ifndef __beziersplineagregator_brush__
#define __beziersplineagregator_brush__

#ifdef _WIN32
#pragma once
#endif

#include "Brush_AbstractInterface.h"
#include "Bezier_SplineAgregator.h"

class Bezier_SplineAgregator_Brush : public Bezier_SplineAgregator, public Brush_AbstractInterface
{
	DECLARE_CLASS_SIMPLE(Bezier_SplineAgregator_Brush, Bezier_SplineAgregator);

public:
	Bezier_SplineAgregator_Brush(const CObjectAbstract * Parent);
	virtual ~Bezier_SplineAgregator_Brush();
	
	virtual void	Deserialize(tinyxml2::XMLElement *xml_current_tree) {;}

	virtual bool	DoEventPressed(const MouseInputData& InputData);
	virtual bool	DoEventMove(const MouseMoveInputData& InputData);

	virtual void	DoDraw();

	virtual bool	GenerateKey(const Vector2f& MousePos, const Matrix &WTM);
	virtual bool	GenerateObjectSpline();
	virtual void	ReleaseGeneratedChilds();

	/** build closed spline */
	virtual void	BuildSplineList(TVecIndexedPoint2f& OutSpline);
protected:
private:
	bool					bPointEdit;
	TVecIndexedPoint2f		PreRenderSpline;
};

#endif//__beziersplineagregator_brush__