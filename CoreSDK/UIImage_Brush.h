#ifndef __uiimage_brush_h__
#define __uiimage_brush_h__

#ifdef _WIN32
#pragma once
#endif

#include "Brush_AbstractInterface.h"
#include "UIImage.h"

class UIImage_Brush : public UIImage, public Brush_AbstractInterface
{
	DECLARE_CLASS_SIMPLE(UIImage_Brush, UIImage);

public:
	UIImage_Brush(const CObjectAbstract * Parent);
	UIImage_Brush(const UIImage_Brush& Source);
	virtual ~UIImage_Brush();
	
	virtual bool		DoEventPressed(const MouseInputData& InputData);

protected:
private:
};

#endif//__uiimage_brush_h__