#include "expstdafx.h"
#include "exporterafx.h"

//------------------------------------------------------------------------
_BoneObject::_BoneObject()
: MaxNode(NULL)
, MaxBoneID(INDEX_NONE)
, ID(INDEX_NONE)
, ParentID(INDEX_NONE)
, Parent(NULL)
, Next(NULL)
, Child(NULL)
{

}

//------------------------------------------------------------------------
_BoneObject::~_BoneObject()
{

}

//------------------------------------------------------------------------
void _BoneObject::LinkChildBone(_BoneObject * childBone)
{
	_BoneObject * oldChild = Child;
	Child = childBone;
	Child->Next = oldChild;
	Child->Parent = this;

	Child->SetParentID(GetID());
}

//------------------------------------------------------------------------
void _BoneObject::BuildFrame(AnimFrame * frame, int Index)
{
	AffineParts PRS;

	decomp_affine(GetMaxNode()->GetObjectTM(Index).ExtractMatrix3(), &PRS);
	/*
	frame->AddBoneTransform( Quaternion(-PRS.q.x, -PRS.q.z, PRS.q.y, PRS.q.w), 
							 Vector(PRS.t.x, PRS.t.z, -PRS.t.y), 
							 GetID());
*/
	for (size_t ChildIndex = 0; ChildIndex < Childs.size(); ++ChildIndex)
	{
		Childs[ChildIndex]->BuildFrame(frame, Index);
	}
}

//------------------------------------------------------------------------
SkeletonObjectMax::SkeletonObjectMax()
{

}

//------------------------------------------------------------------------
SkeletonObjectMax::~SkeletonObjectMax()
{
	for (std::vector<_BoneObject*>::const_iterator Iter = Bones.begin(); Iter != Bones.end(); ++Iter){
		delete (*Iter);
	}
	Bones.clear();
}
//------------------------------------------------------------------------
int	SkeletonObjectMax::Build(class IGameObject * Object)
{
	IGameSkin * skin = Object->GetIGameSkin();

	if (!skin || IGameSkin::IGAME_SKIN != skin->GetSkinType())
		return 0;

	int type, bone_id;
	const int numSkinnedVerts = skin->GetNumOfSkinnedVerts();

	for (int Index = 0; Index < numSkinnedVerts; ++Index)
	{
		type = skin->GetVertexType(Index);

		if (IGameSkin::IGAME_RIGID == type)
		{
			bone_id = skin->GetBoneID(Index, 0);
			AddMaxBone(skin->GetIGameBone(Index, 0), bone_id);
		}
		else
		{
			for (int j = 0; j < skin->GetNumberOfBones(Index); ++j)
			{
				bone_id = skin->GetBoneID(Index, j);
				AddMaxBone(skin->GetIGameBone(Index, j), bone_id);
			}
		}
	}
	return 1;
}

//------------------------------------------------------------------------
int SkeletonObjectMax::GetBoneIDByMaxID(int boneMaxID) const
{
	_BoneObject * bone = FindBoneByMaxID(boneMaxID);

	return (bone != NULL) ? bone->GetID() : INDEX_NONE;
}

//------------------------------------------------------------------------
_BoneObject* SkeletonObjectMax::FindBoneByMaxID(int boneMaxID) const
{
 	for (std::vector<_BoneObject*>::const_iterator Iter = Bones.begin(); Iter != Bones.end(); ++Iter)
 	{
 		if ((*Iter)->GetMaxBoneId() == boneMaxID){
 			return (*Iter);
 		}
 	}
	return NULL;
}
//------------------------------------------------------------------------
_BoneObject* SkeletonObjectMax::AddMaxBone(class IGameNode* boneNode, int maxBoneID)
{
	_BoneObject * bone = FindBoneByMaxID(maxBoneID);

	if (bone == NULL)
	{
		_BoneObject * newBone = new _BoneObject();

		newBone->SetMaxNode(boneNode);
		newBone->SetMaxNodeID(maxBoneID);

		std::string AdaptedName(boneNode->GetName());
		std::replace(AdaptedName.begin(), AdaptedName.end(), ' ', '_');

		newBone->SetName(AdaptedName);
		newBone->SetID(GetNumBones());

		GMatrix nodeTM;
		nodeTM = boneNode->GetObjectTM(ExporterMAX::GetExporter()->GetStaticFrame());

		AffineParts	PRS;
		decomp_affine(nodeTM.ExtractMatrix3(), &PRS);

		Matrix xform;
		xform.set_rot(Quaternion(PRS.q.x, PRS.q.z, PRS.q.y, PRS.q.w));
		xform.set_translation(Vector(-PRS.t.x, PRS.t.z, PRS.t.y));

		newBone->SetLTM(xform);

		Bones.push_back(newBone);

		// parent 
		IGameNode * pParent = boneNode->GetNodeParent();
		
		if (pParent)
		{
			_BoneObject * BoneParent = FindBoneByMaxID(pParent->GetNodeID());

			if (!BoneParent)
			{
				BoneParent = AddMaxBone(pParent, pParent->GetNodeID()); // add parent
				BoneParent->Childs.push_back(newBone);
			}
			else
			{
				newBone->SetParentID(BoneParent->GetID());
				newBone->Parent = BoneParent;
			}
		}
		else
		{
			RootBone = newBone;
			newBone->SetParentID(INDEX_NONE); // root bone
			newBone->Parent = NULL;
		}

		// children nodes...
		int numChildren = boneNode->GetChildCount();

		for (int i = 0; i < numChildren; ++i)
		{
			IGameNode * pChild = boneNode->GetNodeChild(i);
			_BoneObject * BoneChild = FindBoneByMaxID(pChild->GetNodeID());

			if (BoneChild == NULL)
			{
				BoneChild = AddMaxBone(pChild, pChild->GetNodeID());
				newBone->Childs.push_back(BoneChild);
			}
		}
		return newBone;
	}
	return bone;
}

//------------------------------------------------------------------------
void _BoneObject::Serialize(std::stringstream & ss)
{
	ss << "<BoneObject Value=\"Name:" << GetName() << " " << "BoneIndex:" << GetID() << "\">" << std::endl;

	for (size_t Index = 0; Index < Childs.size(); ++Index)
	{
		Childs[Index]->Serialize(ss);
	}
	
	ss << "</BoneObject>" << std::endl;
}

//------------------------------------------------------------------------
void _BoneObject::SerializeXForm(std::stringstream & ss)
{
	ss << GetID() << " " <<
		LTM._11 << " " << LTM._12 << " " << LTM._13 << " " << LTM._14 << " " <<
		LTM._21 << " " << LTM._22 << " " << LTM._23 << " " << LTM._24 << " " <<
		LTM._31 << " " << LTM._32 << " " << LTM._33 << " " << LTM._34 << " " <<
		LTM._41 << " " << LTM._42 << " " << LTM._43 << " " << LTM._44 << std::endl;

	for (size_t Index = 0; Index < Childs.size(); ++Index)
	{
		Childs[Index]->SerializeXForm(ss);
	}
}
//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void SkeletonObjectMax::Serialize(std::stringstream & ss)
{
	RootBone->Serialize(ss);
}

//------------------------------------------------------------------------
void SkeletonObjectMax::SerializeXForms(std::stringstream & ss)
{
	// skeleton
	ss << GetNumBones() << " " << std::endl;
	RootBone->SerializeXForm(ss);
}