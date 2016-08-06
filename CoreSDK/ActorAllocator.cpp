#include "coresdkafx.h"
#include "ObjectFactory.h"

#define COLOR_DARK_RED		0xff8b1818
#define COLOR_DARK_GREEN	0xff117900
#define COLOR_DARK_BLUE		0xff000079

namespace 
{
	volatile static oes::common_base::CAuto<ActorAllocator, CActor>
		ClassRegistrationActorAllocator("ActorAllocator", 0,  0, "ActorAllocator", "CActor");
}

//----------------------------------------------------------------------------------------------
ActorAllocator::ActorAllocator(const CObjectAbstract *parent)
: Super(parent)
, RenderSDKInterface(parent ? ((CCoreSDK*)parent->GetUserData())->GetRenderSDK() : nullptr)
, IEventInterface(parent)
, m_pAppMain(nullptr)
{
	if (parent){
		SetAppMain((CCoreSDK*)parent->GetUserData());
	}
}

//----------------------------------------------------------------------------------------------
ActorAllocator::~ActorAllocator()
{
    ;//
}

//----------------------------------------------------------------------------------------------
void ActorAllocator::SetAppMain(CCoreSDK *app) 
{ 
    m_pAppMain = app;
	SetRenderComponent(m_pAppMain->GetRenderSDK());
}

//----------------------------------------------------------------------------------------------
void ActorAllocator::SuperDeserializer(tinyxml2::XMLElement *xml_current_tree)
{
    oes::common_base::TClassFactory * classFactory = oes::common_base::GetClassFactoryA();

	XML_FOR_EACH_TREE(xml_current_tree)
	{
        oes::common_base::TClassFactory::TMapGenerator::const_iterator Iter = classFactory->m_MapGenerator.begin();

		for (;Iter != classFactory->m_MapGenerator.end(); ++Iter)
		{
			DESERIALIZE_OBJECT_BY_TYPENAME(Iter->first.c_str(), CActor);
		}
	}

	Super::SuperDeserializer(xml_current_tree);
}

//----------------------------------------------------------------------------------------------
/*void ActorAllocator::OnEventNotify(unsigned int id)
{

}

//----------------------------------------------------------------------------------------------
void ActorAllocator::OnEventNotify(unsigned int Id, IObjectAbstract *param)
{

}*/

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