#include "coresdkafx.h"

REGISTER_CLASS_A(CSphereCollisionController, CollisionController)
	new PropertyFLOAT("Radius", (BYTE*)&((CSphereCollisionController*)NULL)->m_fRadius - (BYTE*)NULL, "CSphereCollisionController", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(CSphereCollisionController, CollisionController);

//------------------------------------------------------------------------
CSphereCollisionController::CSphereCollisionController(const CObjectAbstract *pParent)
: Super(pParent)
, m_fRadius(50.f)
{

}

//------------------------------------------------------------------------
CSphereCollisionController::~CSphereCollisionController()
{
	UNREGISTER_EVENT(GetParent(), this, Event_OnChangePivot);
	//UNREGISTER_EVENT_PROXY_INPUT(GetParent(), this);

//	GetPhysicsEngine()->ReleaseObject(this);
}

//------------------------------------------------------------------------
void CSphereCollisionController::OnPropertyChanged(const char* PropertyName)
{	
	if (!strcmp(PropertyName, "Radius"))
	{
		CreatePhysActor();
	}
	Super::OnPropertyChanged(PropertyName);
}

//------------------------------------------------------------------------
void CSphereCollisionController::Initialize()
{	
	assert(GetParent());

	REGISTER_EVENT_(GetParent(), this, boost::bind<void>(&CSphereCollisionController::OnChangePivot, this, _1, _2), Event_OnChangePivot);
	//REGISTER_EVENT_PROXY_INPUT(GetParent(), this, boost::bind<bool>(&SphereCollisionController::ProxyProcessInputKey, this, _1, _2));

	Super::Initialize();

	//CreatePhysActor();
}

//------------------------------------------------------------------------
bool CSphereCollisionController::CreatePhysActor(CContactSensorCallbackBt *pCallback /*= 0*/)
{
	bool bResult = false;

	IDrawInterface* pInterface = GetAppMain()->GetViewportManager()->GetByActor(GetParent());

	if (pInterface)
	{
		/*SSphereBodyDesc Desc;
		Desc.fRadius = GetRadius();
		Desc.bStatic = GetStatic();
		Desc.fDensity = GetDesteny();
		Desc.DISABLE_GRAVITY = Get_DISABLE_GRAVITY();
		Desc.FROZEN_POS_X = Get_FROZEN_POS_X();
		Desc.FROZEN_POS_Y = Get_FROZEN_POS_Y();
		Desc.FROZEN_POS_Z = Get_FROZEN_POS_Z();
		Desc.FROZEN_ROT_X = Get_FROZEN_ROT_X();
		Desc.FROZEN_ROT_Y = Get_FROZEN_ROT_Y();
		Desc.FROZEN_ROT_Z = Get_FROZEN_ROT_Z();
		Desc.FROZEN_POS = Get_FROZEN_POS();
		Desc.FROZEN_ROT = Get_FROZEN_ROT();
		Desc.KINEMATIC = Get_KINEMATIC();
		Desc.pCallback = pCallback;
	
		m_pPhysNode = GetPhysicsEngine()->CreateSphere(this, &pInterface->GetTransformedWTM_(), Desc);*/
	}
	return (m_pPhysNode != 0);
}