#ifndef __uiprogressbar_h__
#define __uiprogressbar_h__

#ifdef _WIN32
#pragma once
#endif

#include "UIImage.h"

class CORESDK_API UIProgressBar : public UIImage
{
	DECLARE_CLASS_SIMPLE(UIProgressBar, UIImage);

public:
	UIProgressBar(const CObjectAbstract *Parent);
	UIProgressBar(const UIProgressBar &Source);
	virtual ~UIProgressBar();

	virtual void		OnPropertyChanged(const char* PropertyName);

	virtual void		DoDraw();

	virtual void		SetValue(float Value) { ValueProgress = Value; }
	virtual float		GetValue() const { return ValueProgress; }

	virtual void		SetValueMin(float Value) { ValueMin = Value; }
	virtual float		GetValueMin() const { return ValueMin; }

	virtual void		SetValueMax(float Value) { ValueMax = Value; }
	virtual float		GetValueMax() const { return ValueMax; }

protected:
	class UIComp_DrawImage	*FillImageComponent; // transient

	float	ValueProgress;
	float	ValueMin;
	float	ValueMax;
};

#endif//__uiprogressbar_h__