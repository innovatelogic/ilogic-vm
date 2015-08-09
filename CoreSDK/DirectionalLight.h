#ifndef __directionallight_h__
#define __directionallight_h__

#ifdef WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class CORESDK_API DirectionalLight : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(DirectionalLight, ActorAllocator);

public:
	DirectionalLight(const class CObjectAbstract *pParent);
	DirectionalLight(const DirectionalLight &Source);
	~DirectionalLight();

protected:
private:
};


#endif//__directionallight_h__