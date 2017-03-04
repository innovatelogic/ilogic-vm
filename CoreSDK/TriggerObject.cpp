#include "coresdkafx.h"

REGISTER_CLASS_A(TriggerObject, ActorAllocator)
	new oes::rflex::PropertyVector("Extends", (BYTE*)&((TriggerObject*)NULL)->Extends - (BYTE*)NULL, "TriggerObject", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new oes::rflex::PropertyBOOL("OnEnter",	(BYTE*)&((TriggerObject*)NULL)->OnEnter - (BYTE*)NULL, "TriggerObject",	"Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new oes::rflex::PropertyBOOL("OnStay", (BYTE*)&((TriggerObject*)NULL)->OnStay - (BYTE*)NULL, "TriggerObject", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new oes::rflex::PropertyBOOL("OnLeave", (BYTE*)&((TriggerObject*)NULL)->OnLeave - (BYTE*)NULL, "TriggerObject",	"Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(TriggerObject, ActorAllocator);

//----------------------------------------------------------------------------------------------
TriggerObject::TriggerObject(const CObjectAbstract * Parent)
: Super(Parent)
, Extends(10.f, 25.f, 10.f)
, OnEnter(true)
, OnStay(false)
, OnLeave(false)
{
	#if 0
NEW_OBJECT_TRANSIENT(ActionEvent, ActionTrigger, "ActionEvent");
	//NEW_OBJECT_TRANSIENT(ActionOnStay, ActionBase, "ActionOnStay");
	//NEW_OBJECT_TRANSIENT(ActionOnLeave, ActionBase, "ActionOnLeave");

	ActionEvent->SetOwnerActor(this);
	//ActionOnStay->SetOwnerActor(this);
	//ActionOnLeave->SetOwnerActor(this);

	ScriptDriver * Driver = GetAppMain()->GetScriptDriver();

	Driver->RegisterAction(ActionEvent);
//	Driver->RegisterAction(ActionOnStay);
//	Driver->RegisterAction(ActionOnLeave);
#endif
}

//----------------------------------------------------------------------------------------------
TriggerObject::~TriggerObject()
{
#if 0
//	UNREGISTER_EVENT(this, this, Event_OnChangePivot);

	//GetPhysicsEngine()->ReleaseObject(this);

	ScriptDriver * Driver = GetAppMain()->GetScriptDriver();
	Driver->UnRegisterAction(ActionEvent);

	ActionEvent->ProcessRelease(true);
	ActionEvent = 0;
#endif
}

//----------------------------------------------------------------------------------------------
/*void TriggerObject::RegisterProperties()
{
	Super::RegisterProperties();

//	REGISTER_PROPERTY_COMMAND("ActionEditor", this, TriggerObject, TriggerObject::RegisterProperties);
}*/

//----------------------------------------------------------------------------------------------
void TriggerObject::OnPropertyChanged(const char* PropertyName)
{
	if (!strcmp(PropertyName, "OnEnter") ||
		!strcmp(PropertyName, "OnStay") ||
		!strcmp(PropertyName, "OnLeave"))
	{
		CreatePhysActor();
	}
	Super::OnPropertyChanged(PropertyName);
}

//----------------------------------------------------------------------------------------------
void TriggerObject::SuperDeserializer(tinyxml2::XMLElement *xml_current_tree)
{
	Super::SuperDeserializer(xml_current_tree);

	#if 0
XML_FOR_EACH_TREE(xml_current_tree)
	{
		XML_CHECK_NODE("ActionTrigger")
		{
			XML_DEF_ATTRIBUTES_NODE(Value);

			ValueParser ValueStore(Value);

			if (ValueStore.IsValue("Name"))
			{
				std::string ValueName = ValueStore.GetValueString("Name");

				if (ValueName == "ActionEvent"){
					ActionEvent->SuperDeserializer(xml_current_tree);
				}
			}
		}
	}
#endif
}

//----------------------------------------------------------------------------------------------
void TriggerObject::SuperSerializer(std::ofstream &Stream, bool bSaveAsExternal /*= false*/)
{
#if 0
	std::string OutValue;

	GetValueString(OutValue);

	Stream << "<" << GetType() << OutValue << ">";

	if (!GetExternal() || bSaveAsExternal)
	{
		TVecActorChildIterator IterActor = m_ChildNodes.begin();

		while (IterActor != m_ChildNodes.end())
		{
			(*IterActor)->SuperSerializer(Stream);
			++IterActor;
		}

		ActionEvent->SuperSerializer(Stream, bSaveAsExternal);
	}
	Stream << "</" << GetType() << ">";
#endif
}

//----------------------------------------------------------------------------------------------
void TriggerObject::Initialize()
{
	Super::Initialize();

	//ActionEvent->Initialize();
	
	CreatePhysActor();
}

//----------------------------------------------------------------------------------------------
void TriggerObject::DoDraw()
{
	Vector  Points[8];    // corners of the box
	Bounds3f Bbox(Extends * -0.5f, Extends * 0.5f);

	Points[0] = Vector(Bbox.bound_min.x, Bbox.bound_min.y, Bbox.bound_min.z); // xyz
	Points[1] = Vector(Bbox.bound_max.x, Bbox.bound_min.y, Bbox.bound_min.z); // Xyz
	Points[2] = Vector(Bbox.bound_min.x, Bbox.bound_max.y, Bbox.bound_min.z); // xYz
	Points[3] = Vector(Bbox.bound_max.x, Bbox.bound_max.y, Bbox.bound_min.z); // XYz
	Points[4] = Vector(Bbox.bound_min.x, Bbox.bound_min.y, Bbox.bound_max.z); // xyZ
	Points[5] = Vector(Bbox.bound_max.x, Bbox.bound_min.y, Bbox.bound_max.z); // XyZ
	Points[6] = Vector(Bbox.bound_min.x, Bbox.bound_max.y, Bbox.bound_max.z); // xYZ
	Points[7] = Vector(Bbox.bound_max.x, Bbox.bound_max.y, Bbox.bound_max.z); // XYZ

	for (int Index = 0; Index < 8; ++Index)
	{
		Vector Temp;
		//transform_coord(Temp, Points[Index], WorldMatrixTransform);
		Points[Index] = Temp;
	}

	GetRenderComponent()->DrawLine(Points[0], Points[2], 0xff00ff00);
	GetRenderComponent()->DrawLine(Points[2], Points[3], 0xff00ff00);
	GetRenderComponent()->DrawLine(Points[3], Points[1], 0xff00ff00);
	GetRenderComponent()->DrawLine(Points[1], Points[0], 0xff00ff00);

	GetRenderComponent()->DrawLine(Points[4], Points[6], 0xff00ff00);
	GetRenderComponent()->DrawLine(Points[6], Points[7], 0xff00ff00);
	GetRenderComponent()->DrawLine(Points[7], Points[5], 0xff00ff00);
	GetRenderComponent()->DrawLine(Points[5], Points[4], 0xff00ff00);

	GetRenderComponent()->DrawLine(Points[0], Points[4], 0xff00ff00);
	GetRenderComponent()->DrawLine(Points[1], Points[5], 0xff00ff00);
	GetRenderComponent()->DrawLine(Points[2], Points[6], 0xff00ff00);
	GetRenderComponent()->DrawLine(Points[3], Points[7], 0xff00ff00);
}

//----------------------------------------------------------------------------------------------
void TriggerObject::OnChangePivot(const CActor * Sender, ESystemEventID EventId)
{
	CreatePhysActor();
}

//----------------------------------------------------------------------------------------------
void TriggerObject::CreatePhysActor()
{
	unsigned int TriggerFlag = 0;

	if (OnEnter){
		TriggerFlag |= PHYS_TRIGGER_ON_ENTER;
	}
	if (OnStay){
		TriggerFlag |= PHYS_TRIGGER_ON_STAY;
	}
	if (OnLeave){
		TriggerFlag |= PHYS_TRIGGER_ON_LEAVE;
	}

	//GetPhysicsEngine()->CreateBoxTrigger(this, GetTransformedWTM_(), GetExtends(), (TriggerShapeFlag)TriggerFlag);
}

//----------------------------------------------------------------------------------------------
void TriggerObject::OnTriggerEvent(CActor * OtherActor, TriggerShapeFlag Flag)
{
	#if 0
if (OnEnter && Flag == PHYS_TRIGGER_ON_ENTER){
		ActionEvent->Execute(OtherActor);
	}

	if (OnStay && Flag == PHYS_TRIGGER_ON_STAY){
		ActionEvent->Execute(OtherActor);
	}

	if (OnLeave && Flag == PHYS_TRIGGER_ON_LEAVE){
		ActionEvent->Execute(OtherActor);
	}
#endif
}