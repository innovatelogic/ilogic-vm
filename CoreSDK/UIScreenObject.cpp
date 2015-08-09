#include "coresdkafx.h"

//#pragma warning( disable : 4355 )

static const float fOpacityDef = 1.f;

REGISTER_CLASS_A(UIScreenObject, ActorAllocator)
	new PropertyBOOL("ClipBounds", (BYTE*)&((UIScreenObject*)NULL)->ClipBounds - (BYTE*)NULL, "UIScreenObject", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyFLOAT("Opacity", (BYTE*)&((UIScreenObject*)NULL)->Opacity - (BYTE*)NULL, "UIScreenObject", "Value",	READ_WRITE,	CTRL_COMBO,	SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fOpacityDef),
ASSEMBLE_PROPS(UIScreenObject)
BEGIN_INTERFACE_DECLARATION(UIScreenObject)
	new SInterfaceDecl("IDrawInterface", (BYTE*)&((UIScreenObject*)NULL)->___startObjectMarkerIDrawInterface - (BYTE*)NULL),
	new SInterfaceDecl("IDrawInterfaceUI", (BYTE*)&((UIScreenObject*)NULL)->___startObjectMarkerIDrawInterfaceUI - (BYTE*)NULL),
ASSEMBLE_INTERFACES(UIScreenObject)
CLASS_ENDL(UIScreenObject)
CLASS_INSTANCE_EX(UIScreenObject, ActorAllocator);

//----------------------------------------------------------------------------------------------
UIScreenObject::UIScreenObject(const CObjectAbstract *Parent/* = NULL*/)
: Super(Parent)
, IDrawInterfaceUI(Parent)
, ActiveState(ActiveState_Normal)
, ClipBounds(false)
, Opacity(fOpacityDef)
{
	RegisterDrawInterface(this);
}

//----------------------------------------------------------------------------------------------
UIScreenObject::UIScreenObject(const UIScreenObject &Source)
: Super(Source)
, IDrawInterfaceUI(Source)
{
	if (this != &Source)
	{
		ActiveState	= ActiveState_Normal;
		Opacity		= Source.Opacity;
	}
}

//----------------------------------------------------------------------------------------------
UIScreenObject::~UIScreenObject()
{
   ;//
}

//----------------------------------------------------------------------------------------------
void UIScreenObject::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);
}

//----------------------------------------------------------------------------------------------
bool UIScreenObject::AddChildNode(const CActor * actor, int Index /*= INDEX_NONE*/)
{
	return Super::AddChildNode(actor, Index);
}

//----------------------------------------------------------------------------------------------
bool UIScreenObject::HitTest(const Vector2f &Position)
{
	Vector2f LocalPosition = Position - GetTransformedWTM_().t2;

	// calculate projection to local axis
	Vector2f AxisX = GetTransformedWTM_().__row0;
	Vector2f AxisY = GetTransformedWTM_().__row1;

	AxisX.Normalize();
	AxisY.Normalize();

	Matrix IWTM;	
	invert(IWTM, GetTransformedWTM_());

	Vector2f TPosition = Vector2f(LocalPosition.Dot(AxisX) * IWTM.__row0.Length(),
								  LocalPosition.Dot(AxisY) * IWTM.__row1.Length());

	Vector2f TSize = GetTransformedSize_();

	bool inX = (TPosition.x >= 0.f && TPosition.x <= TSize.x);
	bool inY = (TPosition.y >= 0.f && TPosition.y <= TSize.y);
	return inX && inY; 
}

//----------------------------------------------------------------------------------------------
void UIScreenObject::SetEnabled(bool enable)
{
	Super::SetEnabled(enable);
	ResolveActiveState();
}
//----------------------------------------------------------------------------------------------
void UIScreenObject::SetFocus(bool bFlag)
{
	IDrawInterface::SetFocus(bFlag);
	ResolveActiveState();
}

//----------------------------------------------------------------------------------------------
void UIScreenObject::SetMouseOver(bool bFlag /*= true*/)
{
	IDrawInterface::SetMouseOver(bFlag);
	ResolveActiveState();
}

