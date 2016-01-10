#ifndef __boneobject_h__
#define __boneobject_h__

#ifdef _WIN32
#pragma once
#endif

#include "RenderObject.h"

class RENDERSDK_API BoneObject : public CRenderObject
{
	DECLARE_CLASS_SIMPLE(BoneObject, CRenderObject);

public:
	BoneObject(const class CObjectAbstract * Parent);
	BoneObject(const class BoneObject & Source);
	virtual ~BoneObject();

	virtual void			PreRelease();

	virtual unsigned int	GetBoneIndex() const { return BoneIndex; }
	virtual void			SetBoneIndex(unsigned int index) { BoneIndex = index; }
	
	virtual void			RenderBone(const Matrix& ParentTransform) const;
	virtual void			ApplyFrames(const SetAnimState& Controller, const class D3DSkeleton * Skeleton) const;

protected:
private:
	unsigned int			BoneIndex;

public:
	mutable class Comp_SkeletonObject * RootSkeleton;
};

#endif//__boneobject_h__