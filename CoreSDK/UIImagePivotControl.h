#pragma once

#include "UIImage.h"
#include "reflx.h"

class UIViewPivotControl;

class UIImagePivotControl : public UIImage
{
	DECLARE_CLASS_SIMPLE(UIImagePivotControl, UIImage);

public:
	UIImagePivotControl(const CObjectAbstract *parent);
	virtual ~UIImagePivotControl();

	virtual bool	DoEventPressed(const MouseInputData& InputData);
	virtual bool	DoEventReleased(const MouseInputData& InputData);

private:
	UIViewPivotControl * m_pPivot;
};