//----------------------------------------------------------------------------------------------
bool UIScreenObject::DoVisibilityTest_()
{
	Vector4f Viewport;
	GetRenderComponent()->GetViewport(Viewport);

	//if (CompositeBounds.bound_max.x < Viewport.x || CompositeBounds.bound_min.x > Viewport.z) { return false; }
	//if (CompositeBounds.bound_max.y < Viewport.y || CompositeBounds.bound_min.y > Viewport.w) { return false; }

	return GetVisible();
}

//----------------------------------------------------------------------------------------------
bool UIScreenObject::UIEditor_ProcessDelete()
{
	if (IsFocused())
	{
		Release();
		return true;
	}

	for (TVecActorChildIterator Iter = m_ChildNodes.begin(); Iter != m_ChildNodes.end(); ++Iter) 
	{
		if ((*Iter)->UIEditor_ProcessDelete()){
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------------
bool UIScreenObject::PushClipBound(Matrix& WTM)
{
	if (GetClipBounds() == true)
	{
		if (GetRenderComponent())
		{
		/*	m_bPushClipping = true; // setup clipping flag
			CRenderSDK * RC =  static_cast<CRenderSDK*>(GetRenderComponent()); 

			Vector2f Pos(WTM.t.x, WTM.t.y); 
			Vector2f ClipSize = GetTransformedSize_();

			// store scissor clipping value data
			m_vClipViewport.x = Pos.x;
			m_vClipViewport.y = Pos.y;
			m_vClipViewport.z = (ClipSize.x);
			m_vClipViewport.w = (ClipSize.y);

			// crop by parent object bounds
			CActor * ParentObject = GetParent();

			while (ParentObject)
			{
				if (ParentObject->m_bPushClipping)
				{
					// crop by parent left side (left X)
					if (m_vClipViewport.x < ParentObject->m_vClipViewport.x){
						m_vClipViewport.z = (m_vClipViewport.x + m_vClipViewport.z) - ParentObject->m_vClipViewport.x;
						m_vClipViewport.x = ParentObject->m_vClipViewport.x;
					}
					// crop by parent top side (top Y)
					if (m_vClipViewport.y < ParentObject->m_vClipViewport.y){
						m_vClipViewport.w = (m_vClipViewport.y + m_vClipViewport.w) - ParentObject->m_vClipViewport.y;
						m_vClipViewport.y = ParentObject->m_vClipViewport.y;
					}
					// crop by parent right side (right X)
					if ((m_vClipViewport.x + m_vClipViewport.z) > (ParentObject->m_vClipViewport.x + ParentObject->m_vClipViewport.z)){
						m_vClipViewport.z = (ParentObject->m_vClipViewport.x + ParentObject->m_vClipViewport.z) - m_vClipViewport.x;
					}
					// crop by parent bottom side (bottom Y)
					if ((m_vClipViewport.y + m_vClipViewport.w) > (ParentObject->m_vClipViewport.y + ParentObject->m_vClipViewport.w)){
						m_vClipViewport.w = (ParentObject->m_vClipViewport.y + ParentObject->m_vClipViewport.w) - m_vClipViewport.y;
					}
					break;
				}
				ParentObject = dynamic_cast<CActor*>(ParentObject->GetParent());
			}

			// clamp in clipping params
			if (m_vClipViewport.x < 0) {
                m_vClipViewport.z = m_vClipViewport.z + m_vClipViewport.x;
                m_vClipViewport.x = 0;
			}
			if (m_vClipViewport.y < 0) {
				m_vClipViewport.w = m_vClipViewport.w + m_vClipViewport.y;
				m_vClipViewport.y = 0;
			}
			m_vClipViewport.z = (m_vClipViewport.z < 0) ? 0 : m_vClipViewport.z;
			m_vClipViewport.w = (m_vClipViewport.w < 0) ? 0 : m_vClipViewport.w;
			 
			// set render scisser test
			GetAppMain()->GetRenderSDK()->SetViewport(m_vClipViewport.x,
				m_vClipViewport.y,
				m_vClipViewport.z,
				m_vClipViewport.w);*/
		}
	}
	return true;
}

//----------------------------------------------------------------------------------------------
void UIScreenObject::PopClipBound()
{
	// restore parent's clip viewport 
	if (GetClipBounds() == true)
	{
/*		UIScreenObject * ParentObject = dynamic_cast<UIScreenObject*>(GetParent());

		// try to roll back parant scisser test parameters
		while (ParentObject)
		{
			if (ParentObject->m_bPushClipping)
			{
				GetAppMain()->GetRenderSDK()->SetViewport(ParentObject->m_vClipViewport.x,	ParentObject->m_vClipViewport.y, 
														  ParentObject->m_vClipViewport.z,	ParentObject->m_vClipViewport.w);
				m_bPushClipping = false; // drop out scisser test flag
				break;
			}
			ParentObject = dynamic_cast<UIScreenObject*>(ParentObject->GetParent());
		}

		// fail roll back drop params/ restore to original viewport size
		if (m_bPushClipping == true)
		{
			CRenderSDK * RC =  static_cast<CRenderSDK*>(GetRenderComponent()); 
			GetAppMain()->GetRenderSDK()->SetViewport(0.f, 0.f, (float)RC->GetRenderDriver()->GetWidth(), (float)RC->GetRenderDriver()->GetHeight());
			m_bPushClipping = true;// drop out scisser test flag
		}*/
	}
}

//----------------------------------------------------------------------------------------------
// DoEventPressed() called when user activate object by pressing mouse button
bool UIScreenObject::DoEventPressed(const MouseInputData& InputData)
{
	return Super::DoEventPressed(InputData);
}

//----------------------------------------------------------------------------------------------
bool UIScreenObject::DoEventReleased(const MouseInputData& InputData)
{
	return Super::DoEventReleased(InputData);
}

//----------------------------------------------------------------------------------------------
/*void UIScreenObject::DoRebuildBounds()
{
	Vector AxisX(1.f, 0.f, 0.f);
	Vector AxisY(0.f, 1.f, 0.f);

	float px0 = WorldMatrixTransform._row0.Dot(AxisX);
	float px1 = WorldMatrixTransform._row1.Dot(AxisX);

	float py0 = WorldMatrixTransform._row0.Dot(AxisY);
	float py1 = WorldMatrixTransform._row1.Dot(AxisY);

	float MinX = 0.f;
	float MinY = 0.f;

	Vector2f MatrixScale = GetTransformedSize_();

	if (px0 < 0.f || px1 < 0.f)
	{
		MinX += (px0 < 0.f && px1 < 0.f) ? (px0 * MatrixScale.x + px1 * MatrixScale.y) :
											(px0 < px1 ? (px0 * MatrixScale.x) : (px1 * MatrixScale.y)); 
	}
	if (py0 < 0.f || py1 < 0.f)
	{
		MinY += (py0 < 0.f && py1 < 0.f) ? (py0 * MatrixScale.x + py1 * MatrixScale.y):
											(py0 < py1 ? (py0 * MatrixScale.x) : (py1 * MatrixScale.y)); 
	}
	Bounds.bound_min = Vector(MinX, MinY, 0.f);
	Bounds.bound_max = Bounds.bound_min +  Vector(fabs(px0 * MatrixScale.x) + fabs(px1 * MatrixScale.y), 
													fabs(py0 * MatrixScale.x) + fabs(py1 * MatrixScale.y), 0);
}*/

//----------------------------------------------------------------------------------------------
/*bool UIScreenObject::DoVisibilityTest()
{
	Vector4f Viewport;
	GetRenderComponent()->GetViewport(Viewport);

	if (CompositeBounds.bound_max.x < Viewport.x || CompositeBounds.bound_min.x > Viewport.z) { return false; }
	if (CompositeBounds.bound_max.y < Viewport.y || CompositeBounds.bound_min.y > Viewport.w) { return false; }

	return true;
}*/
