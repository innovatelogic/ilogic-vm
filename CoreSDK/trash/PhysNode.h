#ifndef __physnode_h__
#define __physnode_h__

#ifdef _WIN32
#pragma once
#endif

struct SPhysNode
{
	SPhysNode(class CActor *pAct, class btRigidBody *pBody, class CContactSensorCallbackBt *pcallback = 0)
		: pActor(pAct)
		, pRigidBody(pBody)
		, pCallback(pcallback)
	{
	}

	inline_ CActor*			GetActor() const { return pActor; }
	inline_ btRigidBody*	GetRigidBody() const { return pRigidBody; }
	
	class CActor		*pActor;		
	class btRigidBody	*pRigidBody;
	class CContactSensorCallbackBt *pCallback;
};

#endif//__physnode_h__