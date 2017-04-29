#include "coresdkafx.h"
#include "ObjectFactory.h"

#define COLOR_DARK_RED		0xff8b1818
#define COLOR_DARK_GREEN	0xff117900
#define COLOR_DARK_BLUE		0xff000079

namespace 
{
	volatile static oes::rflex::CAuto<ActorAllocator, CActor>
		ClassRegistrationActorAllocator("ActorAllocator", 0, "ActorAllocator", "CActor");
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
    oes::rflex::TClassFactory * classFactory = oes::rflex::GetClassFactoryA();

	XML_FOR_EACH_TREE(xml_current_tree)
	{
        oes::rflex::TClassFactory::TMapGenerator::const_iterator Iter = classFactory->m_MapGenerator.begin();

		for (;Iter != classFactory->m_MapGenerator.end(); ++Iter)
		{
			DESERIALIZE_OBJECT_BY_TYPENAME(Iter->first.c_str(), CActor);
		}
	}

	Super::SuperDeserializer(xml_current_tree);
}