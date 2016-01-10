#include "coresdkafx.h"

REGISTER_CLASS_A(CPawn, ActorAllocator)
	new PropertyString("XRef", (BYTE*)&((CPawn*)NULL)->XRef - (BYTE*)NULL, "CPawn", "Value", READ_WRITE, CTRL_EDIT,	true, false),
END_REGISTER_CLASS(CPawn, ActorAllocator);

//----------------------------------------------------------------------------------------------
CPawn::CPawn(const CObjectAbstract* Parent)
: Super(Parent)
//, MeshComponent(NULL)
, XRef("bandersha.mat")
, bWalk(false)
, blend_weight(0.f)
{
	SetStates = new SetAnimState();
	assert(SetStates);

	//NEW_OBJECT_TRANSIENT_CHILD(MeshComponent, Comp_AnimMesh, "MeshComponent", this);
	NEW_OBJECT_TRANSIENT_CHILD(MaterialEffect, Comp_MaterialEffect, "UICompDrawImage_MaterialEffect", this);
	NEW_OBJECT_TRANSIENT_CHILD(CharacterController, BoxCharacterController, "CharacterController", this);
	NEW_OBJECT_TRANSIENT_CHILD(HeadTracker, DummyObject, "HeadTracker", this);
	NEW_OBJECT_TRANSIENT_CHILD(PawnCamera, CCamera, "PawnCamera", this);

}

//----------------------------------------------------------------------------------------------
CPawn::~CPawn()
{
	delete SetStates;
}

//----------------------------------------------------------------------------------------------
void CPawn::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);

	if (!strcmp(PropertyName, "XRef"))
	{
		//MeshComponent->LoadMesh(GetXRef());
		//SetBounds(MeshComponent->GetBounds());
	}
}

//----------------------------------------------------------------------------------------------
void CPawn::Initialize()
{
	Super::Initialize();

	//REGISTER_EVENT_PROXY_INPUT(this, this, boost::bind<bool>(&CPawn::ProxyProcessInputKey, this, _1, _2));

	//MeshComponent->LoadMesh(GetXRef());
	//SetBounds(MeshComponent->GetBounds());

	GetAppMain()->GetUpdateManager()->RegisterActor(this);
}

//----------------------------------------------------------------------------------------------
void CPawn::PreRelease()
{
	GetAppMain()->GetUpdateManager()->UnRegisterActor(this);
	Super::PreRelease();
}

//----------------------------------------------------------------------------------------------
void CPawn::DoDraw()
{
//	GetRenderComponent()->SetTransform(WorldMatrixTransform);

// 	GetRenderComponent()->PushToRenderStack(new RenderChunk_AdvanceAnim<Comp_AnimMesh, SetAnimState>(MeshComponent, *SetStates, INDEX_NONE));
// 	GetRenderComponent()->DrawObject(MeshComponent);
// 
// 	if (MeshComponent->GetShowSkeleton() && MeshComponent->GetSkeleton()){
// 		MeshComponent->GetSkeleton()->RenderBones(GetTransformedWTM());
// 	}
}

