#include "coresdkafx.h"

#define COLOR_DARK_RED		0xff8b1818
#define COLOR_DARK_GREEN	0xff117900
#define COLOR_DARK_BLUE		0xff000079

namespace 
{
	volatile static NObjectFactory::CAuto<ActorAllocator, CActor>
		ClassRegistrationActorAllocator("ActorAllocator", 0,  0, "ActorAllocator", "CActor");
}

//----------------------------------------------------------------------------------------------
ActorAllocator::ActorAllocator(const CObjectAbstract *Parent)
: Super(Parent)
, RenderSDKInterface(Parent ? ((CCoreSDK*)Parent->GetUserData())->GetRenderSDK() : 0)
, AppMain(NULL)
{
	if (Parent){
		SetAppMain((CCoreSDK*)Parent->GetUserData());
	}
}

//----------------------------------------------------------------------------------------------
ActorAllocator::ActorAllocator(const ActorAllocator& Source)
: Super(Source)
, RenderSDKInterface(((CCoreSDK*)Source.GetUserData())->GetRenderSDK())
{
	if (this != &Source)
	{
		AppMain = Source.AppMain;

		// copy childs
		TVecActorChild::const_iterator Iter = Source.m_ChildNodes.begin();

		while (Iter != Source.m_ChildNodes.end())
		{
			if (!(*Iter)->IsTransient())
			{
				CActor * NewObject = CLONE_OBJECT(CActor, *Iter, (*Iter)->GetName(), this);

				if (NewObject)
				{
					NewObject->SetParent(this);
					AddChildNode(NewObject);
				}
			}
			Iter++;
		}
	}
}

//----------------------------------------------------------------------------------------------
ActorAllocator::~ActorAllocator()
{
    ;//
}

//----------------------------------------------------------------------------------------------
void ActorAllocator::SetAppMain(CCoreSDK * App) 
{ 
	AppMain = App;
	SetRenderComponent(AppMain->GetRenderSDK());
}

//----------------------------------------------------------------------------------------------
/*IPhysicsEngine* ActorAllocator::GetPhysicsEngine() const
{
	return  GetAppMain()->GetPhysicsEngine();
}*/

//----------------------------------------------------------------------------------------------
// ScriptDriver* ActorAllocator::GetObjectScriptDriver()
// {
//    return NULL;//AppMain->GetScriptDriver();
// }

//----------------------------------------------------------------------------------------------
void ActorAllocator::SuperDeserializer(tinyxml2::XMLElement *xml_current_tree)
{
	NObjectFactory::TClassFactory * classFactory = GetClassFactoryA();

	int PlainElementIndex = 0;

	XML_FOR_EACH_TREE(xml_current_tree)
	{
		NObjectFactory::TClassFactory::TMapGenerator::const_iterator Iter = classFactory->m_MapGenerator.begin();

		for (;Iter != classFactory->m_MapGenerator.end(); ++Iter)
		{
			DESERIALIZE_OBJECT_BY_TYPENAME(Iter->first.c_str(), CActor);
		}
	}

	Super::SuperDeserializer(xml_current_tree);
}

//----------------------------------------------------------------------------------------------
bool ActorAllocator::DoEventReleased(const MouseInputData& InputData)
{
	//UserStartMousePosition = Vector(0.f, 0.f, 0.f);

	return Super::DoEventReleased(InputData);
}


//----------------------------------------------------------------------------------------------
/*
Vector& ActorAllocator::GlobalToLocalTransform(Vector& OutLocalPoint, const Vector& InGlobalPoint) const
{
	// build global space matrix
	Matrix WTM = CalcWorldTransform();

	Vector UnTransformedLocalPosition = InGlobalPoint - WTM.t;

	// calc projection to local axis
	Vector AxisX = WTM._row0;
	Vector AxisY = WTM._row1;
	Vector AxisZ = WTM._row2;

	AxisX.normalize();
	AxisY.normalize();
	AxisZ.normalize();

	Matrix IWTM;	
	invert(IWTM, WTM);

	OutLocalPoint = Vector(UnTransformedLocalPosition.Dot(AxisX) * IWTM._row0.Length(),
							UnTransformedLocalPosition.Dot(AxisY) * IWTM._row1.Length(),
							UnTransformedLocalPosition.Dot(AxisZ) * IWTM._row2.Length());
	return OutLocalPoint;
}

//----------------------------------------------------------------------------------------------
Matrix ActorAllocator::CalcWorldTransform() const
{
	Matrix OutMatrix;

	std::vector<const CActor*> TmpActorList;

	CActor * IterActor = GetParent();

	while (IterActor || dynamic_cast<ViewportInterface*>(IterActor)) // do not allow top level root processing
	{																 // use it as origin for child nodes
		TmpActorList.push_back(IterActor);
		IterActor = IterActor->GetParent();
	}

	// build global space matrix
	Matrix WTM;
	for (std::vector<const CActor*>::reverse_iterator Iter = TmpActorList.rbegin(); Iter != TmpActorList.rend(); ++Iter)
	{
		OutMatrix = OutMatrix * (*Iter)->GetPivot();
	}

	return OutMatrix;
}*/

//----------------------------------------------------------------------------------------------
bool ActorAllocator::IsBrushActivated()
{
	bool bResult = false;
	return bResult;
}