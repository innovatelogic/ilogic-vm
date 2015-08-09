#include "coresdkafx.h"

REGISTER_CLASS(CPlaneCollisionController, CollisionController)

//------------------------------------------------------------------------
CPlaneCollisionController::CPlaneCollisionController(const CObjectAbstract *pParent)
: Super(pParent)
{
}

//------------------------------------------------------------------------
CPlaneCollisionController::~CPlaneCollisionController()
{
}

//------------------------------------------------------------------------
void CPlaneCollisionController::OnPropertyChanged(const char *pPropertyName)
{	
	Super::OnPropertyChanged(pPropertyName);
}

//------------------------------------------------------------------------
void CPlaneCollisionController::Initialize()
{	
	Super::Initialize();
}

//------------------------------------------------------------------------
bool CPlaneCollisionController::CreatePhysActor(Matrix &matrix)
{
	return GetPhysicsEngine()->CreatePlane(this, &matrix);
}

//------------------------------------------------------------------------
void CPlaneCollisionController::ReleasePhysActor()
{

}