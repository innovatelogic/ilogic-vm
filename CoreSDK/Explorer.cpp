#include "coresdkafx.h"

REGISTER_CLASS(Explorer, ActorAllocator);

//----------------------------------------------------------------------------------
Explorer::Explorer(const CObjectAbstract *pParent)
: Super(0)
, bMiddleButtonPressed(false)
, BufferDataObject(NULL)
{
	SetTransient(true);

	SetUserData(pParent->GetUserData());
	SetUserData(pParent->GetUserData(0), 0);

	SetAppMain((CCoreSDK*)GetUserData());

	NEW_OBJECT_TRANSIENT_CHILD(pExplorer3D, Explorer3D, "Explorer3D", this);
	NEW_OBJECT_TRANSIENT_CHILD(pExplorer2D, Explorer2D, "Explorer2D", this);
	NEW_OBJECT_TRANSIENT_CHILD(m_pModelViewer, CModelViewer, "ModelViewer", this);
}

//----------------------------------------------------------------------------------
Explorer::Explorer(const Explorer& Source)
: Super(Source)
{
	if (this != &Source)
	{
	}
}

//----------------------------------------------------------------------------------
Explorer::~Explorer()
{
	DELETESINGLE(BufferDataObject);
}

//----------------------------------------------------------------------------------
void Explorer::SuperDeserializer(TiXmlElement *xml_current_tree)
{
	InitializeExploers();

	Super::SuperDeserializer(xml_current_tree);
}

//----------------------------------------------------------------------------------
void Explorer::Initialize()
{
	InitializeBrushes();

	Super::Initialize();
}

//----------------------------------------------------------------------------------
void Explorer::InitializeExploers()
{

}

//----------------------------------------------------------------------------------
void Explorer::InitializeBrushes()
{
	for (TVecActorChildIterator Iter = m_ChildNodes.begin(); Iter != m_ChildNodes.end(); ++Iter) 
	{
		if (dynamic_cast<Explorer2D*>(*Iter)){
			dynamic_cast<Explorer2D*>(*Iter)->InitializeBrushes();
		}

		if (dynamic_cast<Explorer3D*>(*Iter)){
			dynamic_cast<Explorer3D*>(*Iter)->InitializeBrushes();
		}
	}
}

//----------------------------------------------------------------------------------
void Explorer::Release() 
{ 
	ResetInitialize();

	// do not release itself
	std::for_each(m_ChildNodes.begin(), m_ChildNodes.end(), std::mem_fun(&CActor::Release));
}

//----------------------------------------------------------------------------------
bool Explorer::IsBrushActivated()
{
	bool bResult = false;
/*
	for (TVecActorChildIterator Iter = ChildNodes.begin(); Iter != ChildNodes.end(); ++Iter) 
	{
		ViewportInterface * VInterface = ActorAllocator::GetViewportInterface(*Iter);
		if (VInterface)
		{
			ActorAllocator * AActor = dynamic_cast<ActorAllocator*>(*Iter);

			if (AActor && AActor->IsBrushActivated())
			{
				bResult = true;
				break;
			}
		}
	}*/
	return bResult;
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

//----------------------------------------------------------------------------------
bool Explorer::ProcessMouseWheel(float ds, int x, int y)
{
/*	const CActor * Focused = GetFocused();

	if (Focused != NULL)
	{
		ViewportInterface * VInterface = ActorAllocator::GetViewportInterface(Focused);
		if (VInterface)
		{
			Matrix ViewMatrix = VInterface->GetViewportViewMatrix();

			// interpolate zoom to target point
			ViewMatrix.t += (Vector((float)x, (float)y, 0.f) - ViewMatrix.t) * (1.f - ds);
			ViewMatrix.a00 *= ds;
			ViewMatrix.a11 *= ds;

			VInterface->SetViewportViewMatrix(ViewMatrix);
		}
	}*/
	return true;
}

//----------------------------------------------------------------------------------
bool Explorer::UIEditor_ProcessDelete()
{
	for (TVecActorChildIterator Iter = m_ChildNodes.begin(); Iter != m_ChildNodes.end(); ++Iter) 
	{
		if ((*Iter)->UIEditor_ProcessDelete())
		{
			return true;
		}
	}
	return false;
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

//----------------------------------------------------------------------------------------------
CModelViewer* Explorer::GetModelViewer() const
{
	return m_pModelViewer;
}