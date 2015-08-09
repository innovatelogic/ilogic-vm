#ifndef __planecollisioncontroller_h__
#define __planecollisioncontroller_h__

#ifdef _WIN32
#pragma once
#endif

#include "CollisionController.h"

class CPlaneCollisionController : public CollisionController
{
	DECLARE_CLASS_SIMPLE(CPlaneCollisionController, CollisionController);

public:
	CPlaneCollisionController(const class CObjectAbstract *pParent);
	virtual ~CPlaneCollisionController();

	virtual void	OnPropertyChanged(const char *PropertyName);
	virtual void	Initialize();

	virtual bool	CreatePhysActor(Matrix &matrix);
	virtual void	ReleasePhysActor();
};

#endif//__planecollisioncontroller_h__