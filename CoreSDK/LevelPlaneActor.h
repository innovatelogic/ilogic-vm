#ifndef __levelplaneactor_h__
#define __levelplaneactor_h__

#ifdef WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class CORESDK_API CLevelPlaneActor : public ActorAllocator, IDrawInterface
{
	DECLARE_CLASS_SIMPLE(CLevelPlaneActor, ActorAllocator);

public:
	CLevelPlaneActor(const CObjectAbstract *pParent);
	CLevelPlaneActor(const CLevelPlaneActor &Source);
	virtual ~CLevelPlaneActor();

	virtual void	OnPropertyChanged(const char *pPropertyName);
	
	virtual void	Initialize();
	virtual void	DoDraw();

protected:
	bool	InitPhysPlane();

	bool	LoadMesh(const char *url);
	void	Clear();

private:
	class Comp_StaticMesh			*m_pMeshComponent;
	class CPlaneCollisionController	*m_pCollisionObject;

	std::string						m_XRef;
};

#endif//__levelplaneactor_h__