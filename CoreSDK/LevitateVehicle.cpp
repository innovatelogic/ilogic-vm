#include "coresdkafx.h"

REGISTER_CLASS_A(LevitateVehicle, ActorAllocator)
	new PropertyString("XRef", (BYTE*)&((LevitateVehicle*)NULL)->XRef - (BYTE*)NULL, "LevitateVehicle",	"Value", READ_WRITE, CTRL_EDIT,	SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(LevitateVehicle, ActorAllocator);

//----------------------------------------------------------------------------------------------
LevitateVehicle::LevitateVehicle(const CObjectAbstract * Parent)
: Super(Parent)
, XRef("ufo.mat")
{
	NEW_OBJECT_TRANSIENT_CHILD(MeshComponent, Comp_StaticMesh, "MeshComponent", this);
	NEW_OBJECT_TRANSIENT_CHILD(CollisionController, ActorCollisionKinematicController, "CollisionController", this);
}

//----------------------------------------------------------------------------------------------
LevitateVehicle::LevitateVehicle(const LevitateVehicle & Source)
: Super(Source)
{
	if (this != &Source)
	{
		XRef = Source.XRef;
		MeshComponent = CLONE_OBJECT(Comp_StaticMesh, Source.MeshComponent, Source.MeshComponent->GetName(), this);
		CollisionController = CLONE_OBJECT(ActorCollisionKinematicController, Source.CollisionController, Source.CollisionController->GetName(), this);
	}
}

//----------------------------------------------------------------------------------------------
LevitateVehicle::~LevitateVehicle()
{

}

//----------------------------------------------------------------------------------------------
void LevitateVehicle::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);
	
	if (!strcmp(PropertyName, "NameMesh"))
	{
		//MeshComponent->LoadMesh(XRef.c_str());
		//CollisionController->Load(XRef);
		//SetBounds(MeshComponent->GetBounds());
	}
}

//----------------------------------------------------------------------------------------------
void LevitateVehicle::Initialize()
{
	Super::Initialize();

	//REGISTER_EVENT_PROXY_INPUT(this, this, boost::bind<bool>(&LevitateVehicle::ProxyProcessInputKey, this, _1, _2));

	//MeshComponent->LoadMesh(XRef.c_str());
	//CollisionController->Load(XRef.c_str());

	//SetBounds(MeshComponent->GetBounds());
}

//----------------------------------------------------------------------------------
void LevitateVehicle::DoDraw()
{
	GetRenderComponent()->DrawObject(MeshComponent);
}

//----------------------------------------------------------------------------------
/*void LevitateVehicle::DoRebuildBounds()
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

//----------------------------------------------------------------------------------------------
/*void LevitateVehicle::SetPivot(const Matrix& matrix) const
{
	Super::SetPivot(matrix);

 	if (IsFocused() && GetAppMain()->GetCameraManager())
 	{
 		const CCamera * Camera = GetAppMain()->GetCameraManager()->GetActiveCamera();
 
 		if (Camera)
 		{
			Vector *V = GetPhysicsEngine()->GetLinearVelocity(CollisionController);
			V->y = 0.f;
			float L = V->Length();

			L = (L > 50.f) ? 50 : L;

 			const_cast<CCamera*>(Camera)->SetPosition(matrix.t - Vector(0.f, 0.f, 350.f + L));
 		}
 	}
}*/
/*
//----------------------------------------------------------------------------------------------
bool LevitateVehicle::ProxyProcessInputKey(const CActor * Sender, const EventInput &InputData)
{
	switch (InputData.Key)
	{
	case KEY_LSHIFT:
		//MoveSpeed = (InputData.State == KEY_Pressed) ? 10.f : 5.f;
		//GetPhisicsEngine()->SetCharacterMovementSpeed(CharacterController, CharacterController->GetMoveSpeed());
		break;

	case KEY_UP:
		GetPhysicsEngine()->AddForce(CollisionController, &Vector(0.f, 100.f, 0.f), PHYS_ACCELERATION);
		break;

	case KEY_DOWN:
		GetPhysicsEngine()->AddForce(CollisionController, &Vector(0.f, -100.f, 0.f), PHYS_ACCELERATION);
		break;

	case KEY_RIGHT:
		GetPhysicsEngine()->AddForce(CollisionController, &Vector(100.f, 0.f, 0.f), PHYS_ACCELERATION);
		break;

	case KEY_LEFT:
		GetPhysicsEngine()->AddForce(CollisionController, &Vector(-100.f, 0.f, 0.f), PHYS_ACCELERATION);
		break;

	case KEY_SPACE:
		break;
	};
	return true;
}
*/