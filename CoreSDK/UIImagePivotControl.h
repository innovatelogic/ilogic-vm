#ifndef __uiimagepivotcontrol_h__
#define __uiimagepivotcontrol_h__

#ifdef WIN32
#pragma once
#endif

#include "UIImage.h"

class UIImagePivotControl : public UIImage
{
	DECLARE_CLASS_SIMPLE(UIImagePivotControl, UIImage);

public:
	UIImagePivotControl(const CObjectAbstract * Parent);
	UIImagePivotControl(const UIImagePivotControl & Source);
	virtual ~UIImagePivotControl();

	virtual bool	DoEventPressed(const MouseInputData& InputData);
	virtual bool	DoEventReleased(const MouseInputData& InputData);

private:
	class UIViewPivotControl * m_pPivot;
};

#endif//__uiimagepivotcontrol_h__