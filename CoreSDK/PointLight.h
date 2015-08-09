#ifndef __pointlight_h__
#define __pointlight_h__

#ifdef WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class CORESDK_API PointLight : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(PointLight, ActorAllocator);

public:
	PointLight(const CObjectAbstract * Parent);
	PointLight(const PointLight & Sender);
	virtual ~PointLight();

	virtual void			DoDraw();

	virtual void			SetColor(unsigned int color) { Color = color; }
	virtual unsigned int	GetColor() { return Color; }

	virtual Matrix	GetLightProjMartrix() const { return LightProjMatrix; }
	virtual Matrix	GetLightViewMatrix() const { return LightViewMatrix; }

protected:
private:
	float				AffectRadius;
	unsigned int		Color;
	mutable Matrix		LightProjMatrix;
	mutable Matrix		LightViewMatrix;
};

#endif//__pointlight_h__