#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
CContactSensorCallbackBt::CContactSensorCallbackBt(CPawnCueBall *pOwner)
: btCollisionWorld::ContactResultCallback()
, m_pOwner(pOwner)
{

}

//----------------------------------------------------------------------------------------------
bool CContactSensorCallbackBt::needsCollision(btBroadphaseProxy *proxy) const
{
	bool bResult = true; //body.checkCollideWithOverride(static_cast<btCollisionObject*>(proxy->m_clientObject));

	if (bResult){
		//
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
btScalar CContactSensorCallbackBt::addSingleResult(btManifoldPoint &cp, 
													const btCollisionObjectWrapper *colObj0, int parentId0, int index0,
													const btCollisionObjectWrapper *colObj1, int parentId1, int index1)
{
	m_pOwner->SetMoveDirection(m_pOwner->GetMoveDirection() == EMoveDirectionDown ?	EMoveDirectionUp : EMoveDirectionDown);
	return 0;
}