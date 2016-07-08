#ifndef __meshcollisoioncomponent_h__
#define __meshcollisoioncomponent_h__

#ifdef WIN32
#pragma once
#endif

#include "CollisionController.h"

class MeshCollisionComponent : public CollisionController
{
	DECLARE_CLASS_SIMPLE(MeshCollisionComponent, CollisionController);

public:
	MeshCollisionComponent(const CObjectAbstract *parent);
	~MeshCollisionComponent();

	virtual void	OnPropertyChanged(const char* PropertyName);

	virtual void	Initialize();

protected:
private:
};

#endif//__meshcollisioncomponent_h__