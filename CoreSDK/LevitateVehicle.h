#ifndef __levitatevehicle_h__
#define __levitatevehicle_h__

#ifdef WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class CORESDK_API LevitateVehicle : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(LevitateVehicle, ActorAllocator);

public:
	LevitateVehicle(const CObjectAbstract * Parent);
	LevitateVehicle(const LevitateVehicle & Source);
	virtual ~LevitateVehicle();

	virtual void		OnPropertyChanged(const char* PropertyName);

	virtual void		Initialize();

	virtual void		DoDraw();

	virtual void			SetNameMesh(const std::string& name) { XRef = name; }
	virtual std::string		GetNameMesh() const { return XRef; }

protected:
private:
	class Comp_StaticMesh					*MeshComponent;
	class ActorCollisionKinematicController	*CollisionController;

	std::string			XRef;
};

#endif//__levitatevehicle_h__