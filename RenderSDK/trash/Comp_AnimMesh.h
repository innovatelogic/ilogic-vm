#ifndef __comp_animmesh_h__
#define __comp_animmesh_h__

#ifdef _WIN32
#pragma once
#endif

#include "Comp_StaticMesh.h"

class RENDERSDK_API Comp_AnimMesh : public Comp_StaticMesh
{
	DECLARE_CLASS_SIMPLE(Comp_AnimMesh, Comp_StaticMesh);
	
public:	
	Comp_AnimMesh(const CObjectAbstract * Parent);
	Comp_AnimMesh(const Comp_AnimMesh & Source);
	~Comp_AnimMesh();

	virtual bool			LoadMesh(const std::string & filename);

	virtual void			AdvanceAnim(const SetAnimState & Set);

	virtual bool			GetShowSkeleton() { return bShowSkeleton; }
	virtual void			SetShowSkeleton(bool Value) { bShowSkeleton = Value; }

	virtual bool			GetPlayAnim() { return bPlayAnim; }
	virtual void			SetPlayAnim(bool Value) { bPlayAnim = Value; }

	virtual const class Comp_SkeletonObject* GetSkeleton() const;

protected:
private:
	class Comp_SkeletonObject	*Skeleton;
	class Comp_AnimTrack		*Track;

	SetAnimState				AnimController;

	bool	bShowSkeleton;
	bool	bPlayAnim;
};

#endif//__comp_animmesh_h__