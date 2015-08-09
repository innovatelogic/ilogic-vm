#include "coresdkafx.h"

REGISTER_CLASS(CRestrictedVolumeCollisionController, CollisionController)

//------------------------------------------------------------------------
CRestrictedVolumeCollisionController::CRestrictedVolumeCollisionController(const CObjectAbstract *pParent)
: Super(pParent)
{
}

//------------------------------------------------------------------------
CRestrictedVolumeCollisionController::~CRestrictedVolumeCollisionController()
{
}

//------------------------------------------------------------------------
void CRestrictedVolumeCollisionController::OnPropertyChanged(const char *pPropertyName)
{	
	Super::OnPropertyChanged(pPropertyName);
}

//------------------------------------------------------------------------
void CRestrictedVolumeCollisionController::Initialize()
{	
	Super::Initialize();
}

//------------------------------------------------------------------------
bool CRestrictedVolumeCollisionController::CreatePhysActor(Matrix &matrix, Vector &Ext, bool bSides[6])
{
	m_pPhysNode = GetPhysicsEngine()->CreateRestrictedVolume(this, Ext, &matrix, bSides);
	return true;
}

//------------------------------------------------------------------------
void CRestrictedVolumeCollisionController::ReleasePhysActor()
{
	GetPhysicsEngine()->ReleaseObject(m_pPhysNode);
	m_pPhysNode = NULL;
}