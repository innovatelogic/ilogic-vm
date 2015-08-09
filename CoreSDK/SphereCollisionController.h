#ifndef __spherecollisioncontroller_h__
#define __spherecollisioncontroller_h__

#ifdef _WIN32
#pragma once
#endif

#include "CollisionController.h"

class CSphereCollisionController : public CollisionController
{
	DECLARE_CLASS_SIMPLE(CSphereCollisionController, CollisionController);

public:
	CSphereCollisionController(const class CObjectAbstract *pParent);
	CSphereCollisionController(const CSphereCollisionController &Source);
	~CSphereCollisionController();

	virtual void		OnPropertyChanged(const char *PropertyName);

	virtual void		Initialize();

	void				SetRadius(float Value) { m_fRadius = Value; }
	float				GetRadius() const { return m_fRadius; }
	
	virtual bool		CreatePhysActor(class CContactSensorCallbackBt *pCallback = 0);

protected:
private:
	float				m_fRadius;
};

#endif//__spherecollisioncontroller_h__