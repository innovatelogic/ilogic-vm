#ifndef __boxcollisioncontroller_h__
#define __boxcollisioncontroller_h__

#ifdef _WIN32
#pragma once
#endif

#include "CollisionController.h"

class BoxCollisionController : public CollisionController
{
	DECLARE_CLASS_SIMPLE(BoxCollisionController, CollisionController);

public:
 	BoxCollisionController(const CObjectAbstract * Parent);
 	virtual ~BoxCollisionController();

	virtual void		OnPropertyChanged(const char* PropertyName);

	virtual void		Initialize();

	// properties	
	virtual	Vector		GetExtends();
	virtual	void		SetExtends(Vector &Ext);

protected:
	virtual bool		CreatePhysActor();

private:
	Vector		Extends;
};

#endif//__boxcollisioncontroller_h__