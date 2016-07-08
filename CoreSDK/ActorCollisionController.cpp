#include "coresdkafx.h"

REGISTER_CLASS_A(ActorCollisionController, CollisionController)
	new PropertyString("XRef", (BYTE*)&((ActorCollisionController*)NULL)->m_XRef - (BYTE*)NULL, "ActorCollisionController", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(ActorCollisionController, CollisionController);

//----------------------------------------------------------------------------------
ActorCollisionController::ActorCollisionController(const CObjectAbstract *pParent)
: Super(pParent)
{

}

//----------------------------------------------------------------------------------
ActorCollisionController::~ActorCollisionController()
{
	//GetPhysicsEngine()->ReleaseObject(this);
}

//----------------------------------------------------------------------------------
void ActorCollisionController::OnPropertyChanged(const char *pPropertyName)
{
	Super::OnPropertyChanged(pPropertyName);
}

//----------------------------------------------------------------------------------
void ActorCollisionController::Initialize()
{
	Super::Initialize();

	REGISTER_EVENT_(GetParent(), this, boost::bind<void>(&ActorCollisionController::OnChangePivot, this, _1, _2), Event_OnChangePivot);
}

//----------------------------------------------------------------------------------
bool ActorCollisionController::Load(const char *filename)
{
	bool bResult = false;

	IDrawInterface *pInterface = GetAppMain()->GetViewportManager()->GetByActor(GetParent());

	if (pInterface)
	{
		/*m_pPhysNode = GetPhysicsEngine()->CreateActorFromFile(filename,
			this,
			&pInterface->GetTransformedWTM_(), 
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
	}
	return bResult;
}

//----------------------------------------------------------------------------------
bool ActorCollisionController::CreatePhysActor()
{
	bool bResult = false;

	IDrawInterface* pInterface = GetAppMain()->GetViewportManager()->GetByActor(GetParent());

	if (pInterface)
	{
		/*m_pPhysNode = GetPhysicsEngine()->CreateActorFromFile(m_XRef.c_str(),
											this,
											&pInterface->GetTransformedWTM_(), 
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
	}
	return true;
}