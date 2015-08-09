#ifndef __uifilter_h__
#define __uifilter_h__

#ifdef _WIN32
#pragma once
#endif

#include "UIScreenObject.h"

class UIFilter : public UIScreenObject
{
	DECLARE_CLASS_SIMPLE(UIFilter, UIScreenObject);

public:
	UIFilter(const CObjectAbstract * Parent);
	UIFilter(const UIFilter& Source);
	virtual ~UIFilter();

	virtual bool  HitTest(const Vector2f& Position) { return false; }
};

#endif//__uifilter_h__
