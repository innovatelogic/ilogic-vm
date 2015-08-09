#ifndef __actorcollisionkinematiccontroller_h__
#define __actorcollisionkinematiccontroller_h__

#ifdef WIN32
#pragma once
#endif

#include "ActorCollisionController.h"

class ActorCollisionKinematicController : public ActorCollisionController
{
	DECLARE_CLASS_SIMPLE(ActorCollisionKinematicController, ActorCollisionController);

public:
	ActorCollisionKinematicController(const CObjectAbstract *pParent);
	ActorCollisionKinematicController(const ActorCollisionKinematicController &Source);
	~ActorCollisionKinematicController();

	virtual void		OnPropertyChanged(const char *PropertyName);
	virtual void		Initialize();

protected:
private:
};

#endif//__actorcollisionkinematiccontroller_h__