#include "coresdkafx.h"

REGISTER_CLASS(Explorer, ActorAllocator);

//----------------------------------------------------------------------------------
Explorer::Explorer(const CObjectAbstract *pParent)
: Super(nullptr)
, BufferDataObject(NULL)
{
	SetTransient(true);

	SetUserData(pParent->GetUserData());
	SetUserData(pParent->GetUserData(0), 0);

	SetAppMain((CCoreSDK*)GetUserData());

	NEW_OBJECT_TRANSIENT_CHILD(pExplorer3D, Explorer3D, "Explorer3D", this);
	NEW_OBJECT_TRANSIENT_CHILD(pExplorer2D, Explorer2D, "Explorer2D", this);
}

//----------------------------------------------------------------------------------
Explorer::~Explorer()
{
	DELETESINGLE(BufferDataObject);
}

//----------------------------------------------------------------------------------
void Explorer::SuperDeserializer(tinyxml2::XMLElement *xml_current_tree)
{
	Super::SuperDeserializer(xml_current_tree);
}

//----------------------------------------------------------------------------------
void Explorer::Initialize()
{
	Super::Initialize();
}

//----------------------------------------------------------------------------------
void Explorer::Release() 
{ 
	ResetInitialize();

	// do not release itself
	std::for_each(m_ChildNodes.begin(), m_ChildNodes.end(), std::mem_fun(&CActor::Release));
}

//----------------------------------------------------------------------------------
void Explorer::Copy(CActor * Source)
{
	if (Source)
	{
		DELETESINGLE(BufferDataObject);
		BufferDataObject = CLONE_OBJECT(CActor, Source, Source->GetName(), NULL);
	}
}

//----------------------------------------------------------------------------------
void Explorer::Paste(CActor *pInPlaceActor /*= NULL*/)
{
	if (BufferDataObject)
	{
		if (pInPlaceActor == 0){
			pInPlaceActor = this;
		}

		if (pInPlaceActor)
		{
			std::string Name(BufferDataObject->GetName());
			std::string ResolvedName = CActor::ResolveName(Name, pInPlaceActor);
			CActor *NewCopyObject = CLONE_OBJECT(CActor, BufferDataObject, ResolvedName.c_str(), pInPlaceActor);

			if (NewCopyObject)
			{
				//NewCopyObject->m_LTM.t += Vector(30.f, 30.f, 0.f);
				
				NewCopyObject->Initialize();
				
				GetAppMain()->GetViewportManager()->RebuildTransform(NewCopyObject);

				//GetAppMain()->GetViewportManager()->SetFocusActor(NewCopyObject);
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
Explorer2D*	Explorer::GetExplorer2D() const
{
	return pExplorer2D;
}

//----------------------------------------------------------------------------------------------
Explorer3D*	Explorer::GetExplorer3D() const
{
	return pExplorer3D;
}