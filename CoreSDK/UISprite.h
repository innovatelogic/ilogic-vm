#ifndef __uisprite_h__
#define __uisprite_h__

#ifdef _WIN32
#pragma once
#endif

#include "UIImage.h"

class UISprite : public UIImage
{
	DECLARE_CLASS_SIMPLE(UISprite, UIImage);

public:
	UISprite(const CObjectAbstract * Parent);
	UISprite(const UISprite &Source);
	virtual ~UISprite();

	virtual bool		HitTest(const Vector2f &Position);
	
	virtual void		DoDraw();
	virtual void		DrawBounds() const;

protected:
private:
};

#endif//__uisprite_h__