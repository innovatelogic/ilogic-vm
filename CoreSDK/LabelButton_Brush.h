#ifndef __labelbutton_brush_h__
#define __labelbutton_brush_h__

#ifndef WIN32
#pragma once
#endif

#include "Brush_AbstractInterface.h"
#include "LabelButton.h"

class LabelButton_Brush : public LabelButton, public Brush_AbstractInterface
{
	DECLARE_CLASS_SIMPLE(LabelButton_Brush, LabelButton);
public:
	LabelButton_Brush(const CObjectAbstract * Parent);
	~LabelButton_Brush();

	virtual bool		DoEventPressed(const MouseInputData& InputData);

protected:
private:
};

#endif//__labelbutton_brush_h__