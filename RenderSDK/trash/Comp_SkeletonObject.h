#ifndef __comp_skeletonobject_h__
#define __comp_skeletonobject_h__

#ifdef _WIN32
#pragma once
#endif

#include "RenderObject.h"

class RENDERSDK_API Comp_SkeletonObject : public CRenderObject
{
	DECLARE_CLASS_SIMPLE(Comp_SkeletonObject, CRenderObject);

	typedef std::vector<const class BoneObject*> TVecBones;

public:
	Comp_SkeletonObject(const CObjectAbstract * Parent);
	Comp_SkeletonObject(const Comp_SkeletonObject & Source);
	virtual ~Comp_SkeletonObject();

	virtual bool			LoadFromFile(const char *filename);

	virtual size_t			GetNumBones() const { return ChildBones.size(); }

	virtual void			AddBoneObjectRef(const class BoneObject * bone);
	virtual void			RemoveBoneObjectRef(const class BoneObject * bone);
	
	virtual const class BoneObject* GetBoneByID(int id);

	virtual void			ApplyFrames(const SetAnimState& Controller, const class D3DSkeleton * Skeleton);

	virtual void			ApplyMesh(const class Comp_MeshNode * mesh);

	virtual void			RenderBones(const Matrix& ParentTransform) const;

protected:
public:
	TVecBones ChildBones;
};

#endif//__comp_skeletonobject_h__