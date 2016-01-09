#ifndef __splineagregator_brush_h__
#define __splineagregator_brush_h__

#ifdef _WIN32
#pragma once
#endif

#include "Brush_AbstractInterface.h"
#include "SplineAgregator.h"

class SplineAgregator_Brush : public SplineAgregator, public Brush_AbstractInterface
{
	DECLARE_CLASS_SIMPLE(SplineAgregator_Brush, SplineAgregator);

public:
	SplineAgregator_Brush(const CObjectAbstract * Parent);
	virtual ~SplineAgregator_Brush();

	virtual void			Deserialize(tinyxml2::XMLElement *xml_current_tree) {;}

	virtual bool			DoEventPressed(const MouseInputData& InputData);

	virtual void			DoDraw();

protected:
	virtual bool			GenerateKey(const Vector2f& MousePos);
	virtual bool			GenerateObjectSpline();
	virtual void			ReleaseGeneratedChilds();

protected:
private:
	TVecIndexedPoint2f		PreRenderSpline;
};

#endif//__splineagregator_brush_h__