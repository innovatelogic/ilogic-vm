#pragma once

#include "CriticalSection.h"
#include "mathlib.h"

#define PHIS_MULTYPLIER 1.0f

enum GameGroup
{
	GROUP_NON_COLLIDABLE,
	GROUP_COLLIDABLE_NON_PUSHABLE,
	GROUP_COLLIDABLE_PUSHABLE,
};

#define COLLIDABLE_MASK	(1<<GROUP_COLLIDABLE_NON_PUSHABLE) | (1<<GROUP_COLLIDABLE_PUSHABLE)

// wrapper under NxForceMode
enum EForceMode
{
	PHYS_FORCE					= 0,	//!< parameter has unit of mass * distance/ time^2, i.e. a force
	PHYS_IMPULSE				= 1,	//!< parameter has unit of mass * distance /time
	PHYS_VELOCITY_CHANGE		= 2,	//!< parameter has unit of distance / time, i.e. the effect is mass independent: a velocity change.
	PHYS_SMOOTH_IMPULSE			= 3,	//!< same as NX_IMPULSE but the effect is applied over all substeps. Use this for motion controllers that repeatedly apply an impulse.
	PHYS_SMOOTH_VELOCITY_CHANGE = 4,	//!< same as NX_VELOCITY_CHANGE but the effect is applied over all substeps. Use this for motion controllers that repeatedly apply an impulse.
	PHYS_ACCELERATION			= 5,	//!< parameter has unit of distance/ time^2, i.e. an acceleration. It gets treated just like a force except the mass is not divided out before integration.
};

enum TriggerShapeFlag
{
	PHYS_NO_EVENT						= 0,

	// Trigger callback will be called when a shape enters the trigger volume.
	PHYS_TRIGGER_ON_ENTER				= (1<<0),
	
	// Trigger callback will be called after a shape leaves the trigger volume.
	PHYS_TRIGGER_ON_LEAVE				= (1<<1),
	
	// Trigger callback will be called while a shape is intersecting the trigger volume.
	PHYS_TRIGGER_ON_STAY				= (1<<2),

	// Combination of all the other trigger enable flags.
	PHYS_TRIGGER_ENABLE				= PHYS_TRIGGER_ON_ENTER|PHYS_TRIGGER_ON_LEAVE|PHYS_TRIGGER_ON_STAY,
};

struct SBodyDesc
{
	bool bStatic;
	float fDensity;
	bool DISABLE_GRAVITY;
	bool FROZEN_POS_X;
	bool FROZEN_POS_Y;
	bool FROZEN_POS_Z;
	bool FROZEN_ROT_X;   
	bool FROZEN_ROT_Y;   
	bool FROZEN_ROT_Z;
	bool FROZEN_POS;
	bool FROZEN_ROT;
	bool KINEMATIC;
	
    class CContactSensorCallbackBt *pCallback;

	SBodyDesc()
		: bStatic(false)
		, fDensity(1.f)
		, DISABLE_GRAVITY(false)
		, FROZEN_POS_X(false)
		, FROZEN_POS_Y(false)
		, FROZEN_POS_Z(false)
		, FROZEN_ROT_X(false)
		, FROZEN_ROT_Y(false)
		, FROZEN_ROT_Z(false)
		, FROZEN_POS(false)
		, FROZEN_ROT(false)
		, KINEMATIC(false)
		, pCallback(NULL)
	{
	}
};

struct SSphereBodyDesc : public SBodyDesc
{
	float fRadius;

	SSphereBodyDesc()
		: fRadius(1.f)
	{
	}
};

struct SBoxBodyDesc : public SBodyDesc
{
	Vector Extends;

	SBoxBodyDesc()
		: Extends(1.f, 1.f, 1.f)
	{
	}
};