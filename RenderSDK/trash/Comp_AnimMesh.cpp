#include "StdafxRender.h"

REGISTER_CLASS_A(Comp_AnimMesh, Comp_StaticMesh)
	new PropertyBOOL("bShowSkeleton", (BYTE*)&((Comp_AnimMesh*)NULL)->bShowSkeleton - (BYTE*)NULL, "Comp_AnimMesh",	"Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyBOOL("bPlayAnim", (BYTE*)&((Comp_AnimMesh*)NULL)->bPlayAnim - (BYTE*)NULL, "Comp_AnimMesh",	"Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(Comp_AnimMesh, Comp_StaticMesh);

//------------------------------------------------------------------------
Comp_AnimMesh::Comp_AnimMesh(const CObjectAbstract * Parent)
: Super(Parent)
, bShowSkeleton(false)
{
	NEW_OBJECT_TRANSIENT_CHILD(Skeleton, Comp_SkeletonObject, "SkeletonObject", this);
	NEW_OBJECT_TRANSIENT_CHILD(Track, Comp_AnimTrack, "AnimTrack", this);
}

//------------------------------------------------------------------------
Comp_AnimMesh::Comp_AnimMesh(const Comp_AnimMesh & Source)
: Super(Source)
, bShowSkeleton(false)
{
	if (this != &Source)
	{
		Skeleton = CLONE_OBJECT(Comp_SkeletonObject, Source.Skeleton, "SkeletonObject", this);
		Track	 = CLONE_OBJECT(Comp_AnimTrack, Source.Track, "AnimTrack", this);
	}
}

//------------------------------------------------------------------------
Comp_AnimMesh::~Comp_AnimMesh()
{

}

//-----------------------------------------------------------------------
bool Comp_AnimMesh::LoadMesh(const std::string & filename)
{
// 	if (Super::LoadMesh(filename))
// 	{
// 		if (Skeleton->LoadFromFile(MaterialSet.SklFile) &&
// 			Track->LoadFromFile(MaterialSet.TrkFile))
// 		{
// 			return true;
// 		}
// 	}
	return false;
}

//------------------------------------------------------------------------
void Comp_AnimMesh::AdvanceAnim(const SetAnimState& Set)
{
/*	if (bPlayAnim)
	{
		std::vector<AnimState>::iterator IterUpdate = AnimController.States.begin();
		while (IterUpdate != AnimController.States.end())
		{
			(*IterUpdate).bValid = false;
			++IterUpdate;
		}

		// go throw all update set
		std::vector<AnimState>::const_iterator Iter = Set.States.begin();
		while (Iter != Set.States.end())
		{
			bool bFind = false;

			// try to update current
			std::vector<AnimState>::iterator IterUpdate = AnimController.States.begin();

			while (IterUpdate != AnimController.States.end())
			{
				if ((*IterUpdate).AnimName == (*Iter).AnimName) // update current
				{
					// copy data
					(*IterUpdate).DeltaTime += (*Iter).DeltaTime;

					if (size_t((*IterUpdate).DeltaTime) >=  Track->pSkeleton->GetNumFrames((*IterUpdate).AnimName)){
						(*IterUpdate).DeltaTime = 0.f;
					}

					(*IterUpdate).bTrackTarget = (*Iter).bTrackTarget;
					(*IterUpdate).PointTarget = (*Iter).PointTarget;
					(*IterUpdate).BonesMask = (*Iter).BonesMask;
					(*IterUpdate).weight = (*Iter).weight;
					(*IterUpdate).bValid = true; // validate

					bFind = true;
					break;
				}
				++IterUpdate;
			}

			if (bFind == false) // add anim state
			{
				AnimController.States.push_back(*Iter);
			}
			++Iter;
		}

		// remove non valid unused elements that contains in AnimController and miss in update Set
 		AnimController.States.erase(
			std::remove_if(AnimController.States.begin(), AnimController.States.end(), std::not1(std::mem_fun_ref(&AnimState::IsValid))), 
 			AnimController.States.end()
 			);

		Skeleton->ApplyFrames(AnimController, Track->pSkeleton);

		for (TVecCompMeshNodesConstIterator Iter = MeshNodes.begin(); Iter != MeshNodes.end(); ++Iter)
		{
			if ((*Iter)->Mesh->pMeshNode->m_IsSkinned)
			{
				Skeleton->ApplyMesh(*Iter);
			}
		}	
	}*/
}

//------------------------------------------------------------------------
const Comp_SkeletonObject* Comp_AnimMesh::GetSkeleton() const
{
	return Skeleton;
}