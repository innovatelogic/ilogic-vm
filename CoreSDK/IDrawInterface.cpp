#include "coresdkafx.h"

#define COLOR_DARK_RED		0xff8b1818
#define COLOR_DARK_GREEN	0xff117900
#define COLOR_DARK_BLUE		0xff000079

#define ROT_N0 1

Vector	IDrawInterface::m_SUserStartMousePosition = Vector(0.f, 0.f, 0.f);
Vector	IDrawInterface::m_SUserStartMouseDisplace = Vector(0.f, 0.f, 0.f);
bool	IDrawInterface::m_bSMiddleButtonPressed = false;

static const bool V_TRUE = true;
static const bool V_FALSE = false;
static const Matrix DefMatrixTransform(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f);
static const Matrix3f DefMatrixScale(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
static const Vector DefVec(0.f, 0.f, 0.f);

BEGIN_AUTO_CLASS(IDrawInterface)
	new PropertyBOOL("bVisible", (BYTE*)&((IDrawInterface*)NULL)->m_bVisible - (BYTE*)&((IDrawInterface*)NULL)->___startObjectMarkerIDrawInterface, "IDrawInterface", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &V_TRUE),
	new PropertyBOOL("RenderDebug",	(BYTE*)&((IDrawInterface*)NULL)->m_bRenderDebug - (BYTE*)&((IDrawInterface*)NULL)->___startObjectMarkerIDrawInterface, "IDrawInterface", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &V_FALSE),
	new PropertyMatrix("LTM", (BYTE*)&((IDrawInterface*)NULL)->m_LTM_ - (BYTE*)&((IDrawInterface*)NULL)->___startObjectMarkerIDrawInterface, "IDrawInterface", "Value", READ_ONLY, CTRL_MATRIX16, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &DefMatrixTransform),
	new PropertyMatrix3x3("STM", (BYTE*)&((IDrawInterface*)NULL)->m_STM_ - (BYTE*)&((IDrawInterface*)NULL)->___startObjectMarkerIDrawInterface, "IDrawInterface", "Value", READ_ONLY, CTRL_MATRIX9, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &DefMatrixScale),
	new PropertyVector("Rotator", (BYTE*)&((IDrawInterface*)NULL)->m_YawPitchRoll - (BYTE*)&((IDrawInterface*)NULL)->___startObjectMarkerIDrawInterface, "IDrawInterface", "Value", READ_ONLY, CTRL_VECTOR, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &DefVec),
END_AUTO_CLASS(IDrawInterface)
CLASS_INSTANCE_INTERFACE(IDrawInterface);

//----------------------------------------------------------------------------------------------
bool IDrawInterface::GetMBPressed()
{
	return IDrawInterface::m_bSMiddleButtonPressed;
}

//----------------------------------------------------------------------------------------------
Vector IDrawInterface::GetUserStartMousePos()
{
	return IDrawInterface::m_SUserStartMousePosition;
}

//----------------------------------------------------------------------------------------------
Vector IDrawInterface::GetUserStartMouseDisplace()
{
	return IDrawInterface::m_SUserStartMouseDisplace;
}

//----------------------------------------------------------------------------------------------
IDrawInterface::IDrawInterface(const CObjectAbstract *pParent)
: m_pNode(0)
, m_VisualStates(AS_NONE)
, m_bVisible(V_TRUE)
, m_bRenderDebug(V_FALSE)
, m_Bounds(V_FLT_MAX, V_FLT_MAX, V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX)
, m_CompositeBounds(V_FLT_MAX, V_FLT_MAX, V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX)
, m_LTM_(DefMatrixTransform)
, m_STM_(DefMatrixScale)
{
	if (pParent)
	{
 		m_pCoreSDK = (CCoreSDK*)pParent->GetUserData();
	}
}

//----------------------------------------------------------------------------------------------
IDrawInterface::IDrawInterface(const IDrawInterface &Source)
: m_pNode(0)
, m_VisualStates(AS_NONE)
{
	if (this != &Source)
	{
		m_pCoreSDK = Source.m_pCoreSDK;
		m_bVisible = Source.m_bVisible;
		m_bRenderDebug = Source.m_bRenderDebug;
		m_LTM_ = Source.m_LTM_;
		m_STM_ = Source.m_STM_;
		m_Bounds = Source.m_Bounds;
		m_CompositeBounds = Source.m_CompositeBounds;
	}
}

//----------------------------------------------------------------------------------------------
IDrawInterface::~IDrawInterface()
{
	UnregisterDrawInterface();
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::RegisterDrawInterface(const CActor *pSrc, int SrcParent /*= 1*/)
{
	const_cast<CActor*>(pSrc)->RegisterInterfaceImpl(this);

	if (m_pCoreSDK){
		m_pNode = m_pCoreSDK->GetViewportManager()->RegisterObject(this, pSrc, (SrcParent == 1) ? pSrc->GetParent() : nullptr );
	}
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::UnregisterDrawInterface()
{
	if (m_pCoreSDK && m_pNode)
	{
		m_pCoreSDK->GetViewportManager()->UnregisterObject(m_pNode);
	}
}

//----------------------------------------------------------------------------------------------
bool IDrawInterface::DoVisibilityTest_() const
{
	return true;
}

//----------------------------------------------------------------------------------------------
bool IDrawInterface::PrePropertyChangeIntf(const char *PropertyName)
{
	return true;
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::OnPropertyChangedIntf(const char *PropertyName)
{
	if (!strcmp(PropertyName, "Position") || 
		!strcmp(PropertyName, "PositionX") ||
		!strcmp(PropertyName, "PositionY") || 
		!strcmp(PropertyName, "PositionZ") ||
		!strcmp(PropertyName, "LTM") || 
		!strcmp(PropertyName, "STM"))
	{
		m_pCoreSDK->GetViewportManager()->RebuildTransform(const_cast<CActor*>(m_pNode->m_pKey));
	}
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::Draw()
{
	DoDraw();

	if (m_pCoreSDK->IsAEditor() && !m_pCoreSDK->IsAGame())
	{
		DrawBounds();
		DrawController();
	}
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::DrawController() const
{
	if (m_pNode->m_pValue->IsFocused())
	{
		RenderSDK::SRTVariant_Adjacency &queve = m_pCoreSDK->GetRenderSDK()->GetCurrQuevueAdjaency();

		if (const CCamera *pCamera = m_pCoreSDK->GetCameraManager()->GetActiveCamera(queve.__RT_VARIANT_NAME_1.__RT_VARIANT_NAME_2.pRenderContext))
		{
			EScrObjectEvent Mode = m_pNode->m_pKey->GetControlMode();
			Matrix WorldMatrixTransform = GetTransformedWTM_();

            const Vector &position = WorldMatrixTransform.t;

			Vector CamStrafe(pCamera->GetStrafe());
			Vector TransDelta(pCamera->GetTransformedWTM_().t - position);

			CamStrafe.normalize();

			float fCathetusOppositLen = TransDelta.Length() * ::tan(0.1f);
			Vector vCathetusOpposit = TransDelta.Cross(Vector(0.f, 1.f, 0.f));
			vCathetusOpposit.normalize();
			vCathetusOpposit *= fCathetusOppositLen;
			
			float k = CamStrafe.Dot(vCathetusOpposit);
						
			Matrix I;
			const Vector AxisX = I._row0 * k;
			const Vector AxisY = I._row1 * k;
			const Vector AxisZ = I._row2 * k;

			m_pCoreSDK->GetRenderSDK()->DrawLine(position, position + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED, false);
			m_pCoreSDK->GetRenderSDK()->DrawLine(position, position + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE, false);
			m_pCoreSDK->GetRenderSDK()->DrawLine(position, position + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN, false);

			// render arrows
			float a, b;
			a = 0.8f; b = 0.1f;

			// X arrow
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisX * a) + (AxisY * b), position + (AxisX * a) + (AxisZ * b), position + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisX * a) + (AxisZ * b), position + (AxisX * a) - (AxisY * b), position + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisX * a) - (AxisY * b), position + (AxisX * a) - (AxisZ * b), position + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisX * a) - (AxisZ * b), position + (AxisX * a) + (AxisY * b), position + AxisX, (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);
			
			//back
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisX * a) - (AxisY * b), position + (AxisX * a) + (AxisZ * b), position + (AxisX * a) + (AxisY * b), (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_DARK_RED);
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisX * a) - (AxisZ * b), position + (AxisX * a) - (AxisY * b), position + (AxisX * a) + (AxisY * b), (Mode == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_DARK_RED);

			//Y arrow
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisY * a) + (AxisZ * b), position + (AxisY * a) + (AxisX * b), position + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisY * a) + (AxisX * b), position + (AxisY * a) - (AxisZ * b), position + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisY * a) - (AxisZ * b), position + (AxisY * a) - (AxisX * b), position + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisY * a) - (AxisX * b), position + (AxisY * a) + (AxisZ * b), position + AxisY, (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
			
			//back
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisY * a) - (AxisZ * b), position + (AxisY * a) + (AxisX * b), position + (AxisY * a) + (AxisZ * b), (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_DARK_BLUE);
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisY * a) + (AxisZ * b), position + (AxisY * a) - (AxisX * b), position + (AxisY * a) - (AxisZ * b), (Mode == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_DARK_BLUE);

			//Z arrow
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisZ * a) + (AxisX * b), position + (AxisZ * a) + (AxisY * b), position + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisZ * a) + (AxisY * b), position + (AxisZ * a) - (AxisX * b), position + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisZ * a) - (AxisX * b), position + (AxisZ * a) - (AxisY * b), position + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisZ * a) - (AxisY * b), position + (AxisZ * a) + (AxisX * b), position + AxisZ, (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_GREEN);

			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisZ * a) + (AxisY * b), position + (AxisZ * a) + (AxisX * b), position + (AxisZ * a) - (AxisY * b), (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_DARK_GREEN);
			m_pCoreSDK->GetRenderSDK()->DrawTriangle(position + (AxisZ * a) - (AxisY * b), position + (AxisZ * a) - (AxisX * b), position + (AxisZ * a) + (AxisY * b), (Mode == SOEvent_ControlLockZ) ? COLOR_YELLOW : COLOR_DARK_GREEN);

			//render origin rects
			float c = 0.3f;

			// X-Y
			m_pCoreSDK->GetRenderSDK()->DrawLine(position + (AxisX * c), position + (AxisX * c) + (AxisY * c), (Mode == SOEvent_ControlLockXY) ? COLOR_YELLOW : COLOR_RED, false);
			m_pCoreSDK->GetRenderSDK()->DrawLine(position + (AxisX * c) + (AxisY * c), position + (AxisY * c), (Mode == SOEvent_ControlLockXY) ? COLOR_YELLOW : COLOR_BLUE, false);

			// Y-Z
			m_pCoreSDK->GetRenderSDK()->DrawLine(position + (AxisY * c), position + (AxisY * c) + (AxisZ * c), (Mode == SOEvent_ControlLockYZ) ? COLOR_YELLOW : COLOR_BLUE, false);
			m_pCoreSDK->GetRenderSDK()->DrawLine(position + (AxisY * c) + (AxisZ * c), position + (AxisZ * c), (Mode == SOEvent_ControlLockYZ) ? COLOR_YELLOW : COLOR_GREEN, false);

			// X-Z
			m_pCoreSDK->GetRenderSDK()->DrawLine(position + (AxisX * c), position + (AxisX * c) + (AxisZ * c), (Mode == SOEvent_ControlLockXZ) ? COLOR_YELLOW : COLOR_RED, false);
			m_pCoreSDK->GetRenderSDK()->DrawLine(position + (AxisX * c) + (AxisZ * c), position + (AxisZ * c), (Mode == SOEvent_ControlLockXZ) ? COLOR_YELLOW : COLOR_GREEN, false);
		
			// higlight control axis
			if (m_pNode->m_pKey->GetControlState() == ActorState_Locked)
			{
				if (GetCoreSDK()->GetEditControlMode() == ECM_Move)
				{
					switch (m_pNode->m_pKey->GetControlMode())
					{
					case SOEvent_ControlLockX:
						{
							m_pCoreSDK->GetRenderSDK()->DrawLine(position - AxisX, position, COLOR_GREEN, false);
							m_pCoreSDK->GetRenderSDK()->DrawLine(position + AxisX, position + (AxisX * 2), COLOR_GREEN, false);
						}break;

					case SOEvent_ControlLockY:
						{	
							m_pCoreSDK->GetRenderSDK()->DrawLine(position - AxisY, position, COLOR_GREEN, false);
							m_pCoreSDK->GetRenderSDK()->DrawLine(position + AxisY, position + (AxisY * 2), COLOR_GREEN, false);
						}break;

					case SOEvent_ControlLockZ:
						{	
							m_pCoreSDK->GetRenderSDK()->DrawLine(position - AxisZ, position, COLOR_GREEN, false);
							m_pCoreSDK->GetRenderSDK()->DrawLine(position + AxisZ, position + (AxisZ * 2), COLOR_GREEN, false);
						}break;
					}
				}
				else if (GetCoreSDK()->GetEditControlMode() == ECM_Rotate)
				{
					switch (m_pNode->m_pKey->GetControlMode())
					{
					case SOEvent_ControlLockX:
						{
							m_pCoreSDK->GetRenderSDK()->DrawLine(position - AxisY, position, COLOR_GREEN, false);
							m_pCoreSDK->GetRenderSDK()->DrawLine(position + AxisY, position + (AxisY * 2), COLOR_GREEN, false);
						}break;

					case SOEvent_ControlLockY:
						{	
							m_pCoreSDK->GetRenderSDK()->DrawLine(position - AxisZ, position, COLOR_GREEN, false);
							m_pCoreSDK->GetRenderSDK()->DrawLine(position + AxisZ, position + (AxisZ * 2), COLOR_GREEN, false);
						}break;

					case SOEvent_ControlLockZ:
						{		
							m_pCoreSDK->GetRenderSDK()->DrawLine(position - AxisX, position, COLOR_GREEN, false);
							m_pCoreSDK->GetRenderSDK()->DrawLine(position + AxisX, position + (AxisX * 2), COLOR_GREEN, false);
						}break;
					}
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::DrawBounds() const
{
#ifndef FINAL_RELEASE
 	if (m_pCoreSDK->GetObjectBoundsVisible() && m_Bounds.IsValid())
 	{
 		m_pCoreSDK->GetRenderSDK()->DrawBounds(m_WorldMatrixTransform, m_Bounds, COLOR_BLUE);
 	}
 
// 	if (m_pCoreSDK->GetSparitalSubdivisionVisible() && CompositeBounds.IsValid())
// 	{
// 		Matrix I;
// 		GetRenderComponent()->DrawBounds(I, CompositeBounds, COLOR_GREEN);
// 	}
#endif//FINAL_RELEASE
}

//----------------------------------------------------------------------------------------------
bool IDrawInterface::ProcessController(const MouseInputData &InputData)
{
	const Vector2f &Position = InputData.MousePos;

	switch (InputData.Code)
	{
	case MOUSE_LEFT:
		{
			if (m_pNode->m_pValue->IsFocused())
			{
				if (m_pCoreSDK->GetCameraManager())
				{
					if (const CCamera *pCamera = m_pCoreSDK->GetCameraManager()->GetActiveCamera(InputData.pRenderContext))
					{
						Vector2f &ControlBox = m_pCoreSDK->GetRegistry()->ControlBox;
						Matrix WorldMatrixTransform = GetTransformedWTM_();

						Matrix ViewMatrix(pCamera->GetViewMatrix());
						Matrix ProjMatrix(pCamera->GetProjMatrix());

						Matrix InvView, I;
						invert(InvView, pCamera->GetViewMatrix());

						Vector CamStrafe(pCamera->GetStrafe());
						Vector TransDelta(pCamera->GetTransformedWTM_().t - WorldMatrixTransform.t);
						CamStrafe.normalize();

						float fCathetusOppositLen = TransDelta.Length() * ::tan(0.1f);
						Vector vCathetusOpposit = TransDelta.Cross(Vector(0.f, 1.f, 0.f));
						vCathetusOpposit.normalize();
						vCathetusOpposit *= fCathetusOppositLen;

						float k = CamStrafe.Dot(vCathetusOpposit); // projection length 

						const Vector AxisX = I._row0 * k;
						const Vector AxisY = I._row1 * k;
						const Vector AxisZ = I._row2 * k;

						const Vector2f ViewportSize((float)InputData.pRenderContext->m_displayModeWidth,
													(float)InputData.pRenderContext->m_displayModeHeight);
						Vector AX, AY, AZ;

						ProjectViewport(AX, WorldMatrixTransform.t + AxisX, ViewMatrix, ProjMatrix, ViewportSize);
						ProjectViewport(AY, WorldMatrixTransform.t + AxisY, ViewMatrix, ProjMatrix, ViewportSize);
						ProjectViewport(AZ, WorldMatrixTransform.t + AxisZ, ViewMatrix, ProjMatrix, ViewportSize);

						Vector ViewDirection = UnprojectViewport(ViewDirection, 
							pCamera->GetProjMatrix(), 
							pCamera->GetViewMatrix(),
							Position,
							ViewportSize);

						if (IsPointInRect(Position.x, Position.y, Vector2f(AX.x, AX.y) - ControlBox, Vector2f(AX.x, AX.y) + ControlBox))
						{
							Vector PlaneNormal = cross(PlaneNormal, ViewDirection, InvView._row1);
							Vector Intersect = RayPlaneIntersect(pCamera->GetTransformedWTM_().t, PlaneNormal, WorldMatrixTransform.t, I._row0);

							m_SUserStartMouseDisplace = (Intersect - WorldMatrixTransform.t/* + AxisX*/) * (1.f/k);
							m_SUserStartMousePosition = Vector(Position.x, Position.y, 0.f);

							return const_cast<CActor*>(m_pNode->m_pKey)->SetControlMode(SOEvent_ControlLockX);
						}
						if (IsPointInRect(Position.x, Position.y, Vector2f(AY.x, AY.y) - ControlBox, Vector2f(AY.x, AY.y) + ControlBox))
						{
							Vector PlaneNormal = cross(PlaneNormal, ViewDirection, InvView._row0);
							Vector Intersect = RayPlaneIntersect(pCamera->GetTransformedWTM_().t, PlaneNormal, WorldMatrixTransform.t, I._row1);

							m_SUserStartMouseDisplace = (Intersect - WorldMatrixTransform.t /*+ AxisY*/) * (1.f/k);
							m_SUserStartMousePosition = Vector(Position.x, Position.y, 0.f);

							return const_cast<CActor*>(m_pNode->m_pKey)->SetControlMode(SOEvent_ControlLockY);
						}
						if (IsPointInRect(Position.x, Position.y, Vector2f(AZ.x, AZ.y) - ControlBox, Vector2f(AZ.x, AZ.y) + ControlBox))
						{
							Vector PlaneNormal = cross(PlaneNormal, ViewDirection, InvView._row1);
							Vector Intersect = RayPlaneIntersect(pCamera->GetTransformedWTM_().t, PlaneNormal, WorldMatrixTransform.t, I._row2);

							m_SUserStartMouseDisplace = (Intersect - WorldMatrixTransform.t /*+ AxisZ*/) * (1.f/k);
							m_SUserStartMousePosition = Vector(Position.x, Position.y, 0.f);

							return const_cast<CActor*>(m_pNode->m_pKey)->SetControlMode(SOEvent_ControlLockZ);
						}

						// check plane origin hit
						/** ray triangle intersection */
						float c = 0.3f;
						float t = 0.f;

						Vector P0 = (WorldMatrixTransform.t + (AxisX * c));
						Vector P1 = (WorldMatrixTransform.t + (AxisX * c) + (AxisY * c));
						Vector P2 = (WorldMatrixTransform.t + (AxisY * c));

						if ((IntersectRayWithTriangle(pCamera->GetTransformedWTM_().t, pCamera->GetTransformedWTM_().t + ViewDirection, WorldMatrixTransform.t, P0, P1, &t) == 1) ||
							(IntersectRayWithTriangle(pCamera->GetTransformedWTM_().t, pCamera->GetTransformedWTM_().t + ViewDirection, WorldMatrixTransform.t, P1, P2, &t) == 1))
						{
							m_SUserStartMouseDisplace = (pCamera->GetTransformedWTM_().t + ViewDirection * t) - (WorldMatrixTransform.t + (AxisX * c) + (AxisY * c));
							m_SUserStartMousePosition = Vector(Position.x, Position.y, 0.f);

							return const_cast<CActor*>(m_pNode->m_pKey)->SetControlMode(SOEvent_ControlLockXY);
						}

						P0 = (WorldMatrixTransform.t + (AxisZ * c));
						P1 = (WorldMatrixTransform.t + (AxisZ * c) + (AxisY * c));
						P2 = (WorldMatrixTransform.t + (AxisY * c));

						if ((IntersectRayWithTriangle(pCamera->GetTransformedWTM_().t, pCamera->GetTransformedWTM_().t + ViewDirection, WorldMatrixTransform.t, P0, P1, &t) == 1) ||
							(IntersectRayWithTriangle(pCamera->GetTransformedWTM_().t, pCamera->GetTransformedWTM_().t + ViewDirection, WorldMatrixTransform.t, P1, P2, &t) == 1))
						{
							m_SUserStartMouseDisplace = (pCamera->GetTransformedWTM_().t + ViewDirection * t) - (WorldMatrixTransform.t + (AxisY * c) + (AxisZ * c));
							m_SUserStartMousePosition = Vector(Position.x, Position.y, 0.f);

							return const_cast<CActor*>(m_pNode->m_pKey)->SetControlMode(SOEvent_ControlLockYZ);
						}

						P0 = (WorldMatrixTransform.t + (AxisX * c));
						P1 = (WorldMatrixTransform.t + (AxisX * c) + (AxisZ * c));
						P2 = (WorldMatrixTransform.t + (AxisZ * c));

						if ((IntersectRayWithTriangle(pCamera->GetTransformedWTM_().t, pCamera->GetTransformedWTM_().t + ViewDirection, WorldMatrixTransform.t, P0, P1, &t) == 1) ||
							(IntersectRayWithTriangle(pCamera->GetTransformedWTM_().t, pCamera->GetTransformedWTM_().t + ViewDirection, WorldMatrixTransform.t, P1, P2, &t) == 1))
						{
							m_SUserStartMouseDisplace = (pCamera->GetTransformedWTM_().t + ViewDirection * t) - (WorldMatrixTransform.t + (AxisX * c) + (AxisZ * c));
							m_SUserStartMousePosition = Vector(Position.x, Position.y, 0.f);

							return const_cast<CActor*>(m_pNode->m_pKey)->SetControlMode(SOEvent_ControlLockXZ);
						}
					}
				}
			}
		}
		break;
		
		case MOUSE_MIDDLE:
		{
			m_SUserStartMousePosition = Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f);
			m_bSMiddleButtonPressed = (InputData.event == MOUSE_Pressed);
			return true;
		}break;
	};
	return false;
}

//----------------------------------------------------------------------------------------------
bool IDrawInterface::ProcessController(const MouseMoveInputData &InputData)
{
	bool bResult = false;

	EObjEditControlMode Mode = m_pCoreSDK->GetEditControlMode();

	if (m_pNode->m_pKey->GetControlState() == ActorState_Locked)
	{
		switch (m_pNode->m_pKey->GetControlMode())
		{
			// local object movement 
		case SOEvent_FreeMove:
		case SOEvent_ControlLockOrigin: // free object move
			{	
			}break;

		case SOEvent_ControlLockX:
			{
				if (Mode == EObjEditControlMode::ECM_Rotate) // rotate by axis
				{
					ProcessControllerRotateLocal(InputData);
				}
				else if (Mode == EObjEditControlMode::ECM_Scale) // scale by X axis
				{
					ProcessControllerScaleLocal(InputData);
				}
				else // move object by [X] axis
				{	
					ProcessControllerTranslate(InputData);
				}
				bResult = true;
			}break;

		case SOEvent_ControlLockY:
			{
				if (Mode == EObjEditControlMode::ECM_Rotate) // rotate by axis
				{
					ProcessControllerRotateLocal(InputData);
				}
				else if (Mode == EObjEditControlMode::ECM_Scale) // scale
				{
					ProcessControllerScaleLocal(InputData);
				}
				else
				{
					ProcessControllerTranslate(InputData);
				}
				bResult = true;
			}break;

		case SOEvent_ControlLockZ:
			{
				if (Mode == EObjEditControlMode::ECM_Rotate) // rotate by axis
				{
					ProcessControllerRotateLocal(InputData);
				}
				else if (Mode == EObjEditControlMode::ECM_Scale) // scale
				{
					ProcessControllerScaleLocal(InputData);
				}
				else
				{	// move object by [Y] axis
					ProcessControllerTranslate(InputData);
				}
				bResult = true;
			}break;

		case SOEvent_ControlLockXY:
			{
				if (Mode == EObjEditControlMode::ECM_Rotate) {// rotate by axis
				}
				else if (Mode == EObjEditControlMode::ECM_Scale) {// scale
				}
				else
				{	// move object by [Y] axis
					ProcessControllerTranslate(InputData);
				}
				bResult = true;
			}break;

		case SOEvent_ControlLockYZ:
			{
				if (Mode == EObjEditControlMode::ECM_Rotate) {// rotate by axis
				}
				else if (Mode == EObjEditControlMode::ECM_Scale) {// scale
				}
				else
				{	// move object by [Y] axis
					ProcessControllerTranslate(InputData);
				}
				bResult = true;
			}break;

		case SOEvent_ControlLockXZ:
			{
				if (Mode == EObjEditControlMode::ECM_Rotate) {// rotate by axis
				}
				else if (Mode == EObjEditControlMode::ECM_Scale) { // scale
				}
				else
				{	// move object by [Y] axis
					ProcessControllerTranslate(InputData);
				}
				bResult = true;
			}break;
		case SOEvent_ScaleTLXY: 
		case SOEvent_ScaleTRXY: 
		case SOEvent_ScaleBLXY:
		case SOEvent_ScaleRX:
		case SOEvent_ScaleLX:
		case SOEvent_ScaleTY:
		case SOEvent_ScaleBY:
		case SOEvent_ScaleBRXY:
			// TODO
			break;
		};
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool IDrawInterface::ProcessControllerRelease(const MouseInputData &InputData)
{
	bool bResult = false;

	if (m_pNode->m_pKey->GetControlState() == ActorState_Locked)
	{
		const_cast<CActor*>(m_pNode->m_pKey)->SetControlMode(SOEvent_None);
		const_cast<CActor*>(m_pNode->m_pKey)->SetControlState(ActorState_None, true); // drop mouse input lock upper to root
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool IDrawInterface::ProcessPress(const MouseInputData &InputData)
{
	bool bResult = false;

	if (!m_pNode->m_pKey->GetEnabled() || !GetVisible()){
		return bResult;
	}

/*	if (!m_pNode->m_pValue->GetCompositeBounds().HitTest(InputData.MousePos)){
		return false;
	}
		
	switch (InputData.event)
	{
	case MOUSE_Pressed:
	case MOUSE_DoubleClick:
		{
			if (m_pNode->m_pKey->GetControlState() == ActorState_None)
			{
				if (!m_pNode->m_pValue->GetCompositeBounds().HitTest(InputData.MousePos)){
					return false;
				}

				if (m_pNode->m_pKey->HitTest(InputData.MousePos)) // top most widget hit test
				{
					unsigned int State = m_pNode->m_pKey->GetStates();
					m_pNode->m_pValue->SetStates(State |= AS_PRESSED);

					//SetControlState(ActorState_Locked, true);	// set mouse input lock
					m_pNode->m_pValue->GainFocus();							// set active state focused

					m_pNode->m_pValue->DoEventPressed(InputData); 
					bResult = true;
				}
			}
		}break;

		case MOUSE_Released:
		{
			// iterate to low level locked object
			if (GetControlState() == ActorState_Locked)
			{
				for (TVecActorChildRevIterator Iter = ChildNodes.rbegin(); Iter != ChildNodes.rend(); ++Iter)
				{
					if ((*Iter)->GetControlState() == ActorState_Locked && 
						(*Iter)->OnEventPressed(InputData)) 
					{
						return true;
					}
				}

				// here lowest locked object 
				if (DoEventReleased(InputData)) // process deactivate widget
				{
					unsigned int State = GetStates();
					SetStates(State &= ~AS_PRESSED);

					SetControlState(ActorState_None, true); // drop mouse input lock upper to root
					bResult = true;
				}
			}
		}break;
	};
	*/
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool IDrawInterface::ProcessRelease(const MouseInputData &InputData)
{
	bool bResult = false;

	unsigned int State = m_pNode->m_pValue->GetVisualStates();

	if (State & AS_PRESSED)
	{
		CActor *pActor = const_cast<CActor*>(m_pNode->m_pKey);

		m_pCoreSDK->GetViewportManager()->SetFocus(this, false);

		const_cast<IDrawInterface*>(m_pNode->m_pValue)->SetVisualStates(State &= ~AS_PRESSED);
		
		pActor->DoEventReleased(InputData);

		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool IDrawInterface::ProcessMouseMove(const MouseMoveInputData &InputData)
{
	return OnMouseMove(InputData);
}

//----------------------------------------------------------------------------------------------
bool IDrawInterface::ProcessMouseWheel(float ds)
{
	return OnMouseWheel(ds);
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::ProcessControllerTranslate(const MouseMoveInputData &InputData)
{
	if (m_pCoreSDK->GetCameraManager())
	{
		const CCamera * pCamera = m_pCoreSDK->GetCameraManager()->GetActiveCamera(InputData.pRenderContext);

		if (pCamera)
		{
			Matrix InvView, I;
			Matrix WorldMatrixTransform = GetTransformedWTM_();

			invert(InvView, pCamera->GetViewMatrix());

			float Length = length(pCamera->GetTransformedWTM_().t - WorldMatrixTransform.t);

			Vector2f ViewportSize((float)InputData.pRenderContext->m_displayModeWidth, 
								  (float)InputData.pRenderContext->m_displayModeHeight);

			Vector ViewDirection = UnprojectViewport(ViewDirection, 
				pCamera->GetProjMatrix(), 
				pCamera->GetViewMatrix(),
				InputData.MousePos,
				ViewportSize);

			Vector CamStrafe(pCamera->GetStrafe());
			Vector TransDelta(pCamera->GetTransformedWTM_().t - WorldMatrixTransform.t);

			CamStrafe.normalize();

			float fCathetusOppositLen = TransDelta.Length() * ::tan(0.1f);
			Vector vCathetusOpposit = TransDelta.Cross(Vector(0.f, 1.f, 0.f));
			vCathetusOpposit.normalize();
			vCathetusOpposit *= fCathetusOppositLen;

			float k = CamStrafe.Dot(vCathetusOpposit); // projection length 

			float c = 0.3f;

			const Vector AxisX = I._row0 * k;
			const Vector AxisY = I._row1 * k;
			const Vector AxisZ = I._row2 * k;

			bool bTransformed = false;

			switch (m_pNode->m_pKey->GetControlMode())
			{
			case SOEvent_ControlLockX:
				{
					Vector PlaneNormal = cross(PlaneNormal, ViewDirection, InvView._row1);
					Vector Intersect = RayPlaneIntersect(pCamera->GetTransformedWTM_().t, PlaneNormal, (WorldMatrixTransform.t), I._row0);

					Vector TPos;
					GlobalToLocalTransform(TPos, Intersect - (m_SUserStartMouseDisplace * k /*+ AxisX*/));
					SetPosition_(TPos);
				
					bTransformed = true;
				}break;		

			case SOEvent_ControlLockY:
				{
					Vector PlaneNormal = cross(PlaneNormal, ViewDirection, InvView._row0);
					Vector Intersect = RayPlaneIntersect(pCamera->GetTransformedWTM_().t, PlaneNormal, WorldMatrixTransform.t, I._row1);

					Vector TPos = GlobalToLocalTransform(Intersect, Intersect - (m_SUserStartMouseDisplace * k/*+ AxisY*/));
					SetPosition_(TPos);

					bTransformed = true;
				}break;

			case SOEvent_ControlLockZ:
				{
					Vector PlaneNormal = cross(PlaneNormal, ViewDirection, InvView._row1);
					Vector Intersect = RayPlaneIntersect(pCamera->GetTransformedWTM_().t, PlaneNormal, WorldMatrixTransform.t, I._row2);

					Vector TPos = GlobalToLocalTransform(Intersect, Intersect - (m_SUserStartMouseDisplace * k/*+ AxisZ*/));
					SetPosition_(TPos);

					bTransformed = true;
				}break;

			case SOEvent_ControlLockXY:
				{
					Vector Intersect = RayPlaneIntersect(WorldMatrixTransform.t, I._row2, pCamera->GetTransformedWTM_().t, ViewDirection);

					Vector TPos = GlobalToLocalTransform(Intersect, Intersect - (m_SUserStartMouseDisplace + (AxisX * c) + (AxisY * c)));
					SetPosition_(TPos);

					bTransformed = true;
				}break;

			case SOEvent_ControlLockYZ:
				{
					Vector Intersect = RayPlaneIntersect(WorldMatrixTransform.t, I._row0, pCamera->GetTransformedWTM_().t, ViewDirection);

					Vector TPos = GlobalToLocalTransform(Intersect, Intersect - (m_SUserStartMouseDisplace + (AxisY * c) + (AxisZ * c)));
					SetPosition_(TPos);

					bTransformed = true;
				}break;

			case SOEvent_ControlLockXZ:
				{
					Vector Intersect = RayPlaneIntersect(WorldMatrixTransform.t, I._row1, pCamera->GetTransformedWTM_().t, ViewDirection);

					Vector TPos = GlobalToLocalTransform(Intersect, Intersect - (m_SUserStartMouseDisplace + (AxisX * c) + (AxisZ * c)));
					SetPosition_(TPos);

					bTransformed = true;
				}break;

			default:
				break;
			};

			if (bTransformed)
			{
				m_pCoreSDK->GetViewportManager()->RebuildTransform(const_cast<CActor*>(m_pNode->m_pKey));
				const_cast<CActor*>(m_pNode->m_pKey)->UpdateEntitiesChangeWorldPos(GetTransformedWTM_());
				const_cast<CActor*>(m_pNode->m_pKey)->BroadcastEvent(Event_OnChangePivot);
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::ProcessControllerRotateLocal(const MouseMoveInputData &InputData)
{
	if (m_pCoreSDK->GetCameraManager())
	{
		if (const CCamera *pCamera = m_pCoreSDK->GetCameraManager()->GetActiveCamera(InputData.pRenderContext))
		{
			Matrix InvView, I;
			Matrix WorldMatrixTransform = GetTransformedWTM_();

			invert(InvView, pCamera->GetViewMatrix());

			Vector2f ViewportSize((float)InputData.pRenderContext->m_displayModeWidth, 
								  (float)InputData.pRenderContext->m_displayModeHeight);

			Vector ViewDirection = UnprojectViewport(ViewDirection, 
				pCamera->GetProjMatrix(), 
				pCamera->GetViewMatrix(),
				InputData.MousePos,
				ViewportSize);

			Vector PrevViewDirection = UnprojectViewport(PrevViewDirection, 
				pCamera->GetProjMatrix(), 
				pCamera->GetViewMatrix(),
				Vector2f(m_SUserStartMousePosition.x, m_SUserStartMousePosition.y),
				ViewportSize);

			bool bTransformed = false;

			switch (m_pNode->m_pKey->GetControlMode())
			{
				case SOEvent_ControlLockX:
				{
					Vector Intersect = RayPlaneIntersect(WorldMatrixTransform.t, Vector(0.f, 0.f, 1.f), pCamera->GetTransformedWTM_().t, ViewDirection);
					Vector IntersectPrev = RayPlaneIntersect(WorldMatrixTransform.t, Vector(0.f, 0.f, 1.f), pCamera->GetTransformedWTM_().t, PrevViewDirection);

					Vector D1 = Intersect - WorldMatrixTransform.t;
					Vector D2 = IntersectPrev - WorldMatrixTransform.t;

					D1.normalize();
					D2.normalize();

					float Dot = D2.Dot(D1);

					if (Dot < 1.f) 
					{
						float Sign1 = D1.Dot(Vector(1.f, 0.f, 0.f));
						float Sign2 = D2.Dot(Vector(1.f, 0.f, 0.f));

						if (D1.y > 0.f)
						{
							Sign1 *= -1.f;
							Sign2 *= -1.f;
						}

						float fdelta = ((Sign1 > Sign2) ? 1.f : -1.f) * ::acosf(Dot);
						m_YawPitchRoll.x += fdelta;

						Quaternion rot(0.f, 0.f, 0.f, 1.f);
#ifdef ROT_N0
						rot.set_rot(fdelta, 0.f, 0.f);

						Matrix M;
						rot.Normalize();
						rot.ToMatrix(&M);

						Matrix LTM = GetLTM_();
						Vector t = LTM.t;
						LTM.t.Set(0.f, 0.f, 0.f);
						LTM = LTM * M;
						LTM.t = t;
						SetLTM_(LTM);
#else
						
						rot.set_rot(m_YawPitchRoll.x, m_YawPitchRoll.y, m_YawPitchRoll.z);

						Matrix M;
						rot.Normalize();
						rot.ToMatrix(&M);

						Matrix LTM = GetLTM_();
						M.t = LTM.t;
						SetLTM_(M);
#endif//
					}

					m_SUserStartMousePosition = Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f);
					bTransformed = true;
				}break;

				case SOEvent_ControlLockY:
				{
					Vector Intersect = RayPlaneIntersect(WorldMatrixTransform.t, Vector(1.f, 0.f, 0.f), pCamera->GetTransformedWTM_().t, ViewDirection);
					Vector IntersectPrev = RayPlaneIntersect(WorldMatrixTransform.t, Vector(1.f, 0.f, 0.f), pCamera->GetTransformedWTM_().t, PrevViewDirection);

					Vector D1 = Intersect - WorldMatrixTransform.t;
					Vector D2 = IntersectPrev - WorldMatrixTransform.t;

					D1.normalize();
					D2.normalize();

					float Dot = D2.Dot(D1);

					if (Dot < 1.f) 
					{
						float Sign1 = D1.Dot(Vector(0.f, 0.f, 1.f));
						float Sign2 = D2.Dot(Vector(0.f, 0.f, 1.f));

						if (D1.y < 0.f)
						{
							Sign1 *= -1.f;
							Sign2 *= -1.f;
						}

						float fdelta = ((Sign1 > Sign2) ? 1.f : -1.f) * ::acosf(Dot);
						m_YawPitchRoll.y += fdelta;

						Quaternion rot(0.f, 0.f, 0.f, 1.f);
#ifdef ROT_N0
						//rot.set_rot(0.f, fdelta, 0.f);
						rot.set_rot(0.f, 0.f, fdelta);

						Matrix M;
						rot.Normalize();
						rot.ToMatrix(&M);

						Matrix LTM = GetLTM_();
						Vector t = LTM.t;
						LTM.t.Set(0.f, 0.f, 0.f);
						LTM = LTM * M;
						LTM.t = t;
						SetLTM_(LTM);
#else
						rot.set_rot(m_YawPitchRoll.x, m_YawPitchRoll.y, m_YawPitchRoll.z);

						Matrix M;
						rot.Normalize();
						rot.ToMatrix(&M);

						Matrix LTM = GetLTM_();
						M.t = LTM.t;
						SetLTM_(M);
#endif//
					}
					
					m_SUserStartMousePosition = Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f);
					bTransformed = true;
				}break;

				case SOEvent_ControlLockZ:
				{
					Vector Intersect = RayPlaneIntersect(WorldMatrixTransform.t, Vector(0.f, 1.f, 0.f), pCamera->GetTransformedWTM_().t, ViewDirection);
					Vector IntersectPrev = RayPlaneIntersect(WorldMatrixTransform.t, Vector(0.f, 1.f, 0.f), pCamera->GetTransformedWTM_().t, PrevViewDirection);

					Vector D1 = Intersect - WorldMatrixTransform.t;
					Vector D2 = IntersectPrev - WorldMatrixTransform.t;

					D1.normalize();
					D2.normalize();

					float Dot = D2.Dot(D1);

					if (Dot < 1.f) 
					{
						float Sign1 = D1.Dot(Vector(0.f, 0.f, 1.f));
						float Sign2 = D2.Dot(Vector(0.f, 0.f, 1.f));

						if (D1.x > 0.f)
						{
							Sign1 *= -1.f;
							Sign2 *= -1.f;
						}

						float fdelta = ((Sign1 > Sign2) ? 1.f : -1.f) * ::acosf(Dot);
						m_YawPitchRoll.z += fdelta;

						Quaternion rot(0.f, 0.f, 0.f, 1.f);
#ifdef ROT_N0
						//rot.set_rot(0.f, 0.f, fdelta);
						rot.set_rot(0.f, fdelta, 0.f);

						Matrix M;
						rot.Normalize();
						rot.ToMatrix(&M);

						Matrix LTM = GetLTM_();
						Vector t = LTM.t;
						LTM.t.Set(0.f, 0.f, 0.f);
						LTM = LTM * M;
						LTM.t = t;
						SetLTM_(LTM);
#else
						rot.set_rot(m_YawPitchRoll.x, m_YawPitchRoll.y, m_YawPitchRoll.z);

						Matrix M;
						rot.Normalize();
						rot.ToMatrix(&M);

						Matrix LTM = GetLTM_();
						M.t = LTM.t;
						SetLTM_(M);
#endif//
					}

					m_SUserStartMousePosition = Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f);
					bTransformed = true;
				}break;
			};

			if (bTransformed)
			{
				m_pCoreSDK->GetViewportManager()->RebuildTransform(const_cast<CActor*>(m_pNode->m_pKey));
				const_cast<CActor*>(m_pNode->m_pKey)->UpdateEntitiesChangeWorldPos(GetTransformedWTM_());
				const_cast<CActor*>(m_pNode->m_pKey)->BroadcastEvent(Event_OnChangePivot);
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::ProcessControllerScaleLocal(const MouseMoveInputData &InputData)
{
	bool bTransformed = false;

	Vector2f DT;

	switch (m_pNode->m_pKey->GetControlMode())
	{
	case SOEvent_FreeMove:
	case SOEvent_ControlLockOrigin:
		{
			//Size += InputData.DeltaPos;
		}break;

	case SOEvent_ControlLockX:
		{
			//Size += Vector2f(InputData.DeltaPos.x, 0.f);
		}break;

	case SOEvent_ControlLockY:
		{
			//Size += Vector2f(0, InputData.DeltaPos.y);
		}break;
	};

	if (bTransformed)
	{
		m_pCoreSDK->GetViewportManager()->RebuildTransform(const_cast<CActor*>(m_pNode->m_pKey));
		const_cast<CActor*>(m_pNode->m_pKey)->UpdateEntitiesChangeWorldPos(GetTransformedWTM_());
		const_cast<CActor*>(m_pNode->m_pKey)->BroadcastEvent(Event_OnChangePivot);
	}
}

//----------------------------------------------------------------------------------------------
bool IDrawInterface::OnMouseMove(const MouseMoveInputData &InputData)
{
	return false;
}

//----------------------------------------------------------------------------------------------
bool IDrawInterface::OnMouseWheel(float ds)
{
	return false;
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::SetFocus(bool bFlag /*= true*/)
{
	if (bFlag)
	{
		if (m_VisualStates & AS_FOCUSED){ // already focused
			return;
		}
		SetVisualStates(m_VisualStates |= AS_FOCUSED);
	}else
	{
		SetVisualStates(m_VisualStates &= ~AS_FOCUSED);
	}
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::SetMouseOver(bool bFlag /*= true*/)
{
	if (bFlag)
	{
		SetVisualStates(m_VisualStates |= AS_MOUSE_OVER);
	}else{
		SetVisualStates(m_VisualStates &= ~AS_MOUSE_OVER);
	}
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::DoBuildWorldTransform_(const Matrix &WTM)
{
	m_WorldMatrixTransform = GetLTM_() * WTM * GetSTM_();

    m_Bounds.SetUnvalid(); // invalidate

    std::vector<IRenderInterface*> &vecRenderEntities = const_cast<CActor*>(m_pNode->m_pKey)->m_VecRenderEntities;
	for (std::vector<IRenderInterface*>::const_iterator iter = vecRenderEntities.begin(); iter != vecRenderEntities.end(); ++iter)
	{
		(*iter)->SetRWTM(m_WorldMatrixTransform);
	
		Bounds3f BBox = (*iter)->GetRBounds_();

		if (BBox.IsValid())
		{
            const unsigned int BBOX_POINTS = 8;

			Vector BoxPoints[BBOX_POINTS] =
			{ 
				/*Vector(BBox.bound_min.x, BBox.bound_min.y, BBox.bound_min.z),
				Vector(BBox.bound_min.x, BBox.bound_min.y, BBox.bound_max.z),
				Vector(BBox.bound_max.x, BBox.bound_min.y, BBox.bound_max.z),
				Vector(BBox.bound_max.x, BBox.bound_min.y, BBox.bound_min.z),

				Vector(BBox.bound_min.x, BBox.bound_max.y, BBox.bound_min.z),
				Vector(BBox.bound_min.x, BBox.bound_max.y, BBox.bound_max.z),
				Vector(BBox.bound_max.x, BBox.bound_max.y, BBox.bound_max.z),
				Vector(BBox.bound_max.x, BBox.bound_max.y, BBox.bound_min.z),*/

                // TODO make homogeneous with exporter tool
                Vector(BBox.bound_min.z, BBox.bound_min.y, BBox.bound_min.x),
                Vector(BBox.bound_max.z, BBox.bound_min.y, BBox.bound_min.x),
                Vector(BBox.bound_max.z, BBox.bound_min.y, BBox.bound_max.x),
                Vector(BBox.bound_min.z, BBox.bound_min.y, BBox.bound_max.x),

                Vector(BBox.bound_min.z, BBox.bound_max.y, BBox.bound_min.x),
                Vector(BBox.bound_max.z, BBox.bound_max.y, BBox.bound_min.x),
                Vector(BBox.bound_max.z, BBox.bound_max.y, BBox.bound_max.x),
                Vector(BBox.bound_min.z, BBox.bound_max.y, BBox.bound_max.x),
			}; 

			Matrix MT = m_WorldMatrixTransform;
			MT.t = Vector(0.f, 0.f, 0.f);

            Bounds3f tmpBound;
			for (int Index = 0; Index < BBOX_POINTS; ++Index)
			{
				Vector point = transform_coord(BoxPoints[Index], MT);
                
                if (Index == 0)
                {
                    tmpBound.SetBounds(point, point);
                    continue;
                }
                tmpBound.Add(point);
			}

            const Vector bound_min = m_WorldMatrixTransform.t + tmpBound.bound_min;
            const Vector bound_max = m_WorldMatrixTransform.t + tmpBound.bound_max;

            (*iter)->SetWBounds(Bounds3f(bound_min, bound_max));

            if (iter == vecRenderEntities.begin())
            {
                m_Bounds.SetBounds(bound_min, bound_max);
                continue;
            }
            else
            {
                m_Bounds.Add(bound_min);
                m_Bounds.Add(bound_max);
            }
		}
	}

    // invalidate composite bounds
    m_CompositeBounds = m_Bounds;
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::DoBuildCompounds()
{

}

//----------------------------------------------------------------------------------------------
/** Transforms local space position in to global space point */
 Vector& IDrawInterface::LocalToGlobalTransform(Vector& OutGlobalPoint, const Vector& InLocalPoint) const
 {
	 transform_coord(OutGlobalPoint, InLocalPoint, m_WorldMatrixTransform);

	 return OutGlobalPoint;
 }

 //----------------------------------------------------------------------------------------------
 Vector& IDrawInterface::GlobalToLocalTransform(Vector& OutLocalPoint, const Vector& InGlobalPoint) const
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
 Matrix IDrawInterface::CalcWorldTransform() const
 {
	 Matrix OutMatrix;

	 std::vector<const IDrawInterface*> TmpActorList;

	 TNodeMap<class CActor, class IDrawInterface> *IterActor = m_pNode->GetRootNode();

	 while (IterActor) // do not allow top level root processing
	 {				   // use it as origin for child nodes
		 TmpActorList.push_back(IterActor->m_pValue);
		 IterActor = IterActor->GetRootNode();
	 }

	 // build global space matrix
	 for (std::vector<const IDrawInterface*>::reverse_iterator Iter = TmpActorList.rbegin(); Iter != TmpActorList.rend(); ++Iter)
	 {
		 OutMatrix = OutMatrix * (*Iter)->GetPivot_();
	 }
	 return OutMatrix;
 }


 //----------------------------------------------------------------------------------------------
 bool IDrawInterface::TransformObjectAToObjectBLocalPos(CActor * ObjectA, const CActor * ObjectB, Vector& LocalBPos/* = Vector(0.f, 0.f, 0.f)*/, bool bKeepAOrientation /*= true*/)
 {
	 bool bResult = false;
	 /*
	 if (ObjectA && ObjectB)
	 {
		 Vector GlobalPoint;
		 ObjectB->LocalToGlobalTransform(GlobalPoint, LocalBPos);

		 Vector LocalPoint;
		 ObjectA->GlobalToLocalTransform(LocalPoint, GlobalPoint);
		 ObjectA->SetPosition(LocalPoint);

		 // common parent object
		 // 		CActor * ObjectC = NULL;
		 // 
		 // 		// try to find common root node
		 // 		CActor * ParentB = ObjectB->GetParent();
		 // 		while (ParentB && !ObjectC)
		 // 		{
		 // 			CActor * ParentA = const_cast<CActor*>(ObjectA);
		 // 			while (ParentA != NULL)
		 // 			{
		 // 				if (ParentB == ParentA)
		 // 				{
		 // 					ObjectC = ParentA;
		 // 					break;
		 // 				}
		 // 				ParentA = ParentA->GetParent();
		 // 			}
		 // 			ParentB = ParentB->GetParent();
		 // 		}
		 // 
		 // 		// common root node found
		 // 		if (ObjectC != NULL)
		 // 		{		
		 // 			std::vector<const CActor*> TmpActorListA, TmpActorListB;
		 // 
		 // 			// build B-way array 
		 // 			CActor * IterActor = const_cast<CActor*>(ObjectB);
		 // 			while (IterActor != NULL) 
		 // 			{
		 // 				TmpActorListB.push_back(IterActor);
		 // 				IterActor = IterActor->GetParent();
		 // 			}
		 // 
		 // 			// build A array
		 // 			IterActor = const_cast<CActor*>(ObjectA);
		 // 			while (IterActor != NULL) 
		 // 			{
		 // 				TmpActorListA.push_back(IterActor);
		 // 				IterActor = IterActor->GetParent();
		 // 			}
		 // 
		 // 			// build [Parent-ObjectB] space matrix
		 // 			Matrix CA_LTM, CB_LTM;
		 // 			for (std::vector<const CActor*>::reverse_iterator Iter = TmpActorListB.rbegin(); Iter != TmpActorListB.rend(); ++Iter)
		 // 			{
		 // 				CB_LTM = CB_LTM * (*Iter)->GetPivot();
		 // 			}
		 // 
		 // 			// build [Parent-ObjectA] local transform
		 // 			for (std::vector<const CActor*>::reverse_iterator Iter = TmpActorListA.rbegin(); Iter != TmpActorListA.rend(); ++Iter)
		 // 			{
		 // 				CA_LTM = CA_LTM * (*Iter)->GetPivot();
		 // 			}
		 // 
		 // 			// add local shift
		 //  			Matrix LTM_LBP = ObjectB->GetPivot();
		 //  			LTM_LBP.t = LocalBPos;
		 //  			CB_LTM = CB_LTM * LTM_LBP;
		 // 
		 //  			Vector V = CB_LTM.t - CA_LTM.t;
		 // 
		 // 			// calc projection to local axis
		 //  			Vector AxisX = CA_LTM._row0;
		 //  			Vector AxisY = CA_LTM._row1;
		 //  			Vector AxisZ = CA_LTM._row2;
		 //  
		 //  			AxisX.normalize();
		 //  			AxisY.normalize();
		 //  			AxisZ.normalize();
		 // 
		 // 			Vector LocalPoint = ObjectA->m_LTM.t + Vector(V.Dot(AxisX), V.Dot(AxisY), V.Dot(AxisZ));
		 //  			ObjectA->SetPosition(LocalPoint);
		 bResult = true;
		 //		}
	 }*/
	 return bResult;
 }

 //----------------------------------------------------------------------------------------------
 bool IDrawInterface::GetObjectAInLocalSpaceB(Matrix &outLTM, const IDrawInterface *pObjectA, const IDrawInterface *pObjectB)
 {
	 bool bResult = false;

	 if (pObjectA && pObjectB)
	 {
		 CCoreSDK *pApp =  pObjectA->GetCoreSDK();
		 assert(pApp);

		 // common parent object
		 CActor *pObjectC = nullptr;
		 CActor *pActorA = const_cast<CActor*>(pObjectA->m_pNode->m_pKey);
		 CActor *pActorB = const_cast<CActor*>(pObjectB->m_pNode->m_pKey);

		 // common root node found
		 if (CActor::FindCommonParent(&pObjectC, pActorA, pActorB))
		 {
			 std::vector<const CActor*> tmpActorListA, tmpActorListB;

			 // build B-way array 
			 CActor *pIterActor = const_cast<CActor*>(pActorB);
			 while (pIterActor != pObjectC) 
			 {
				 tmpActorListB.push_back(pIterActor);
				 pIterActor = pIterActor->GetParent();
			 }

			 // build A array
			 pIterActor = const_cast<CActor*>(pActorA);
			 while (pIterActor != pObjectC) 
			 {
				 tmpActorListA.push_back(pIterActor);
				 pIterActor = pIterActor->GetParent();
			 }

			 // build [Parent - ObjectB] space matrix
			 Matrix CA_LTM, CB_LTM; 
			 for (std::vector<const CActor*>::reverse_iterator Iter = tmpActorListB.rbegin(); Iter != tmpActorListB.rend(); ++Iter)
			 {
				 IDrawInterface *pIActor = pApp->GetViewportManager()->GetByActor(const_cast<CActor*>(*Iter));
				 if (pIActor){
					CB_LTM = CB_LTM * (pIActor->GetLTM_());
				 }
				 
			 }
			 // [Parent - ObjectA] local transform
			 for (std::vector<const CActor*>::reverse_iterator Iter = tmpActorListA.rbegin(); Iter != tmpActorListA.rend(); ++Iter)
			 {
				 IDrawInterface *pIActor = pApp->GetViewportManager()->GetByActor(const_cast<CActor*>(*Iter));
				 if (pIActor){
					CA_LTM = CA_LTM * (pIActor->GetLTM_());
				 }
			 }

			 Matrix ICA_LTM;	
			 outLTM = invert(ICA_LTM, CA_LTM) * CB_LTM;
			 bResult = true;
		 }
	 }
	 return bResult;
 }

 //----------------------------------------------------------------------------------------------
 /** Calculates object world matrix.*/
 Matrix IDrawInterface::GetParentWorldTransform(const CActor* Object)
 {
	 Matrix WTM; 
	/* std::vector<const CActor*> TmpActorList;

	 CActor * IterActor = Object->GetParent();
	 while (IterActor)
	 {
		 TmpActorList.push_back(IterActor);
		 IterActor = IterActor->GetParent();
	 }

	 // build global space matrix
	 
	 for (std::vector<const CActor*>::reverse_iterator Iter = TmpActorList.rbegin(); Iter != TmpActorList.rend(); ++Iter)
	 {
		 WTM = WTM * (*Iter)->GetPivot() * (*Iter)->GetSTM();
	 }*/
	 return WTM;
 }