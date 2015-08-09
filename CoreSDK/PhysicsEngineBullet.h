#ifndef __physicsenginebullet_h__
#define __physicsenginebullet_h__

#ifdef _WIN32
#pragma once
#endif

#include "IPhysicsEngine.h"

class CActor;

class CORESDK_API CPhysicsEngineBullet : public IPhysicsEngine
{
	typedef std::map<CActor*, Matrix> TMapSyncData;
	typedef TMapSyncData::iterator			TMapSyncDataIterator;

	typedef std::list<struct SPhysNode*>	TListPhysNode;

public:
	CPhysicsEngineBullet(class CCoreSDK *pSDK);
	virtual ~CPhysicsEngineBullet();

	virtual bool Initialize();
	virtual void Release();

	virtual void Simulate(float fElapsedTime);
	virtual void SimulateCharacters(float fElapsedTime);
	
	virtual void PostSimulateUpdate();
	virtual void DebugRender(class CRenderSDK *pRenderSDK);

	virtual bool SetPosition(const CActor *pSender);
	virtual bool SetControllerPosition(const CActor *pSender, const Vector *pPosition);

	virtual bool SetActorPosition(const CActor *pSender, const Matrix *pWTM);
	virtual bool SetActorPositionNode(const struct SPhysNode *pPhysNode, const class Matrix *pWTM);
	
	virtual bool ReleaseObject(const class CActor *pSender);
	virtual bool ReleaseObject(const SPhysNode *pSender);

	virtual bool ReleaseController(const class CActor *pSender);

	virtual bool CreateTriangleMesh(const class CActor *pOwner,
									const Vector *pVecArr,
									size_t nSize,
									const unsigned int *pIndexes,
									size_t nIdxSize);

	virtual bool CreateBoxController(const class CActor *pOwner, const Vector *pPosition, const Vector *pExtends);
	
	virtual bool CreateBox(const class CActor *pSender,
							const Matrix *pWTM, 
							const Vector *pExtends,
							bool bStatic,
							float density,
							bool DISABLE_GRAVITY,
							bool FROZEN_POS_X,
							bool FROZEN_POS_Y,
							bool FROZEN_POS_Z,
							bool FROZEN_ROT_X,   
							bool FROZEN_ROT_Y,   
							bool FROZEN_ROT_Z,
							bool FROZEN_POS,
							bool FROZEN_ROT,
							bool KINEMATIC);

	virtual bool CreateSphere(const class CActor *pSender, 
								const Matrix *pWTM, 
								const float Radius,
								bool bStatic,
								float density,
								bool DISABLE_GRAVITY,
								bool FROZEN_POS_X,
								bool FROZEN_POS_Y,
								bool FROZEN_POS_Z,
								bool FROZEN_ROT_X,   
								bool FROZEN_ROT_Y,   
								bool FROZEN_ROT_Z,
								bool FROZEN_POS,
								bool FROZEN_ROT,
								bool KINEMATIC);

	virtual SPhysNode* CreateSphere(const class CActor *pSender, const Matrix *pWTM, const SSphereBodyDesc &Desc);

	virtual SPhysNode* CreateActorFromFile(const char *filename,
										const CActor *pSender,
										const Matrix *pWTM,
										bool bStatic,
										float density,
										bool DISABLE_GRAVITY,
										bool FROZEN_POS_X,
										bool FROZEN_POS_Y,
										bool FROZEN_POS_Z,
										bool FROZEN_ROT_X,   
										bool FROZEN_ROT_Y,   
										bool FROZEN_ROT_Z,
										bool FROZEN_POS,
										bool FROZEN_ROT,
										bool KINEMATIC);

	virtual bool CreateBoxTrigger(const class TriggerObject *pSender,
									const Matrix *pWTM, 
									const Vector *pExtends,
									TriggerShapeFlag flag);

	virtual bool CreatePlane(const CActor *pSender,	const Matrix *pWTM);

	virtual SPhysNode* CreateRestrictedVolume(const CActor *pSender, const Vector &Size, const Matrix *pWTM, bool bSides[6]);
		
	virtual bool GetEnableSimulation() const { return m_bEnableSimulation; }
	virtual void SetEnableSimulation(bool flag) { m_bEnableSimulation = flag; }

	virtual void SetActorEnable(const struct SPhysNode *pPhysNode, bool bFlag);
	virtual bool GetActorEnable(const struct SPhysNode *pPhysNode) const;

	virtual bool SetCharacterMovement(const CActor *pSender, Vector *displace, float Speed = 1.f);
	virtual bool SetCharacterMovementSpeed(const CActor *pSender, float Speed);
	virtual bool CharacterJump(const CActor *pSender, float Height);

	// helper functions
	virtual float Helper_GetHeight(const CActor *pSender);

	virtual void Lock() { m_PhysUpdateCS.enter(); }
	virtual void Unlock() { m_PhysUpdateCS.leave(); }

	virtual bool GetRenderDebug() const { return m_bRenderDebug; }
	virtual void SetRenderDebug(bool flag) { m_bRenderDebug = flag; }

	virtual void AddForceAtPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode);
	virtual void AddForceAtLocalPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode);
	virtual void AddLocalForceAtPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode);
	virtual void AddLocalForceAtLocalPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode);
	virtual void AddForce(const class CActor *pSender, const Vector *pForce, EForceMode mode);
	virtual void AddLocalForce(const class CActor *pSender, const Vector *pForce, EForceMode mode);
	virtual void AddTorque(const class CActor *pSender, const Vector *pTorque, EForceMode mode);
	virtual void AddLocalTorque(const class CActor *pSender, const Vector *pTorque, EForceMode mode);

	virtual void	SetCentralForce(const struct SPhysNode *pPhysNode, const Vector *pValue);
	virtual Vector*	GetCentralForce(const class CActor *pSender) const;

	virtual void	SetCentralImpulse(const struct SPhysNode *pPhysNode, const Vector *pValue);
	virtual Vector*	GetCentralImpulse(const class CActor *pSender) const;
	
	virtual void	SetLinearVelocity(const struct SPhysNode *pPhysNode, const Vector *pValue);
	virtual Vector*	GetLinearVelocity(const class CActor *pSender) const;

	virtual void SetGravity(SPhysNode *pNode, const Vector *pValue);

	virtual void ContactTest(struct SPhysNode *pNode);

protected:
	virtual void SyncActors();
	
private:
	void	ProcessContacts();

	CActor* GetActorByShape(const class btCollisionShape *pShape) const;

private:
	class btBroadphaseInterface				*m_pBroadPhase;
	class btCollisionDispatcher				*m_pDispatcher;
	class btConstraintSolver				*m_pConstraintSolver;
	class btDefaultCollisionConfiguration	*m_pCollisionConfiguration;
	class btDynamicsWorld					*m_pDynamicsWorld;

	//keep the collision shapes, for deletion/cleanup
	btAlignedObjectArray<class btCollisionShape*> m_CollisionShapes;

	class CPhysDebugDrawerGL				*m_pDebugDrawer;

	TMapSyncData							m_SyncData;

	TListPhysNode							m_ListPhysNodes;

	bool				m_bEnableSimulation;
	CriticalSection		m_PhysUpdateCS;
	bool				m_bRenderDebug;

	class CCoreSDK		*m_pCoreSDK;
};

#endif//__physicsenginebullet_h__