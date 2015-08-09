#include "coresdkafx.h"

REGISTER_CLASS(ActorCollisionKinematicController, ActorCollisionController);

//----------------------------------------------------------------------------------------------
ActorCollisionKinematicController::ActorCollisionKinematicController(const CObjectAbstract * Parent)
: Super(Parent)
{

}

//----------------------------------------------------------------------------------------------
ActorCollisionKinematicController::ActorCollisionKinematicController(const ActorCollisionKinematicController & Source)
: Super(Source)
{
	if (this != &Source)
	{

	}
}

//----------------------------------------------------------------------------------------------
ActorCollisionKinematicController::~ActorCollisionKinematicController()
{

}

//----------------------------------------------------------------------------------------------
void ActorCollisionKinematicController::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);
}

//----------------------------------------------------------------------------------------------
void ActorCollisionKinematicController::Initialize()
{
	Super::Initialize();
}