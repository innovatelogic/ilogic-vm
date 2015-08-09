#include "StdafxRender.h"

REGISTER_CLASS(Comp_SkeletonObject, CRenderObject);

//------------------------------------------------------------------------
Comp_SkeletonObject::Comp_SkeletonObject(const CObjectAbstract * Parent)
: Super(Parent)
{

}


//------------------------------------------------------------------------
Comp_SkeletonObject::Comp_SkeletonObject(const Comp_SkeletonObject & Source)
: Super(Source)
{

}

//------------------------------------------------------------------------
Comp_SkeletonObject::~Comp_SkeletonObject()
{

}

//------------------------------------------------------------------------
bool Comp_SkeletonObject::LoadFromFile(const char *filename)
{
	if (Super::LoadFromFile(filename))
	{
		std::vector<const CActor*> TmpArray;
		GetObjectsByClass(TmpArray, "BoneObject", true);
		
		for (size_t Index = 0; Index < TmpArray.size(); ++Index)
		{
			const BoneObject * bone = static_cast<const BoneObject*>(TmpArray[Index]);

			if (bone)
			{
				AddBoneObjectRef(bone);
				bone->RootSkeleton = this;
			}
			
		}
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
void Comp_SkeletonObject::AddBoneObjectRef(const BoneObject * bone)
{
	TVecBones::const_iterator Iter = std::find(ChildBones.begin(), ChildBones.end(), bone);

	if (Iter == ChildBones.end())
	{
		ChildBones.push_back(bone);
	}
}

//------------------------------------------------------------------------
void Comp_SkeletonObject::RemoveBoneObjectRef(const class BoneObject * bone)
{
	TVecBones::const_iterator Iter = std::find(ChildBones.begin(), ChildBones.end(), bone);

	if (Iter != ChildBones.end()){
		ChildBones.erase(Iter);
	}
}

//------------------------------------------------------------------------
const BoneObject* Comp_SkeletonObject::GetBoneByID(int id)
{
	for (size_t Index = 0; Index < ChildBones.size(); ++Index)
	{
		if (ChildBones[Index]->GetBoneIndex() == id)
		{
			return ChildBones[Index];
		}
	}
	return NULL;
}

//------------------------------------------------------------------------
void Comp_SkeletonObject::ApplyFrames(const SetAnimState& Controller, const D3DSkeleton * Skeleton)
{
	for (size_t Index = 0; Index < ChildBones.size(); ++Index)
	{
		ChildBones[Index]->ApplyFrames(Controller, Skeleton);
	}
}

//------------------------------------------------------------------------
void Comp_SkeletonObject::ApplyMesh(const Comp_MeshNode * mesh)
{
//   	for (size_t Index = 0; Index < mesh->Mesh->pMeshNode->m_Bones.size(); ++Index)
//   	{
//   		int IndexBone = mesh->Mesh->pMeshNode->m_Bones[Index];
//   
//   		const BoneObject * Bone = GetBoneByID(IndexBone);
//   
//   		if (Bone){
//  			mesh->Mesh->SetBoneTransform(Index, Bone->GetPivot().GetPtr());
//   		}
//   	}
}

//------------------------------------------------------------------------
void Comp_SkeletonObject::RenderBones(const Matrix& ParentTransform) const
{
	for (size_t Index = 0; Index < ChildBones.size(); ++Index)
	{
		ChildBones[Index]->RenderBone(ParentTransform);
	}
}