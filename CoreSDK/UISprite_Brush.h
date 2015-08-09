#ifndef __uisprite_brush_h__
#define __uisprite_brush_h__

#ifdef _WIN32
#pragma once
#endif

#include "Brush_AbstractInterface.h"
#include "UISprite.h"

class UISprite_Brush : public UISprite, public Brush_AbstractInterface
{
	DECLARE_CLASS_SIMPLE(UISprite_Brush, UISprite);

public:
	UISprite_Brush(const CObjectAbstract * Parent);
	UISprite_Brush(const UISprite_Brush &Source);
	virtual ~UISprite_Brush();

	virtual bool		DoEventPressed(const MouseInputData& InputData);

protected:
private:
};


#endif//__uisprite_brush_h__