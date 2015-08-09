#ifndef __restrictedvolumecollisioncontroller_h__
#define __restrictedvolumecollisioncontroller_h__

#ifdef _WIN32
#pragma once
#endif

#include "CollisionController.h"

class CRestrictedVolumeCollisionController : public CollisionController
{
	DECLARE_CLASS_SIMPLE(CRestrictedVolumeCollisionController, CollisionController);

public:
	CRestrictedVolumeCollisionController(const class CObjectAbstract *pParent);
	virtual ~CRestrictedVolumeCollisionController();

	virtual void	OnPropertyChanged(const char *pPropertyName);
	virtual void	Initialize();

	virtual bool	CreatePhysActor(Matrix &matrix, Vector &Ext, bool bSides[6]);
	virtual void	ReleasePhysActor();
};

#endif//__restrictedvolumecollisioncontroller_h__