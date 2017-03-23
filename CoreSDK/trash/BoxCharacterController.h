#ifndef __boxcharactercontroller_h__
#define __boxcharactercontroller_h__

#ifdef _WIN32
#pragma once
#endif

#include "BaseModificator.h"

class BoxCharacterController : public BaseModificator
{
	DECLARE_CLASS_SIMPLE(BoxCharacterController, BaseModificator);

public:
	BoxCharacterController(const CObjectAbstract *parent);
	virtual ~BoxCharacterController();

	virtual void		OnPropertyChanged(const char* PropertyName);

	virtual void		Initialize();

	virtual void		OnChangePivot(const CActor * Sender, ESystemEventID EventId);

	/** rendering */
	virtual void		DrawBounds() const {}

	/* property interface*/
	Vector		GetExtends() const { return Extends; }
	void		SetExtends(Vector &Ext) { Extends = Ext; }
	
	float		GetUpForce() const { return UpForce; }
	void		SetUpForce(float Force) { UpForce = Force; }

	float		GetMoveSpeed() const { return MoveSpeed; }
	void		SetMoveSpeed(float Speed) { MoveSpeed = Speed; }

protected:
	virtual void		CreatePhysActor();

private:
	/* box extends */
	Vector				Extends;
	float				UpForce;
	float				MoveSpeed;
};

#endif//__boxcharactercontroller_h__