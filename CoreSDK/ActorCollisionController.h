#ifndef __actorcollisioncontroller_h__
#define __actorcollisioncontroller_h__

#ifdef WIN32
#pragma once
#endif

#include "CollisionController.h"

class ActorCollisionController : public CollisionController
{
	DECLARE_CLASS_SIMPLE(ActorCollisionController, CollisionController);
public:
	ActorCollisionController(const CObjectAbstract *pParent);
	ActorCollisionController(const ActorCollisionController &Source);
	virtual ~ActorCollisionController();

	virtual void			OnPropertyChanged(const char *pPropertyName);

	virtual void			Initialize();

	virtual bool			Load(const char *filename);

protected:
	virtual bool			CreatePhysActor();

private:
	std::string				m_XRef;
};

#endif//__actorcollisioncontroller_h__
