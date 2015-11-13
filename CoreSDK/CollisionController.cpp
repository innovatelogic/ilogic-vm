#include "coresdkafx.h"

static const bool	V_FALSE = false;
static const float	fDestenyDef = 0.1f;
static const Matrix DefMatrixTransform(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f);

REGISTER_CLASS_A(CollisionController, BaseModificator)
	new PropertyBOOL("bStatic",	(BYTE*)&((CollisionController*)NULL)->bStatic - (BYTE*)NULL, "CollisionController", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyFLOAT("Desteny", (BYTE*)&((CollisionController*)NULL)->Desteny - (BYTE*)NULL, "CollisionController", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fDestenyDef),
	new PropertyBOOL("b2DMode", (BYTE*)&((CollisionController*)NULL)->b2DMode - (BYTE*)NULL, "CollisionController", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyBOOL("DISABLE_GRAVITY",	(BYTE*)&((CollisionController*)NULL)->DISABLE_GRAVITY - (BYTE*)NULL, "CollisionController",	"Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyBOOL("FROZEN_POS_X", (BYTE*)&((CollisionController*)NULL)->FROZEN_POS_X - (BYTE*)NULL, "CollisionController", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyBOOL("FROZEN_POS_Y", (BYTE*)&((CollisionController*)NULL)->FROZEN_POS_Y - (BYTE*)NULL, "CollisionController", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyBOOL("FROZEN_POS_Z", (BYTE*)&((CollisionController*)NULL)->FROZEN_POS_Z - (BYTE*)NULL, "CollisionController", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyBOOL("FROZEN_ROT_X", (BYTE*)&((CollisionController*)NULL)->FROZEN_ROT_X - (BYTE*)NULL, "CollisionController", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyBOOL("FROZEN_ROT_Y", (BYTE*)&((CollisionController*)NULL)->FROZEN_ROT_Y - (BYTE*)NULL, "CollisionController", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyBOOL("FROZEN_ROT_Z", (BYTE*)&((CollisionController*)NULL)->FROZEN_ROT_Z - (BYTE*)NULL, "CollisionController", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyBOOL("FROZEN_POS", (BYTE*)&((CollisionController*)NULL)->FROZEN_POS - (BYTE*)NULL, "CollisionController", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyBOOL("FROZEN_ROT", (BYTE*)&((CollisionController*)NULL)->FROZEN_ROT - (BYTE*)NULL, "CollisionController", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyBOOL("KINEMATIC", (BYTE*)&((CollisionController*)NULL)->KINEMATIC - (BYTE*)NULL, "CollisionController", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyMatrix("LTM", (BYTE*)&((CollisionController*)NULL)->m_LTM - (BYTE*)NULL, "CollisionController", "Value", CTRL_MATRIX16, READ_WRITE, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &DefMatrixTransform),
END_REGISTER_CLASS(CollisionController, BaseModificator);

//----------------------------------------------------------------------------------
CollisionController::CollisionController(const CObjectAbstract* Parent)
: Super(Parent)
, bStatic(V_FALSE)
, Desteny(fDestenyDef)
, b2DMode(V_FALSE)
, DISABLE_GRAVITY(V_FALSE)
, FROZEN_POS_X(V_FALSE)
, FROZEN_POS_Y(V_FALSE)
, FROZEN_POS_Z(V_FALSE)
, FROZEN_ROT_X(V_FALSE)
, FROZEN_ROT_Y(V_FALSE)
, FROZEN_ROT_Z(V_FALSE)
, FROZEN_POS(V_FALSE)
, FROZEN_ROT(V_FALSE)
, KINEMATIC(V_FALSE)
, m_LTM(DefMatrixTransform)
, m_pPhysNode(0)
{

}
//----------------------------------------------------------------------------------
CollisionController::CollisionController(const CollisionController &Source)
: Super(Source)
{
	if (this != &Source)
	{
		bStatic = Source.bStatic;
		Desteny = Source.Desteny;
		b2DMode = Source.b2DMode;
		DISABLE_GRAVITY = Source.DISABLE_GRAVITY;
		FROZEN_POS_X = Source.FROZEN_POS_X;
		FROZEN_POS_Y = Source.FROZEN_POS_Y;
		FROZEN_POS_Z = Source.FROZEN_POS_Z;
		FROZEN_ROT_X = Source.FROZEN_ROT_X;
		FROZEN_ROT_Y = Source.FROZEN_ROT_Y;
		FROZEN_ROT_Z = Source.FROZEN_ROT_Z;
		FROZEN_POS = Source.FROZEN_POS;
		FROZEN_ROT = Source.FROZEN_ROT;
		KINEMATIC = Source.KINEMATIC;
	}
}
//----------------------------------------------------------------------------------
CollisionController::~CollisionController()
{
	if (m_pPhysNode){
		//GetPhysicsEngine()->ReleaseObject(m_pPhysNode);
	}
}

//------------------------------------------------------------------------
void CollisionController::OnPropertyChanged(const char* PropertyName)
{	
	if (!strcmp(PropertyName, "bStatic") ||
		!strcmp(PropertyName, "Desteny") ||
		!strcmp(PropertyName, "b2DMode") ||
		!strcmp(PropertyName, "DISABLE_GRAVITY") ||
		!strcmp(PropertyName, "FROZEN_POS_X") || 
		!strcmp(PropertyName, "FROZEN_POS_Y") || 
		!strcmp(PropertyName, "FROZEN_POS_Z") ||
		!strcmp(PropertyName, "FROZEN_ROT_X") || 
		!strcmp(PropertyName, "FROZEN_ROT_Y") ||
		!strcmp(PropertyName, "FROZEN_ROT_Z") ||
		!strcmp(PropertyName, "FROZEN_POS") ||
		!strcmp(PropertyName, "FROZEN_ROT") ||
		!strcmp(PropertyName, "KINEMATIC"))
	{
		//CreatePhysActor();
	}
	Super::OnPropertyChanged(PropertyName);
}

//------------------------------------------------------------------------
void CollisionController::OnChangePivot(const CActor * Sender, ESystemEventID EventId)
{
	Matrix TempMat;// = GetParent()->GetTransformedWTM();

	//GetPhysicsEngine()->SetActorPosition(this, &TempMat);
}

//------------------------------------------------------------------------
void CollisionController::Enable(bool bFlag)
{
	if (m_pPhysNode)
	{
		//GetPhysicsEngine()->SetActorEnable(m_pPhysNode, bFlag);
	}
}

//------------------------------------------------------------------------
void CollisionController::ContactTest()
{
	if (m_pPhysNode)
	{
		//GetPhysicsEngine()->ContactTest(m_pPhysNode);
	}
}

//------------------------------------------------------------------------
void CollisionController::SetWorldPos(const Matrix &matrix)
{
	if (m_pPhysNode)
	{
		//GetPhysicsEngine()->SetActorPositionNode(m_pPhysNode, &matrix);
	}
	
}