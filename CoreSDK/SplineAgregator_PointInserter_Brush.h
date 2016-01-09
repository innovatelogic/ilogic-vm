#ifndef __splineagregator_pointinserter_brush_h__
#define __splineagregator_pointinserter_brush_h__

#ifdef _WIN32
#pragma once
#endif

#include "Brush_AbstractInterface.h"
#include "UIScreenObject.h"

class SplineAgregator_PointInserter_Brush : public UIScreenObject, public Brush_AbstractInterface
{
	DECLARE_CLASS_SIMPLE(SplineAgregator_PointInserter_Brush, UIScreenObject);

public:
	SplineAgregator_PointInserter_Brush(const CObjectAbstract * Parent);
	virtual ~SplineAgregator_PointInserter_Brush();

	virtual void			Deserialize(tinyxml2::XMLElement *xml_current_tree) {;}

	virtual bool			DoEventPressed(const MouseInputData& InputData);

	virtual void			DoDraw();
	virtual void			DrawBounds() const;

protected:
private:
};

#endif//__splineagregator_pointinserter_brush_h__