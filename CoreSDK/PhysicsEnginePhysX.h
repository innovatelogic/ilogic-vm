#ifndef __physics_engine_h__
#define __physics_engine_h__

#ifdef _WIN32
#pragma once
#endif

#include "IPhysicsEngine.h"

/**
	PhysX engine wrapper
*/
/*
class CORESDK_API PhysicsEnginePhysX : public IPhysicsEngine
{
	typedef std::map <class NxController*,	MovementData> TMapMovementData;
	typedef TMapMovementData::iterator		TMapMovementDataIterator;
	
	typedef std::map<class CActor*, Matrix> TMapSyncData;
	typedef TMapSyncData::iterator			TMapSyncDataIterator;

public:
	PhysicsEnginePhysX(class CCoreSDK *pSDK);
    virtual ~PhysicsEnginePhysX();

	virtual bool Initialize();
	virtual void Release();

	virtual void Simulate(float fElapsedTime);
	virtual void SimulateCharacters(float fElapsedTime);
	
	virtual void PostSimulateUpdate();
	virtual void DebugRender(class CRenderSDK *pRenderSDK);

	virtual bool SetPosition(const class CActor *pSender);
	virtual bool SetControllerPosition(const class CActor *pSender, const Vector *pPosition);
	virtual bool SetActorPosition(const class CActor *pSender, const Matrix *pWTM);
	virtual bool ReleaseObject(const class CActor *pSender);
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

	virtual bool CreateActorFromFile(const char *filename,
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
		
	virtual bool GetEnableSimulation() const { return bEnableSimulation; }
	virtual void SetEnableSimulation(bool flag) { bEnableSimulation = flag; }

	virtual bool SetCharacterMovement(const CActor *pSender, Vector *displace, float Speed = 1.f);
	virtual bool SetCharacterMovementSpeed(const CActor *pSender, float Speed);
	virtual bool CharacterJump(const CActor *pSender, float Height);

	// helper functions
	virtual float Helper_GetHeight(const CActor *pSender);

	virtual void Lock() { PhysXUpdateCS.enter(); }
	virtual void Unlock() { PhysXUpdateCS.leave(); }

	virtual bool GetRenderDebug() const { return bRenderDebug; }
	virtual void SetRenderDebug(bool flag) { bRenderDebug = flag; }

	virtual void AddForceAtPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode);
	virtual void AddForceAtLocalPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode);
	virtual void AddLocalForceAtPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode);
	virtual void AddLocalForceAtLocalPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode);
	virtual void AddForce(const class CActor *pSender, const Vector *pForce, EForceMode mode);
	virtual void AddLocalForce(const class CActor *pSender, const Vector *pForce, EForceMode mode);
	virtual void AddTorque(const class CActor *pSender, const Vector *pTorque, EForceMode mode);
	virtual void AddLocalTorque(const class CActor *pSender, const Vector *pTorque, EForceMode mode);

	virtual void	SetLinearVelocity(const class CActor *pSender, const Vector *pValue);
	virtual Vector*	GetLinearVelocity(const class CActor *pSender) const;

	void SetActorFlags(NxActor *pActor,
							bool bStatic,
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

	unsigned int MoveCharacter(NxController *pController,
								const Vector *pDispVector,
								float elapsedTime,
								unsigned int collisionGroups,
								float heightDelta);
	
	void ReleaseNxActor(NxActor *pActor);

protected:
	virtual void SyncActors();

private:
	CCoreSDK					*pCoreSDK;
	float						FixedTimestep;

	bool						bEnableSimulation;
	class NxPhysicsSDK			*m_pPhisicsSDK;
	class NxScene				*m_pScene;
	class UserAllocator			*m_pMyAllocator;
	class NxControllerManager	*m_pControllerManager;
	class ControllerHitReport	*m_pControllerReport;
	class TriggerCallbackReport *m_pTriggerCallbackReport;

	// controllers movement data
	TMapMovementData			MMovementData;
	// gravity
	NxVec3						DefaultGravity;
	CriticalSection				PhysXUpdateCS;

	TMapSyncData				SyncPhysicsInfo;

	bool						bRenderDebug;
};

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template <class T_CLASS, class T_BASE> class CommandReleaseNxActor
: public CommandBase
{	
	typedef void (T_CLASS::*ExecuteCommand)(T_BASE*);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;
	T_BASE				*Param;

	CommandReleaseNxActor(ExecuteCommand pfRI, T_CLASS* tParam, T_BASE	*vParam)
		: pfn_RI(pfRI)
		, TParam(tParam)
		, Param(vParam)
	{
	}

	virtual void Execute() const
	{
		(TParam->*pfn_RI)(Param);
	}
};
*/
#endif//__phisics_engine_h__