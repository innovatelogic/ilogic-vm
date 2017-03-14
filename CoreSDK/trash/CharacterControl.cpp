#include "CharacterControl.h"
/*
NxU32 gNbPts = 0;
NxVec3 gPts[MAX_NB_PTS];

//----------------------------------------------------------------------------------------------
NxControllerAction ControllerHitReport::onShapeHit(const NxControllerShapeHit & hit)
{
	if (1 && hit.shape)
	{
		NxCollisionGroup group = hit.shape->getGroup();
		if (group != GROUP_COLLIDABLE_NON_PUSHABLE)
		{
			NxActor& actor = hit.shape->getActor();
			if (actor.isDynamic())
			{
				if ((gPts[gNbPts].x != hit.worldPos.x) || (gPts[gNbPts].y != hit.worldPos.y) || (gPts[gNbPts].z != hit.worldPos.z))
				{
					gPts[gNbPts++].x = (float)hit.worldPos.x;
					gPts[gNbPts++].y = (float)hit.worldPos.y;
					gPts[gNbPts++].z = (float)hit.worldPos.z;
					if (gNbPts == MAX_NB_PTS)  gNbPts = 0;
				}

				// We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
				// useless stress on the solver. It would be possible to enable/disable vertical pushes on
				// particular objects, if the gameplay requires it.
				if (hit.dir.y == 0.0f)
				{
					NxF32 coeff = actor.getMass() * hit.length * 10.0f;
					actor.addForceAtLocalPos(hit.dir*coeff, NxVec3(0,0,0), NX_IMPULSE);
					//						actor.addForceAtPos(hit.dir*coeff, hit.controller->getPosition(), NX_IMPULSE);
					//						actor.addForceAtPos(hit.dir*coeff, hit.worldPos, NX_IMPULSE);
				}
			}
		}
	}
	return NX_ACTION_NONE;
}

//----------------------------------------------------------------------------------------------
NxControllerAction ControllerHitReport::onControllerHit(const NxControllersHit& hit)
{
	return NX_ACTION_NONE;
}*/