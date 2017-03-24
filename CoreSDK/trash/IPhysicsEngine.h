#ifndef __iphysicsengine_h__
#define __iphysicsengine_h__

#ifdef _WIN32
#pragma once
#endif

#include "physicsafx.h"

#ifdef PHYS_ENABLED

class CORESDK_API IPhysicsEngine
{
public:
	virtual bool Initialize() = 0;
	virtual void Release() = 0;

	virtual void Simulate(float ElapsedTime) = 0;
	virtual void SimulateCharacters(float ElapsedTime) = 0;
	
	virtual void PostSimulateUpdate() = 0;
	virtual void DebugRender(class CRenderSDK *pRenderSDK) = 0;

	virtual bool SetPosition(const class CActor *pSender) = 0;
	virtual bool SetControllerPosition(const class CActor * Sender, const Vector *pPosition) = 0;

	virtual bool SetActorPosition(const class CActor * Sender, const Matrix *pWTM) = 0;
	virtual bool SetActorPositionNode(const struct SPhysNode *pPhysNode, const Matrix *pWTM) = 0;

	virtual bool ReleaseObject(const class CActor *pSender) = 0;
	virtual bool ReleaseObject(const SPhysNode *pSender) = 0;
	
	virtual bool ReleaseController(const class CActor *pSender) = 0;

	virtual bool CreateTriangleMesh(const class CActor *pOwner,
							const Vector *pVecArr,
							size_t nSize,
							const unsigned int *pIndexes,
							size_t nIdxSize) = 0;

	virtual bool CreateBoxController(const class CActor *pOwner, const Vector *pPosition, const Vector *pExtends) = 0;
	
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
							bool KINEMATIC) = 0;

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
								bool KINEMATIC) = 0;

	virtual SPhysNode* CreateSphere(const class CActor *pSender, const Matrix *pWTM, const SSphereBodyDesc& Desc) = 0;
 
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
										bool KINEMATIC) = 0;


	virtual bool CreateBoxTrigger(const class TriggerObject *pSender,
									const Matrix *pWTM,
									const Vector *pExtends,
									TriggerShapeFlag flag) = 0;

	virtual bool CreatePlane(const CActor *pSender,	const Matrix *pWTM) = 0;

	virtual SPhysNode* CreateRestrictedVolume(const CActor *pSender, const Vector &Size, const Matrix *pWTM, bool bSides[6]) = 0;
	
	virtual bool GetEnableSimulation() const = 0;
	virtual void SetEnableSimulation(bool flag) = 0;

	virtual void SetActorEnable(const struct SPhysNode *pPhysNode, bool bFlag) = 0;
	virtual bool GetActorEnable(const struct SPhysNode *pPhysNode) const = 0;

	virtual bool SetCharacterMovement(const CActor *pSender, Vector *pDisplace, float Speed = 1.f) = 0;
	virtual bool SetCharacterMovementSpeed(const CActor *pSender, float Speed) = 0;
	virtual bool CharacterJump(const CActor *pSender, float Height) = 0;

	// helper functions
	virtual float Helper_GetHeight(const CActor *pSender) = 0;

	virtual void Lock() = 0;
	virtual void Unlock() = 0;

	virtual bool GetRenderDebug() const = 0;
	virtual void SetRenderDebug(bool flag) = 0;

	virtual void AddForceAtPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode) = 0;
	virtual void AddForceAtLocalPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode) = 0;
	virtual void AddLocalForceAtPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode) = 0;
	virtual void AddLocalForceAtLocalPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode) = 0;
	virtual void AddForce(const class CActor *pSender, const Vector *pForce, EForceMode mode) = 0;
	virtual void AddLocalForce(const class CActor *pSender, const Vector *pForce, EForceMode mode) = 0;
	virtual void AddTorque(const class CActor *pSender, const Vector *pTorque, EForceMode mode) = 0;
	virtual void AddLocalTorque(const class CActor *pSender, const Vector *pTorque, EForceMode mode) = 0;

	virtual void	SetLinearVelocity(const struct SPhysNode *pPhysNode, const Vector *pValue) = 0;
	virtual Vector*	GetLinearVelocity(const class CActor *pSender) const = 0;

	virtual void	SetCentralForce(const struct SPhysNode *pPhysNode, const Vector *pValue) = 0;
	virtual Vector*	GetCentralForce(const class CActor *pSender) const = 0;

	virtual void	SetCentralImpulse(const struct SPhysNode *pPhysNode, const Vector *pValue) = 0;
	virtual Vector*	GetCentralImpulse(const class CActor *pSender) const = 0;

	virtual void SetGravity(SPhysNode *pNode, const Vector *pValue) = 0;

	virtual void ContactTest(SPhysNode* pNode) = 0;

protected:
	virtual void SyncActors() = 0;
};

#endif//PHYS_ENABLED

#endif//__iphysicsengine_h__