//----------------------------------------------------------------------------------------------
/*void CPawn::DoRebuildBounds()
{
	Bounds3f MeshBBox = MeshComponent->GetBounds();

	Vector BoxPoints[] = { 
		Vector(MeshBBox.bound_min.x, MeshBBox.bound_min.y, MeshBBox.bound_min.z),
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
/*void CPawn::SetPivot(const Matrix& matrix) const
{
	Super::SetPivot(matrix);

	PawnCamera->SetPosition(matrix.t + Vector(0.f, 0.f, -70.f));
}
//----------------------------------------------------------------------------------------------
bool CPawn::ProxyProcessInputKey(const CActor * Sender, const EventInput &InputData)
{
	switch (InputData.Key)
	{
	case KEY_LSHIFT:
		//MoveSpeed = (InputData.State == KEY_Pressed) ? 10.f : 5.f;
		GetPhysicsEngine()->SetCharacterMovementSpeed(CharacterController, CharacterController->GetMoveSpeed());
		break;

	case KEY_UP:
		{
			Vector t = (InputData.State == KEY_Pressed) ? Vector(0.f, 0.f, -1.f) : Vector(0.f, 0.f, 0.f);
			GetPhysicsEngine()->SetCharacterMovement(CharacterController, &t, CharacterController->GetMoveSpeed());
			bWalk = (InputData.State == KEY_Pressed);
		}break;

	case KEY_DOWN:
		{
			Vector t = (InputData.State == KEY_Pressed) ? Vector(0.f, 0.f, 1.f) : Vector(0.f, 0.f, 0.f);
			GetPhysicsEngine()->SetCharacterMovement(CharacterController, &t, CharacterController->GetMoveSpeed());
			bWalk = (InputData.State == KEY_Pressed);
		}break;

	case KEY_RIGHT:
		{
			Vector t = (InputData.State == KEY_Pressed) ? Vector(1.f, 0.f, 0.f) : Vector(0.f, 0.f, 0.f);
			GetPhysicsEngine()->SetCharacterMovement(CharacterController, &t, CharacterController->GetMoveSpeed());
			bWalk = (InputData.State == KEY_Pressed);
		}break;

	case KEY_LEFT:
		{
			Vector t = (InputData.State == KEY_Pressed) ? Vector(-1.f, 0.f, 0.f) : Vector(0.f, 0.f, 0.f);
			GetPhysicsEngine()->SetCharacterMovement(CharacterController, &t, CharacterController->GetMoveSpeed());
			bWalk = (InputData.State == KEY_Pressed);
		}break;

	case KEY_SPACE:
		if (InputData.State == KEY_Pressed){
			GetPhysicsEngine()->CharacterJump(CharacterController, CharacterController->GetUpForce());
		}
		break;
	};
	return true;
}

//----------------------------------------------------------------------------------------------
void CPawn::DoTick(float DeltaTime)
{
	Super::DoTick(DeltaTime);

	SetStates->States.clear();

	if (bWalk)
	{
		blend_weight += 0.05f;
		if (blend_weight > 1.f){
			blend_weight = 1.f;
		}
	}
	else
	{
		blend_weight -= 0.05f;
		if (blend_weight < 0.f){
			blend_weight = 0.f;
		}
	}

	if (1.f - blend_weight > 0.f)
	{
		AnimState StateIdle("idle", DeltaTime * 10.f, 1.f - blend_weight);
		// 
		// 
		// 
		// //  		StateIdle.BonesMask.push_back(5);
		// //  		StateIdle.BonesMask.push_back(4);
		// //  		StateIdle.BonesMask.push_back(3);
		// //  		StateIdle.BonesMask.push_back(2);
		// //  		StateIdle.BonesMask.push_back(6);
		// //  		StateIdle.BonesMask.push_back(7);
		// //  		StateIdle.BonesMask.push_back(8);
		// //  		StateIdle.BonesMask.push_back(9);
		// 
		SetStates->States.push_back(StateIdle);
	}

	AnimState StateWalk("walk", DeltaTime * 10.f, blend_weight);
	SetStates->States.push_back(StateWalk);

	//AnimState StateTrack(1.f - blend_weight > 0.f ? "idle" : "walk", 0.1f, blend_weight);

	// 	StateTrack.bTrackTarget = true;
	//	StateTrack.PointTarget = HeadTracker->GetPosition();
	// 	StateTrack.BonesMask.push_back(10);
	// 	StateTrack.BonesMask.push_back(11);

	//SetStates.States.push_back(StateTrack);
	// 	StateWalk.BonesMask.clear();
	// 	StateWalk.BonesMask.push_back(17);
	// 	StateWalk.BonesMask.push_back(16);
	// 	StateWalk.BonesMask.push_back(15);
	// 	StateWalk.BonesMask.push_back(14);
	// 	StateWalk.BonesMask.push_back(13);
	// 	StateWalk.BonesMask.push_back(12);
}*/