#include "StdafxRender.h"

REGISTER_CLASS_A(BoneObject, CRenderObject)
	new PropertyUINT("BoneIndex", (BYTE*)&((BoneObject*)NULL)->BoneIndex - (BYTE*)NULL, "BoneObject", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(BoneObject, CRenderObject);

//----------------------------------------------------------------------------------
BoneObject::BoneObject(const CObjectAbstract * Parent)
: Super(Parent)
, BoneIndex(0)
, RootSkeleton(NULL)
{
}

//----------------------------------------------------------------------------------
BoneObject::BoneObject(const BoneObject & Source)
: Super(Source)
{
	if (this != &Source)
	{
		BoneIndex = Source.BoneIndex;
	}
}

//----------------------------------------------------------------------------------
BoneObject::~BoneObject()
{

}

//----------------------------------------------------------------------------------
void BoneObject::PreRelease()
{
	Super::PreRelease();

	if (RootSkeleton != 0){
		RootSkeleton->RemoveBoneObjectRef(this);
	}
}

//----------------------------------------------------------------------------------
void BoneObject::RenderBone(const Matrix& Transform) const
{
/*	for (size_t Index = 0; Index < ChildNodes.size(); ++Index)
	{
		BoneObject * bone = dynamic_cast<BoneObject*>(ChildNodes[Index]);
		if (bone)
		{
			GetRenderComponent()->DrawLine((Transform * WorldMatrixTransform).t, (Transform * bone->WorldMatrixTransform).t, COLOR_RED, false);
			bone->RenderBone(Transform);
		}
	}*/
}

//----------------------------------------------------------------------------------
void BoneObject::ApplyFrames(const SetAnimState& Controller, const D3DSkeleton * pSkeleton) const
{	
	Pose ResPose;
	Matrix BoneXForm;
	bool bAffectBone = false;
	bool bTrack = false;
	Vector TrackPoint;
		
	pSkeleton->GetBoneBindMatrix(BoneXForm.GetPtr(), BoneIndex);

 	for (std::vector<AnimState>::const_iterator Iter = Controller.States.begin(); Iter != Controller.States.end(); ++Iter)
 	{
 		if ((*Iter).BonesMask.empty() || std::find((*Iter).BonesMask.begin(), (*Iter).BonesMask.end(), BoneIndex) != (*Iter).BonesMask.end())
 		{
 			if ((*Iter).bTrackTarget == true) // set temp track position
 			{ 
 				TrackPoint = (*Iter).PointTarget;
 				bTrack = true;
 			}

			Pose TPose;
			pSkeleton->GetFramePosition(TPose.Rotation.GetPtr(), TPose.Position.GetPtr(), (*Iter).AnimName, BoneIndex, (*Iter).DeltaTime);
 
 			ResPose = !bAffectBone ? TPose : Lerp(ResPose, (*Iter).weight, ResPose, TPose);
 
 			bAffectBone = true;
 		}
 	}

	if (bAffectBone)
	{	
		Matrix xform, refxform, mattmp(ResPose.Rotation, ResPose.Position);

		invert(refxform, BoneXForm);

// 		if (bTrack)
// 		{
// 			Matrix M;
// 
//  			M._row0 = TrackPoint - ResPose.Position;
//  			cross(M._row2, M._row0, Vector(0.f, 1.f, 0.f));
//  			cross(M._row1, M._row0, M._row2);
// 			
//  			M._row0.normalize();
//  			M._row1.normalize();
//  			M._row2.normalize();
// 
// 			std::swap(M._row0, M._row1);
// 			M._row0 *= -1.f;
// 			M._row2 *= -1.f;
// 
// 			mattmp = M;
// 			mattmp.t = ResPose.Position;
// 		}

		xform = refxform * mattmp;

		//SetPivot(xform);

//		WorldMatrixTransform = xform * BoneXForm;
	}
}
