#ifndef __vehicleactor_h__
#define __vehicleactor_h__

#ifdef _WIN32
#pragma once
#endif

#include "coresdkafx.h"

class CORESDK_API VehicleActor : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(VehicleActor, ActorAllocator);

public:
	// Constructor/Destructor
	VehicleActor(const CObjectAbstract * Parent);
	VehicleActor(const VehicleActor & Source);
	virtual ~VehicleActor();

	virtual void			OnPropertyChanged(const char* PropertyName);

	virtual void			Initialize();

	virtual void			DoDraw();

	virtual void			SetNameMesh(const std::string& name) { XRef = name; }
	virtual std::string		GetNameMesh() const { return XRef; }

protected:
private:
	class Comp_AnimMesh				*MeshComponent;
	class ActorCollisionController	*CollisionObject;

	std::string			XRef;
};


#endif//__vehicleactor_h__