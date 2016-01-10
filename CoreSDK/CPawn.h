#ifndef __cpawn_h__
#define __cpawn_h__

#ifdef WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class CORESDK_API CPawn : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(CPawn, ActorAllocator);

public:
	CPawn(const CObjectAbstract* Parent);
	virtual ~CPawn();

	virtual void			OnPropertyChanged(const char* PropertyName);

	virtual void			Initialize();
	virtual void			PreRelease();

	virtual void			DoDraw();

	virtual void			SetXRef(std::string& xref) { XRef = xref; }
	virtual std::string		GetXRef() { return XRef; }

protected:
private:
	std::string						XRef;
	//class Comp_AnimMesh				*MeshComponent;
	class Comp_MaterialEffect		*MaterialEffect;
	class BoxCharacterController	*CharacterController;
	class DummyObject				*HeadTracker;
	class CCamera 					*PawnCamera;

	bool	bWalk;
	float	blend_weight;

	struct SetAnimState				*SetStates;
};

#endif//__cpawn_h__