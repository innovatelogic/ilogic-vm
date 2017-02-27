#ifndef __collisioncontroller_h__
#define __collisioncontroller_h__

#ifdef _WIN32
#pragma once
#endif

#include "ICollisionInterface.h"
#include "BaseModificator.h"

class CollisionController : public BaseModificator,
							public oes::common_base::ICollisionInterface
{
	DECLARE_CLASS_SIMPLE(CollisionController, BaseModificator);

public:
	CollisionController(const CObjectAbstract *pParent);
	virtual ~CollisionController();

	virtual void		OnPropertyChanged(const char *PropertyName);

	virtual void		OnChangePivot(const CActor *pSender, ESystemEventID EventId);

	/** ICollisionInterface impl. */
	virtual void		Enable(bool bFlag); 

	// properties
	bool		GetStatic() const		{ return bStatic; }
	void		SetStatic(bool flag)	{ bStatic = flag; }

	float		GetDesteny() const		{ return Desteny; }
	void		SetDesteny(float Value) { Desteny = Value; }

	bool		Get2DMode()	const		{ return b2DMode; }
	void		Set2DMode(bool flag)	{ b2DMode = flag; }

	bool		Get_DISABLE_GRAVITY() const		{ return DISABLE_GRAVITY; }
	void		Set_DISABLE_GRAVITY(bool flag)	{ DISABLE_GRAVITY = flag; }

	bool		Get_FROZEN_POS_X() const	{ return FROZEN_POS_X; }
	void		Set_FROZEN_POS_X(bool flag) { FROZEN_POS_X = flag; }

	bool		Get_FROZEN_POS_Y() const	{ return FROZEN_POS_Y; }
	void		Set_FROZEN_POS_Y(bool flag) { FROZEN_POS_Y = flag; }

	bool		Get_FROZEN_POS_Z() const	{ return FROZEN_POS_Z; }
	void		Set_FROZEN_POS_Z(bool flag) { FROZEN_POS_Z = flag; }

	bool		Get_FROZEN_ROT_X() const	{ return FROZEN_ROT_X; }
	void		Set_FROZEN_ROT_X(bool flag) { FROZEN_ROT_X = flag; }

	bool		Get_FROZEN_ROT_Y() const	{ return FROZEN_ROT_Y; }
	void		Set_FROZEN_ROT_Y(bool flag) { FROZEN_ROT_Y = flag; }

	bool		Get_FROZEN_ROT_Z() const	{ return FROZEN_ROT_Z; }
	void		Set_FROZEN_ROT_Z(bool flag) { FROZEN_ROT_Z = flag; }

	bool		Get_FROZEN_POS() const		{ return FROZEN_POS; }
	void		Set_FROZEN_POS(bool flag)	{ FROZEN_POS = flag; }

	bool		Get_FROZEN_ROT() const		{ return FROZEN_ROT; }
	void		Set_FROZEN_ROT(bool flag)	{ FROZEN_ROT = flag; }

	bool		Get_KINEMATIC() const		{ return KINEMATIC; }
	void		Set_KINEMATIC(bool flag)	{ KINEMATIC = flag; }

	inline_		Matrix		GetLTM() const { return m_LTM; }
	inline_		SPhysNode*  GetPNode() const { return m_pPhysNode; }

	void		ContactTest();

	virtual void SetWorldPos(const Matrix &matrix);

protected:
private:
	bool		bStatic;
	float		Desteny;
	bool		b2DMode;

	bool		DISABLE_GRAVITY;
	bool		FROZEN_POS_X;
	bool		FROZEN_POS_Y;
	bool		FROZEN_POS_Z;
	bool		FROZEN_ROT_X;   
	bool		FROZEN_ROT_Y;   
	bool		FROZEN_ROT_Z;
	bool		FROZEN_POS;
	bool		FROZEN_ROT;
	bool		KINEMATIC;

	mutable Matrix	m_LTM;

protected:
	struct SPhysNode *m_pPhysNode;
};

#endif// __collisioncontroller_h__