#include "Stdafxrender.h"
#include "ObjectFactory.h"

static const bool	V_TRUE = true;

REGISTER_CLASS_PURE(CRenderObject, CActor)
	new PropertyBOOL("bCastShadows", (BYTE*)&((CRenderObject*)NULL)->bCastShadows - (BYTE*)NULL, "CRenderObject", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_TRUE),
	new PropertyBOOL("bReciveShadows", (BYTE*)&((CRenderObject*)NULL)->bReciveShadows - (BYTE*)NULL, "CRenderObject", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_TRUE),
END_REGISTER_CLASS_PURE(CRenderObject, CActor);

//----------------------------------------------------------------------------------------------
CRenderObject::CRenderObject(const CObjectAbstract *pParent)
: Super(pParent)
, RenderSDKInterface((CRenderSDK*)pParent->GetUserData(0))
, bCastShadows(V_TRUE)
, bReciveShadows(V_TRUE)
{
}

//----------------------------------------------------------------------------------------------
CRenderObject::CRenderObject(const CRenderObject &Source)
: Super(Source)
, RenderSDKInterface((CRenderSDK*)Source.GetUserData(0))
{
	if (this != &Source)
	{
		bCastShadows = Source.bCastShadows;
		bReciveShadows = Source.bReciveShadows;
	}
}

//----------------------------------------------------------------------------------------------
CRenderObject::~CRenderObject()
{
    ;//
}

//----------------------------------------------------------------------------------------------
void CRenderObject::SuperDeserializer(tinyxml2::XMLElement *xml_current_tree)
{
    oes::common_base::TClassFactory * classFactory = oes::common_base::GetClassFactoryA();

	int PlainElementIndex = 0;

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
/*void CRenderObject::OnEventNotify(unsigned int id)
{

}

//----------------------------------------------------------------------------------------------
void CRenderObject::OnEventNotify(unsigned int Id, IObjectAbstract *param)
{

}*/

//----------------------------------------------------------------------------------------------
void CRenderObject::Render() const
{

}

//----------------------------------------------------------------------------------------------
void CRenderObject::PostRender() const
{

}

//----------------------------------------------------------------------------------------------
void CRenderObject::RebuildMesh()
{
	//DWORD ThreadId = ::GetCurrentThreadId();

	//if (ThreadId != GetRenderComponent()->ThreadRenderID)
	//{
	//	GetRenderComponent()->PushToRenderStack(new Command<CRenderObject, CRenderObject>(&CRenderObject::RebuildMesh, this), true);
	//}
	//else
	//{
		DoRebuildMesh();
	//}
}

//----------------------------------------------------------------------------------------------
void CRenderObject::DoRebuildMesh()
{
	
}