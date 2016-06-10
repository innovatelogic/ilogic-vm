#ifndef __explorer3d_h__
#define __explorer3d_h__

#ifdef _WIN32
#pragma once
#endif

#include "ActorAllocator.h"
#include "ViewportInterface.h"

class CORESDK_API Explorer3D : public ActorAllocator, 
	public core_sdk_api::TIViewport,
	public IDrawInterface
{
	DECLARE_CLASS_SIMPLE(Explorer3D, ActorAllocator);

public:
	//Constructor/Destructor
	Explorer3D(const CObjectAbstract *pParent);
	virtual ~Explorer3D();

	virtual void	InitializeBrushes();
	virtual void	DoDraw();

	virtual bool	DoVisibilityTest_();

protected:
private:
	virtual void	DrawGrid();
	virtual void	DrawShadowMaps();

private:
	std::vector<class PointLight*> VLights;
};

#endif//__explorer3d_h__