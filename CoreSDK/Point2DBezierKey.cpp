#include "coresdkafx.h"

REGISTER_CLASS(Point2DBezierKey, Point2D);

//---------------------------------------------------------------------
Point2DBezierKey::Point2DBezierKey(const CObjectAbstract * Parent)
: Super(Parent)
{
//	SetSize(Vector2f(5.f, 5.f));
}

//---------------------------------------------------------------------
Point2DBezierKey::Point2DBezierKey(const Point2DBezierKey& Source)
: Super(Source)
{
	if (this != &Source)
	{

	}
}

//---------------------------------------------------------------------
Point2DBezierKey::~Point2DBezierKey()
{

}

//---------------------------------------------------------------------
void Point2DBezierKey::DoDraw()
{
/*	const CActor * Parent = GetParent();

	if (Parent)
	{
		const CActor * SuperParent = Parent->GetParent();

		if (SuperParent && (SuperParent->IsFocused() || SuperParent->GetExtraSelection()))
		{	
			Matrix IWTM;
			invert(IWTM, WorldMatrixTransform);

			Vector2f TSize = GetSize();
			Vector2f AxisX = (IWTM.__row0 * TSize.x) * (1.f / GetRenderComponent()->GetViewMatrix().__row0.Length());
			Vector2f AxisY = (IWTM.__row1 * TSize.y) * (1.f / GetRenderComponent()->GetViewMatrix().__row0.Length());

			GetRenderComponent()->DrawLine(WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * -0.5f, IWTM),		   
										   WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * -0.5f + AxisX, IWTM), 0xff000000);
			GetRenderComponent()->DrawLine(WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * -0.5f + AxisX, IWTM),
										   WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * 0.5f, IWTM), 0xff000000);
			GetRenderComponent()->DrawLine(WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * 0.5f, IWTM),
										   WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * 0.5f - AxisX, IWTM), 0xff000000);
			GetRenderComponent()->DrawLine(WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * 0.5f - AxisX, IWTM),
										   WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * -0.5f, IWTM), 0xff000000);
		}
	}*/
}
//---------------------------------------------------------------------
/*bool Point2DBezierKey::ProcessControllers(const Vector2f& Position)
{	
	const CActor * Parent = GetParent();

	if (Parent)
	{
		const CActor * SuperParent = Parent->GetParent();

		if (SuperParent && (SuperParent->IsFocused() || SuperParent->GetExtraSelection()))
		{	
			core_sdk_api::ViewportInterface * Interface = GetViewportInterface(this);
			assert(Interface);

			Matrix ViewMatrix = Interface->GetViewportViewMatrix();
			Vector2f BoxOrigin = GetSize() * (1.f / ViewMatrix.__row0.Length());

			if (IsPointInRect(Position.x, Position.y, 
				WorldMatrixTransform.t2 - (Vector2f(BoxOrigin.x, BoxOrigin.y) * 0.5f),
				WorldMatrixTransform.t2 + (Vector2f(BoxOrigin.x, BoxOrigin.y))* 0.5f)
				)
			{
				SetControlMode(SOEvent_ControlLockOrigin);
				return true;
			}
		}
	}
	
	return false;
}*/

//---------------------------------------------------------------------
bool Point2DBezierKey::DoEventReleased(const MouseInputData& InputData)
{
	if (GetParent() && 
		GetParent()->GetParent() &&
		dynamic_cast<Bezier_SplineAgregator*>(GetParent()->GetParent()) &&
		!GetParent()->GetParent()->IsPendingToDelete())
	{
		GetAppMain()->EnterDrawCS();
		GetRenderComponent()->EnterCS();

		dynamic_cast<Bezier_SplineAgregator*>(GetParent()->GetParent())->Rebuild();

		GetRenderComponent()->LeaveCS();
		GetAppMain()->LeaveDrawCS();
	}
	return Super::DoEventReleased(InputData);
}