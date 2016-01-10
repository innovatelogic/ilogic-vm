#include "coresdkafx.h"

REGISTER_CLASS_A(VehicleActor, ActorAllocator)
	new PropertyString("XRef", (BYTE*)&((VehicleActor*)NULL)->XRef - (BYTE*)NULL, "VehicleActor", "Value", READ_WRITE, CTRL_EDIT_RESOURCE, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(VehicleActor, ActorAllocator);

//----------------------------------------------------------------------------------------------
VehicleActor::VehicleActor(const CObjectAbstract * Parent)
: Super(Parent)
, MeshComponent(NULL)
, XRef("")
{
	NEW_OBJECT_TRANSIENT_CHILD(MeshComponent, Comp_AnimMesh, "MeshComponent", this);
	NEW_OBJECT_TRANSIENT_CHILD(CollisionObject, ActorCollisionController, "CollisionObject", this);
}

//----------------------------------------------------------------------------------------------
VehicleActor::VehicleActor(const VehicleActor & Source)
: Super(Source)
{
	if (this != &Source)
	{
		XRef = Source.XRef;
		MeshComponent = CLONE_OBJECT(Comp_AnimMesh, Source.MeshComponent, "MeshComponent", this);
		CollisionObject = CLONE_OBJECT(ActorCollisionController, Source.CollisionObject, "CollisionObject", this);
	}
}

//----------------------------------------------------------------------------------------------
VehicleActor::~VehicleActor()
{

}

//----------------------------------------------------------------------------------------------
void VehicleActor::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);

	if (!strcmp(PropertyName, "NameMesh"))
	{
//		MeshComponent->LoadMesh(StrValue);
//		CollisionObject->Load(StrValue);
		//SetBounds(MeshComponent->GetBounds());
	}
}

//----------------------------------------------------------------------------------------------
void VehicleActor::Initialize()
{
	Super::Initialize();

	MeshComponent->LoadMesh(XRef);
	//CollisionObject->Load(XRef.c_str());

	//SetBounds(MeshComponent->GetBounds());
}

//----------------------------------------------------------------------------------------------
void VehicleActor::DoDraw()
{
	//GetRenderComponent()->SetTransform(WorldMatrixTransform);
	GetRenderComponent()->DrawObject(MeshComponent);

	//if (MeshComponent->GetShowSkeleton() && MeshComponent->GetSkeleton()){
	//	MeshComponent->GetSkeleton()->RenderBones(GetTransformedWTM_());
	//}
}

//----------------------------------------------------------------------------------------------
/*void VehicleActor::DoRebuildBounds()
{
	Bounds3f MeshBBox = MeshComponent->GetBounds();

	Vector BoxPoints[] = {	Vector(MeshBBox.bound_min.x, MeshBBox.bound_min.y, MeshBBox.bound_min.z),
							Vector(MeshBBox.bound_min.x, MeshBBox.bound_min.y, MeshBBox.bound_max.z),
							Vector(MeshBBox.bound_max.x, MeshBBox.bound_min.y, MeshBBox.bound_max.z),
							Vector(MeshBBox.bound_max.x, MeshBBox.bound_min.y, MeshBBox.bound_min.z),

							Vector(MeshBBox.bound_min.x, MeshBBox.bound_max.y, MeshBBox.bound_min.z),
							Vector(MeshBBox.bound_min.x, MeshBBox.bound_max.y, MeshBBox.bound_max.z),
							Vector(MeshBBox.bound_max.x, MeshBBox.bound_max.y, MeshBBox.bound_max.z),
							Vector(MeshBBox.bound_max.x, MeshBBox.bound_max.y, MeshBBox.bound_min.z),
						}; 
 
	Bounds.SetBounds(Vector(0.f, 0.f, 0.f), Vector(0.f, 0.f, 0.f));

	Matrix MT = WorldMatrixTransform;
	MT.t = Vector(0.f, 0.f, 0.f);

	for (int Index = 0; Index < 8; ++Index)
	{
		Vector TransformMax = transform_coord(BoxPoints[Index], MT);
		Bounds.Add(TransformMax);
	}

	// some hacking scheme/ to do redesign this
	MeshComponent->SetWorldBound(Bounds3f(Vector(WorldMatrixTransform.t + Bounds.bound_min), 
											Vector(WorldMatrixTransform.t + Bounds.bound_max)));
}*/