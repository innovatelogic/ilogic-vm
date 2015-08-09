#ifndef __contactsensorcallbackbt_h__
#define __contacksensorcallbackbt_h__

#ifdef _WIN32
#pragma once
#endif

#include <btBulletDynamicsCommon.h>

class CContactSensorCallbackBt : public btCollisionWorld::ContactResultCallback
{
public:
	CContactSensorCallbackBt(class CPawnCueBall *pOwner);

	virtual bool needsCollision(btBroadphaseProxy *proxy) const;

	virtual btScalar addSingleResult(btManifoldPoint &cp, 
										const btCollisionObjectWrapper *colObj0, int parentId0, int index0,
										const btCollisionObjectWrapper *colObj1, int parentId1, int index1);
protected:
private:
	class CPawnCueBall	*m_pOwner;
};

#endif//__contactsensorcallback_h__