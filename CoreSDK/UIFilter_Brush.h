#ifndef __uifilter_brush_h__
#define __uifilter_brush_h__

#ifdef _WIN32
#pragma once
#endif

#include "Brush_AbstractInterface.h"
#include "UIFilter.h"

class UIFilter_Brush : public UIFilter, public Brush_AbstractInterface
{
	DECLARE_CLASS_SIMPLE(UIFilter_Brush, UIFilter);

public:
	UIFilter_Brush(const CObjectAbstract * Parent);
	UIFilter_Brush(const UIFilter_Brush& Source);
	virtual ~UIFilter_Brush();

protected:
private:
};

#endif//__uifilter_brush_h__