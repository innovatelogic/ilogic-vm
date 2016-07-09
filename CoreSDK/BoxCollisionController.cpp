#include "coresdkafx.h"

REGISTER_CLASS_A(BoxCollisionController, CollisionController)
	new PropertyVector("Extends", (BYTE*)&((BoxCollisionController*)NULL)->Extends - (BYTE*)NULL, "BoxCollisionController",	"Value", READ_WRITE, CTRL_EDIT,	SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(BoxCollisionController, CollisionController);

//----------------------------------------------------------------------------------
BoxCollisionController::BoxCollisionController(const CObjectAbstract * Parent)
: Super(Parent)
, Extends(100.f, 100.f, 100.f)
{

}

//----------------------------------------------------------------------------------
BoxCollisionController::~BoxCollisionController()
{
//	UNREGISTER_EVENT(GetParent(), this, Event_OnChangePivot);
	//UNREGISTER_EVENT_PROXY_INPUT(GetParent(), this);

#ifdef PHYS_ENABLED
	GetPhysicsEngine()->ReleaseObject(this);
#endif
}

//------------------------------------------------------------------------
void BoxCollisionController::OnPropertyChanged(const char* PropertyName)
{	
	if (!strcmp(PropertyName, "Extends"))
	{
		CreatePhysActor();
	}
	Super::OnPropertyChanged(PropertyName);
}

//------------------------------------------------------------------------
void BoxCollisionController::Initialize()
{	
	assert(GetParent());

//	REGISTER_EVENT_(GetParent(), this, boost::bind<void>(&BoxCollisionController::OnChangePivot, this, _1, _2), Event_OnChangePivot);
	//REGISTER_EVENT_PROXY_INPUT(GetParent(), this, boost::bind<bool>(&BoxCollisionController::ProxyProcessInputKey, this, _1, _2));

	Super::Initialize();

	CreatePhysActor();
}

//------------------------------------------------------------------------
Vector BoxCollisionController::GetExtends()					
{ 
	return Extends; 
}

//------------------------------------------------------------------------
void BoxCollisionController::SetExtends(Vector &Ext)
{ 
	Extends = Ext; 
}

//------------------------------------------------------------------------
bool BoxCollisionController::CreatePhysActor()
{
/*	Matrix TempMat = GetParent()->GetTransformedWTM();

	if (Get2DMode()){ // modify transform
		TempMat.t.y *= -1.f;
	}

	return GetPhysicsEngine()->CreateBox(this,
								&TempMat, 
								&GetExtends(),
								GetStatic(),
								GetDesteny(),
								Get_DISABLE_GRAVITY(),
								Get_FROZEN_POS_X(),
								Get_FROZEN_POS_Y(),
								Get_FROZEN_POS_Z(),
								Get_FROZEN_ROT_X(),
								Get_FROZEN_ROT_Y(),
								Get_FROZEN_ROT_Z(),
								Get_FROZEN_POS(),
								Get_FROZEN_ROT(),
								Get_KINEMATIC());*/

	return true;
}