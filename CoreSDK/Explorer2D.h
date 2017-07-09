#pragma once

#include "UIScreenObject.h"
#include "ViewportInterface.h"

class CORESDK_API Explorer2D : public UIScreenObject, public core_sdk_api::TIViewport
{
	DECLARE_CLASS_SIMPLE(Explorer2D, UIScreenObject);

public:
	// Constructor / Destructor
	Explorer2D(const CObjectAbstract * Parent);
	virtual ~Explorer2D();

	virtual void		Initialize();
	virtual void		InitializeBrushes();

	virtual void		DoDraw();

	virtual	Vector2f	GetTransformedSize_() const;

	inline_ virtual bool	HitTest(const Vector2f& Position) { return false; }

protected:
private:
};