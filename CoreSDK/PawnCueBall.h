#ifndef __pawncueball_h__
#define __pawncueball_h__

#ifdef WIN32
#pragma once
#endif

#include "ActorAllocator.h"

enum EMoveDirection
{
	EMoveDirectionUp = 0,
	EMoveDirectionDown,
};

class CORESDK_API CPawnCueBall : public ActorAllocator,
	public IDrawInterface,
	public IEntityController,
	public IUpdateInterface
{
	DECLARE_CLASS_SIMPLE(CPawnCueBall, ActorAllocator);

public:
	CPawnCueBall(const CObjectAbstract *pParent);
	virtual ~CPawnCueBall();

	virtual void			OnPropertyChanged(const char *pPropertyName);

	virtual void			Initialize();
	virtual void			PreRelease();

	virtual void			DoDraw();

	// IEntityController
	virtual bool			ProcessInputMouseEnt(const MouseInputData &InputData);
	virtual bool			ProcessInputMouseEnt(const MouseMoveInputData &InputData);
	virtual bool			ProcessButton(const EventInput &InputData);

	// IUpdateInterface
	virtual void			Update(float DeltaTime);
	virtual void			PostPhysUpdate(float DeltaTime);

	EMoveDirection			GetMoveDirection() const { return m_EMoveDirection; }
	void					SetMoveDirection(EMoveDirection dir) { m_EMoveDirection = dir; }

	virtual void			PostPhysEvent(EPhysEventCallback EVENT_ID, void *pParamA, void *pParamB);

protected:
private:
	EMoveDirection m_EMoveDirection;

	class Comp_StaticMesh				*m_pMeshComponent;
	class CSphereCollisionController	*m_pSphereController;
	class CContactSensorCallbackBt		*m_pContactCallback;

	std::string							m_XRef;

	Vector								m_linearVelocity;
	Vector								m_totalForce; // Newtons

	Matrix								m_worldTransform;
	Matrix								m_worldTransformPrev;
};

#endif//__pawncueball_h__