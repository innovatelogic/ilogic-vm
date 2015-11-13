#include "BoxCharacterController.h"
#include "RenderSDK.h"

REGISTER_CLASS_A(BoxCharacterController, BaseModificator)
	new PropertyVector("Extends", (BYTE*)&((BoxCharacterController*)NULL)->Extends - (BYTE*)NULL, "TriggerObject", "Value", READ_WRITE,	CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyFLOAT("UpForce", (BYTE*)&((BoxCharacterController*)NULL)->UpForce - (BYTE*)NULL, "TriggerObject", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyFLOAT("MoveSpeed", (BYTE*)&((BoxCharacterController*)NULL)->MoveSpeed - (BYTE*)NULL, "TriggerObject", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(BoxCharacterController, BaseModificator);

//------------------------------------------------------------------------
BoxCharacterController::BoxCharacterController(const CObjectAbstract * Parent /*= NULL*/)
: Super(Parent)
, Extends(10.f, 25.f, 10.f)
, UpForce(85.f)
, MoveSpeed(25.0f)
{
}

//------------------------------------------------------------------------
BoxCharacterController::BoxCharacterController(const BoxCharacterController &Source)
: Super(Source)
{
	if (this != &Source)
	{
		Extends = Source.Extends;
		UpForce = Source.UpForce;
		MoveSpeed = Source.MoveSpeed;
	}
}

//------------------------------------------------------------------------
BoxCharacterController::~BoxCharacterController()
{
	assert(GetParent());

	UNREGISTER_EVENT(GetParent(), this, Event_OnChangePivot);
	//UNREGISTER_EVENT_PROXY_INPUT(GetParent(), this);

	//GetPhysicsEngine()->ReleaseObject(this);
}

//------------------------------------------------------------------------
void BoxCharacterController::OnPropertyChanged(const char* PropertyName)
{
	if (!strcmp(PropertyName, "Extends") ||
		!strcmp(PropertyName, "UpForce"))
	{
		CreatePhysActor();
	}
	Super::OnPropertyChanged(PropertyName);
}

//------------------------------------------------------------------------
void BoxCharacterController::Initialize()
{	
	Super::Initialize();

	REGISTER_EVENT_(GetParent(), this, boost::bind<void>(&BoxCharacterController::OnChangePivot, this, _1, _2), Event_OnChangePivot);

	CreatePhysActor();
}

//------------------------------------------------------------------------
void BoxCharacterController::OnChangePivot(const CActor * Sender, ESystemEventID EventId)
{	
	//GetPhysicsEngine()->SetControllerPosition(this, &GetParent()->GetTransformedWTM().t);
}

//------------------------------------------------------------------------
/*void BoxCharacterController::SetPivot(const Matrix& matrix) const
{
	assert(GetParent());

	Matrix TempMat = matrix;

	//if (Get2DMode()){
	//	TempMat.t.y *= -1.f;
	//}
	GetParent()->SetPivot(TempMat);
	
	Super::SetPivot(matrix);
}*/

//------------------------------------------------------------------------
void BoxCharacterController::CreatePhysActor()
{
//	Matrix TempMat = GetParent()->GetTransformedWTM();

//	GetPhysicsEngine()->CreateBoxController(this, &TempMat.t, &GetExtends());
}
