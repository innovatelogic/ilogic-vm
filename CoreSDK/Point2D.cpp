#include "coresdkafx.h"

REGISTER_CLASS(Point2D, UIScreenObject);

//---------------------------------------------------------------------
Point2D::Point2D(const CObjectAbstract * Parent)
: Super(Parent)
{
//    SetSize(Vector2f(8.f, 8.f));
}

//---------------------------------------------------------------------
Point2D::Point2D(const Point2D& Source)
: Super(Source)
{
	if (this != &Source)
	{

	}
}

//---------------------------------------------------------------------
Point2D::~Point2D()
{
    ;//
}

//---------------------------------------------------------------------
/** Load XML file */
// void Point2D::Deserialize(tinyxml2::XMLElement *xml_current_tree)
// {
// 	Super::Deserialize(xml_current_tree);
// 
// 	SetSize(Vector2f(8.f, 8.f));
// }

//---------------------------------------------------------------------
bool Point2D::ProcessUserController(const MouseMoveInputData& InputData)
{
// 	switch (GetControlMode())
// 	{
// 	case SOEvent_FreeMove:
// 	case SOEvent_ControlLockOrigin:
// 	case SOEvent_ControlLockX:
// 	case SOEvent_ControlLockY:
// 		{
// 			if (InputData.ModifKey & MK_Control || InputData.ModifKey & MK_Shift) // avoid scale and rotate by axis
// 			{ 
// 				return true;
// 			}
// 		}break;
// 	}
// 	return Super::ProcessUserController(InputData);
	return false;
}

//-----------------------------------------------------------------------------
bool Point2D::HitTest(const Vector2f &Position)
{
/*	core_sdk_api::ViewportInterface * Interface = GetViewportInterface(this);
	assert(Interface);

	Matrix ViewMatrix = Interface->GetViewportViewMatrix();;

	Vector2f BoxOrigin = GetSize() * (1.f / ViewMatrix.__row0.Length());

	if (IsPointInRect(Position.x, Position.y, 
		WorldMatrixTransform.t2 - (Vector2f(BoxOrigin.x, BoxOrigin.y) * 0.5f),
		WorldMatrixTransform.t2 + (Vector2f(BoxOrigin.x, BoxOrigin.y))* 0.5f)
		)
	{
		return true;
	}*/
	return false;
}

//---------------------------------------------------------------------
void Point2D::DoDraw()
{
/*	CActor * Parent = GetParent();

	if (Parent)
	{
		bool bChildFocused = Parent->IsFocused() || Parent->GetExtraSelection();

		for (TVecActorChildIterator Iter = Parent->ChildNodes.begin(); !bChildFocused && Iter != Parent->ChildNodes.end(); ++Iter) 
		{
			if ((*Iter)->GetType() == GetType())
			{
				if ((*Iter)->IsFocused()){
					bChildFocused = true;
					break;
				}
			}
		}

		if (IsFocused() || bChildFocused)
		{
			Matrix IWTM;
			invert(IWTM, WorldMatrixTransform);

			Vector2f TSize = GetSize();
			Vector2f AxisX = (IWTM.__row0 * TSize.x) * (1.f / GetRenderComponent()->GetViewMatrix().__row0.Length());
			Vector2f AxisY = (IWTM.__row1 * TSize.y) * (1.f / GetRenderComponent()->GetViewMatrix().__row0.Length());

			GetRenderComponent()->DrawLine(WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * -0.5f, IWTM),		   
										   WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * -0.5f + AxisX, IWTM), 0xffffff00);
			GetRenderComponent()->DrawLine(WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * -0.5f + AxisX, IWTM),
										   WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * 0.5f, IWTM), 0xffffff00);
			GetRenderComponent()->DrawLine(WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * 0.5f, IWTM),
										   WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * 0.5f - AxisX, IWTM), 0xffffff00);
			GetRenderComponent()->DrawLine(WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * 0.5f - AxisX, IWTM),
										   WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * -0.5f, IWTM), 0xffffff00);
		}
	}*/
}

//---------------------------------------------------------------------
void Point2D::PreRelease()
{
	if (dynamic_cast<SplineAgregator*>(GetParent()) && 
		!GetParent()->IsPendingToDelete() && 
 		!GetParent()->bMarkerDeleteFlag
 		/*!ForceOperation*/)
	{
		GetAppMain()->EnterDrawCS();
		
		dynamic_cast<SplineAgregator*>(GetParent())->Rebuild();

		GetAppMain()->LeaveDrawCS();
	}
	Super::PreRelease();
}

//---------------------------------------------------------------------
bool Point2D::DoEventReleased(const MouseInputData& InputData)
{
	/*if (dynamic_cast<SplineAgregator*>(GetParent()) && !GetParent()->IsPendingToDelete())
	{
		GetAppMain()->EnterDrawCS();
		GetRenderComponent()->EnterCS();

		dynamic_cast<SplineAgregator*>(GetParent())->Rebuild();
		GetParent()->RebuildTransform();
		
		GetRenderComponent()->LeaveCS();
		GetAppMain()->LeaveDrawCS();
	}*/
	return Super::DoEventReleased(InputData);
}

//---------------------------------------------------------------------
/*bool Point2D::ProcessControllers(const Vector2f& Position)
{
	core_sdk_api::ViewportInterface * Interface = GetViewportInterface(this);
	assert(Interface);

	Matrix ViewMatrix = Interface->GetViewportViewMatrix();

	Vector2f BoxOrigin = GetSize() * (1.f / ViewMatrix.__row0.Length());

	if (GetParent() && GetParent()->IsFocused())
	{	
		if (IsPointInRect(Position.x, Position.y, 
			WorldMatrixTransform.t2 - (Vector2f(BoxOrigin.x, BoxOrigin.y) * 0.5f),
			WorldMatrixTransform.t2 + (Vector2f(BoxOrigin.x, BoxOrigin.y))* 0.5f)
			)
		{
			SetControlMode(SOEvent_ControlLockOrigin);
			return true;
		}
	}
	return false;
}*/