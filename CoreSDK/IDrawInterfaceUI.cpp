#include "coresdkafx.h"

static const oes::common_base::EPixelTransform TransformDef = PixelOwner_Transform;
static const float fDefaultDim = 100.f;

BEGIN_AUTO_CLASS(IDrawInterfaceUI)
	//new oes::rflex::PropertyPixelTransform("HPixelTransform", (BYTE*)&((IDrawInterfaceUI*)NULL)->m_HPixelTransform - (BYTE*)&((IDrawInterfaceUI*)NULL)->___startObjectMarkerIDrawInterfaceUI, "IDrawInterfaceUI", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, ((const int*)&TransformDef)),
    new oes::rflex::TProperty<int, IDrawInterfaceUI>("HPixelTransform", "IDrawInterfaceUI", "Value",
        [&](const void *ptr, const char *v)
        {
            void *nc_ptr = const_cast<void*>(ptr);
            IDrawInterfaceUI *act = dynamic_cast<IDrawInterfaceUI*>(reinterpret_cast<CObjectAbstract*>(nc_ptr));

            int iValue = 0;
            if (strcmp(v, "PixelOwner_Transform") == 0) {
                iValue = PixelOwner_Transform;
            }
            else if (strcmp(v, "PixelViewport_Transform") == 0) {
                iValue = PixelViewport_Transform;
            }
            else if (strcmp(v, "PersentageOwner_Transform") == 0) {
                iValue = PersentageOwner_Transform;
            }
            else {
                iValue = PersentageViewport_Transform;
            }
            act->m_HPixelTransform = iValue;
        },
        [&](const void *ptr, char *out){}),
        //new oes::rflex::PropertyPixelTransform("VPixelTransform", (BYTE*)&((IDrawInterfaceUI*)NULL)->m_VPixelTransform - (BYTE*)&((IDrawInterfaceUI*)NULL)->___startObjectMarkerIDrawInterfaceUI, "IDrawInterfaceUI", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, ((const int*)&TransformDef)),
        new oes::rflex::TProperty<int, IDrawInterfaceUI>("VPixelTransform", "IDrawInterfaceUI", "Value",
                [&](const void *ptr, const char *v)
        {
            void *nc_ptr = const_cast<void*>(ptr);
            IDrawInterfaceUI *act = dynamic_cast<IDrawInterfaceUI*>(reinterpret_cast<CObjectAbstract*>(nc_ptr));

            int iValue = 0;
            if (strcmp(v, "PixelOwner_Transform") == 0) {
                iValue = PixelOwner_Transform;
            }
            else if (strcmp(v, "PixelViewport_Transform") == 0) {
                iValue = PixelViewport_Transform;
            }
            else if (strcmp(v, "PersentageOwner_Transform") == 0) {
                iValue = PersentageOwner_Transform;
            }
            else {
                iValue = PersentageViewport_Transform;
            }
            act->m_VPixelTransform = iValue;
        },
        [&](const void *ptr, char *out){}),
        //new oes::rflex::PropertyPixelTransform("HSizeTransform", (BYTE*)&((IDrawInterfaceUI*)NULL)->m_HSizeTransform - (BYTE*)&((IDrawInterfaceUI*)NULL)->___startObjectMarkerIDrawInterfaceUI, "IDrawInterfaceUI", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, ((const int*)&TransformDef)),
        new oes::rflex::TProperty<int, IDrawInterfaceUI>("HSizeTransform", "IDrawInterfaceUI", "Value",
                [&](const void *ptr, const char *v)
        {
            void *nc_ptr = const_cast<void*>(ptr);
            IDrawInterfaceUI *act = dynamic_cast<IDrawInterfaceUI*>(reinterpret_cast<CObjectAbstract*>(nc_ptr));

            int iValue = 0;
            if (strcmp(v, "PixelOwner_Transform") == 0) {
                iValue = PixelOwner_Transform;
            }
            else if (strcmp(v, "PixelViewport_Transform") == 0) {
                iValue = PixelViewport_Transform;
            }
            else if (strcmp(v, "PersentageOwner_Transform") == 0) {
                iValue = PersentageOwner_Transform;
            }
            else {
                iValue = PersentageViewport_Transform;
            }
            act->m_HSizeTransform = iValue;
        },
        [&](const void *ptr, char *out) {}),
        //new oes::rflex::PropertyPixelTransform("VSizeTransform", (BYTE*)&((IDrawInterfaceUI*)NULL)->m_VSizeTransform - (BYTE*)&((IDrawInterfaceUI*)NULL)->___startObjectMarkerIDrawInterfaceUI, "IDrawInterfaceUI",	"Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, ((const int*)&TransformDef)),
            new oes::rflex::TProperty<int, IDrawInterfaceUI>("VSizeTransform", "IDrawInterfaceUI", "Value",
                [&](const void *ptr, const char *v)
        {
            void *nc_ptr = const_cast<void*>(ptr);
            IDrawInterfaceUI *act = dynamic_cast<IDrawInterfaceUI*>(reinterpret_cast<CObjectAbstract*>(nc_ptr));

            int iValue = 0;
            if (strcmp(v, "PixelOwner_Transform") == 0) {
                iValue = PixelOwner_Transform;
            }
            else if (strcmp(v, "PixelViewport_Transform") == 0) {
                iValue = PixelViewport_Transform;
            }
            else if (strcmp(v, "PersentageOwner_Transform") == 0) {
                iValue = PersentageOwner_Transform;
            }
            else {
                iValue = PersentageViewport_Transform;
            }
            act->m_VSizeTransform = iValue;
        },
        [&](const void *ptr, char *out) {}),
        
        //new oes::rflex::PropertyFLOAT("Width", (BYTE*)&((IDrawInterfaceUI*)NULL)->m_Width - (BYTE*)&((IDrawInterfaceUI*)NULL)->___startObjectMarkerIDrawInterfaceUI, "IDrawInterfaceUI", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fDefaultDim),
        new oes::rflex::TProperty<std::string, IDrawInterfaceUI>("Width", "IDrawInterfaceUI", "Value",
        [&](const void *ptr, const char *v)
        {
            void *nc_ptr = const_cast<void*>(ptr);
            IDrawInterfaceUI *act = dynamic_cast<IDrawInterfaceUI*>(reinterpret_cast<CObjectAbstract*>(nc_ptr));
            act->m_Width = (float)atof(v);
        },
        [&](const void *ptr, char *out){}),
        //new oes::rflex::PropertyFLOAT("Height", (BYTE*)&((IDrawInterfaceUI*)NULL)->m_Height - (BYTE*)&((IDrawInterfaceUI*)NULL)->___startObjectMarkerIDrawInterfaceUI, "IDrawInterfaceUI", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fDefaultDim),
        new oes::rflex::TProperty<std::string, CObjectAbstract>("Height", "IDrawInterfaceUI", "Value",
        [&](const void *ptr, const char *v)
        {
            void *nc_ptr = const_cast<void*>(ptr);
            IDrawInterfaceUI *act = dynamic_cast<IDrawInterfaceUI*>(reinterpret_cast<CObjectAbstract*>(nc_ptr));
            act->m_Height = (float)atof(v);
        },
        [&](const void *ptr, char *out){}),
    END_AUTO_CLASS(IDrawInterfaceUI)
CLASS_INSTANCE_INTERFACE(IDrawInterfaceUI);

//----------------------------------------------------------------------------------------------
IDrawInterfaceUI::IDrawInterfaceUI(const CObjectAbstract *pParent)
: IDrawInterface(pParent)
, m_Size(fDefaultDim, fDefaultDim)
, m_SizeTransformed(1.f, 1.f)
, m_bPushClipping(false)
{
	m_PixelTransform[H_Transform] = m_PixelTransform[V_Transform] = TransformDef;
	m_SizeTransform[H_Transform] = m_SizeTransform[V_Transform] = TransformDef;
}

//----------------------------------------------------------------------------------------------
IDrawInterfaceUI::IDrawInterfaceUI(const IDrawInterfaceUI &Source)
: IDrawInterface(Source)
{
}

//----------------------------------------------------------------------------------------------
IDrawInterfaceUI::~IDrawInterfaceUI()
{

}

//----------------------------------------------------------------------------------------------
void IDrawInterfaceUI::RegisterDrawInterface(const CActor *Src, int SrcParent /*= 1*/)
{
	IDrawInterface::RegisterDrawInterface(Src, SrcParent);

	const_cast<CActor*>(Src)->RegisterInterfaceImpl(this);
}

//----------------------------------------------------------------------------------------------
bool IDrawInterfaceUI::PrePropertyChangeIntf(const char *PropertyName)
{
	EPixelTransform NewValue = PixelOwner_Transform;

	if (!stricmp(PropertyName, "PixelOwner_Transform")){
		NewValue = PixelOwner_Transform;
	}
	else if (!stricmp(PropertyName, "PixelViewport_Transform")){
		NewValue = PixelViewport_Transform;
	}
	else if (!stricmp(PropertyName, "PersentageOwner_Transform")){
		NewValue = PersentageOwner_Transform;
	}
	else if (!stricmp(PropertyName, "PersentageViewport_Transform")){
		NewValue = PersentageViewport_Transform;
	}

	if (!stricmp(PropertyName, "HPixelTransform")){
		ConvertPixelTransform_H(GetPixelTransform_H(), NewValue);
	}
	else if (!stricmp(PropertyName, "VPixelTransform")){
		ConvertPixelTransform_V(GetPixelTransform_V(), NewValue);
	}
	else if (!stricmp(PropertyName, "HSizeTransform")){
		ConvertSizeTransform_H(GetSizePixelTransform_H(), NewValue);
	}
	else if (!stricmp(PropertyName, "VSizeTransform")){
		ConvertSizeTransform_V(GetSizePixelTransform_V(), NewValue);
	}
	return IDrawInterface::PrePropertyChangeIntf(PropertyName);
}

//----------------------------------------------------------------------------------------------
void IDrawInterfaceUI::OnPropertyChangedIntf(const char *PropertyName)
{
	if (!strcmp(PropertyName, "HPixelTransform") ||
		!strcmp(PropertyName, "VPixelTransform") ||
		!strcmp(PropertyName, "HSizeTransform") ||
		!strcmp(PropertyName, "VSizeTransform") ||
		!strcmp(PropertyName, "Height") ||
		!strcmp(PropertyName, "Width"))
	{
		m_pCoreSDK->GetViewportManager()->RebuildTransform(const_cast<CActor*>(m_pNode->m_pKey));
	}

	IDrawInterface::OnPropertyChangedIntf(PropertyName);
}

//----------------------------------------------------------------------------------------------
void IDrawInterfaceUI::DrawController() const
{
	return;
#ifndef FINAL_RELEASE
	//if (GetAppMain()->IsAEditor() && !GetAppMain()->IsAGame())
	/*
		if (m_pNode->m_pValue->IsFocused())
		{	
			assert(m_pCoreSDK && m_pCoreSDK->GetRegistry());

            core_sdk_api::TIViewport *pViewportInterface = m_pCoreSDK->GetExplorerInstance()->GetExplorer2D();
			
			Matrix WorldMatrixTransform = GetTransformedWTM_();

			Matrix IWTM;
			invert(IWTM, WorldMatrixTransform);

			Matrix ViewMatrix = pViewportInterface->GetViewportViewMatrix();

			float AxisLength = m_pCoreSDK->GetRegistry()->ControlAxisX.Length();
			Vector2f AxisX = (IWTM.__row0 *  AxisLength) * (1.f / ViewMatrix.__row0.Length());
			Vector2f AxisY = (IWTM.__row1 * -AxisLength) * (1.f / ViewMatrix.__row0.Length());

			// render origin box
			m_pCoreSDK->GetRenderSDK()->_DebugDrawRectangle2D(WorldMatrixTransform.t2, 
														WorldMatrixTransform.t2 + transform_coord(AxisX * 0.1f, IWTM),
														WorldMatrixTransform.t2 + transform_coord((AxisX + AxisY) * 0.1f, IWTM),
														WorldMatrixTransform.t2 + transform_coord(AxisY * 0.1f, IWTM), 
														(m_pNode->m_pKey->GetControlMode() == SOEvent_ControlLockOrigin) ? COLOR_YELLOW : 0xff000000);
			// render axis
			m_pCoreSDK->GetRenderSDK()->DrawLine(WorldMatrixTransform.t2,
										   WorldMatrixTransform.t2 + transform_coord(AxisX - (AxisX * 0.05f), IWTM), (m_pNode->m_pKey->GetControlMode() == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED, false);
			m_pCoreSDK->GetRenderSDK()->DrawLine(WorldMatrixTransform.t2,
										   WorldMatrixTransform.t2 + transform_coord(AxisY - (AxisY * 0.05f), IWTM), (m_pNode->m_pKey->GetControlMode() == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE, false);
			
			// render arrow x
			Vector2f AX0 = (AxisX + (AxisX * 0.05f));
			Vector2f AX1 = (AxisX - (AxisX * 0.05f)) - (AxisY * 0.05f);
			Vector2f AX2 = (AxisX - (AxisX * 0.05f)) + (AxisY * 0.05f);
		
			// render arrow y
			Vector2f AY0 =  AxisY + (AxisY * 0.05f);
			Vector2f AY1 = (AxisY - (AxisY * 0.05f)) - (AxisX * 0.05f);
			Vector2f AY2 = (AxisY - (AxisY * 0.05f)) + (AxisX * 0.05f);

			m_pCoreSDK->GetRenderSDK()->DrawTriangle2D(WorldMatrixTransform.t2 + transform_coord(AX0, IWTM),
												 WorldMatrixTransform.t2 + transform_coord(AX1, IWTM),
												 WorldMatrixTransform.t2 + transform_coord(AX2, IWTM), (m_pNode->m_pKey->GetControlMode() == SOEvent_ControlLockX) ? COLOR_YELLOW : COLOR_RED);

			m_pCoreSDK->GetRenderSDK()->DrawTriangle2D(WorldMatrixTransform.t2 + transform_coord(AY1, IWTM),
												 WorldMatrixTransform.t2 + transform_coord(AY0, IWTM),
												 WorldMatrixTransform.t2 + transform_coord(AY2, IWTM), (m_pNode->m_pKey->GetControlMode() == SOEvent_ControlLockY) ? COLOR_YELLOW : COLOR_BLUE);
		}
	*/
#endif//FINAL_RELEASE
}

//----------------------------------------------------------------------------------------------
void IDrawInterfaceUI::DrawBounds() const
{
	return;
#ifndef FINAL_RELEASE
	//if (GetAppMain()->IsAEditor() && !GetAppMain()->IsAGame())
	{
/*		if (m_pNode->m_pValue->IsFocused())
		{
			Matrix WorldMatrixTransform = GetTransformedWTM_();
					
			Matrix IWTM;
			invert(IWTM, WorldMatrixTransform);

			Vector2f SizeTransformed = GetTransformedSize_();
			
			Vector2f AxisX(SizeTransformed.x, 0.f);
			Vector2f AxisY(0.f, SizeTransformed.y);
			Vector2f AxisXY = AxisX + AxisY;

			m_pCoreSDK->GetRenderSDK()->DrawLine(WorldMatrixTransform.t2 + transform_coord(Vector2f(0.f, 0.f), IWTM),
												 WorldMatrixTransform.t2 + transform_coord(AxisX, IWTM), COLOR_YELLOW);
			m_pCoreSDK->GetRenderSDK()->DrawLine(WorldMatrixTransform.t2 + transform_coord(AxisX, IWTM),
												 WorldMatrixTransform.t2 + transform_coord(AxisXY, IWTM), COLOR_YELLOW);
			m_pCoreSDK->GetRenderSDK()->DrawLine(WorldMatrixTransform.t2 + transform_coord(AxisXY, IWTM),
												 WorldMatrixTransform.t2 + transform_coord(AxisY, IWTM), COLOR_YELLOW);
			m_pCoreSDK->GetRenderSDK()->DrawLine(WorldMatrixTransform.t2 + transform_coord(AxisY, IWTM),
												 WorldMatrixTransform.t2 + transform_coord(Vector2f(0.f, 0.f), IWTM), COLOR_YELLOW);
			
			Bounds3f Bounds = GetBounds_();
			Bounds3f CompositeBounds = GetCompositeBounds_();

			if (m_pCoreSDK->GetObjectBoundsVisible() && Bounds.IsValid())
			{
				m_pCoreSDK->GetRenderSDK()->DrawBounds2f(WorldMatrixTransform, Bounds, COLOR_BLUE);
			}
			if (m_pCoreSDK->GetSparitalSubdivisionVisible() && CompositeBounds.IsValid())
			{
				Matrix I;
				m_pCoreSDK->GetRenderSDK()->DrawBounds2f(I, CompositeBounds, COLOR_GREEN);
			}
		}*/
	}
#endif//FINAL_RELEASE*/
}

//----------------------------------------------------------------------------------------------
bool IDrawInterfaceUI::ProcessController(const MouseInputData &InputData)
{
	bool bResult = false;
/*
	const Vector2f &Position = InputData.MousePos;

	switch (InputData.Code)
	{
	case MOUSE_LEFT:
		{
			if (m_pNode->m_pValue->IsFocused())
			{
                core_sdk_api::TIViewport *pInterface = m_pCoreSDK->GetExplorerInstance()->GetExplorer2D();
 
 				assert(pInterface);

				Matrix WorldMatrixTransform = GetTransformedWTM_();
 
 				//calculate origin mouse position 
 				Vector2f IPosition = Position - WorldMatrixTransform.t2;
 
 				Matrix IWTM, ViewMatrix;
 				invert(IWTM, WorldMatrixTransform);
 
 				ViewMatrix = pInterface->GetViewportViewMatrix();
 
 				float AxisLength = m_pCoreSDK->GetRegistry()->ControlAxisX.Length();
 				Vector2f AxisX = (IWTM.__row0 *  AxisLength) * (1.f / ViewMatrix.__row0.Length());
 				Vector2f AxisY = (IWTM.__row1 * -AxisLength) * (1.f / ViewMatrix.__row0.Length());
 
 				Vector2f &ControlBox = m_pCoreSDK->GetRegistry()->ControlBox * (1.f / ViewMatrix.__row0.Length());
	 			Vector2f &ControlBoxOrigin = m_pCoreSDK->GetRegistry()->ControlBoxOrigin * (1.f / ViewMatrix.__row0.Length());
 
 				// check controls 2D
 				if (IsPointInRect(Position.x, Position.y, 
 								  WorldMatrixTransform.t2 - Vector2f(0.f, ControlBoxOrigin.y),
 								  WorldMatrixTransform.t2 + Vector2f(ControlBoxOrigin.x, 0.f)))
 				{
					m_SUserStartMousePosition = Vector(Position.x, Position.y, 0.f);

					Vector2f disp = InputData.MousePos - WorldMatrixTransform.t2;
					m_SUserStartMouseDisplace = Vector(disp.x, disp.y, 0.f);

 					return const_cast<CActor*>(m_pNode->m_pKey)->SetControlMode(SOEvent_ControlLockOrigin);
 				}
 				
				if (IsPointInRect(Position.x, Position.y, 
	 							  WorldMatrixTransform.t2 + transform_coord(AxisX, IWTM) - ControlBox, 
	 							  WorldMatrixTransform.t2 + transform_coord(AxisX, IWTM) + ControlBox))
	 			{
	 				m_SUserStartMousePosition = Vector(Position.x, Position.y, 0.f);

					Vector2f disp = InputData.MousePos - WorldMatrixTransform.t2;
					m_SUserStartMouseDisplace = Vector(disp.x, disp.y, 0.f);

	 				return const_cast<CActor*>(m_pNode->m_pKey)->SetControlMode(SOEvent_ControlLockX);
	 			}
	 			
				if (IsPointInRect(Position.x, Position.y, 
	 							  WorldMatrixTransform.t2 + transform_coord(AxisY, IWTM) - ControlBox, 
	 							  WorldMatrixTransform.t2 + transform_coord(AxisY, IWTM) + ControlBox))
	 			{
	 				m_SUserStartMousePosition = Vector(Position.x, Position.y, 0.f);

					Vector2f disp = InputData.MousePos - WorldMatrixTransform.t2;
					m_SUserStartMouseDisplace = Vector(disp.x, disp.y, 0.f);

	 				return const_cast<CActor*>(m_pNode->m_pKey)->SetControlMode(SOEvent_ControlLockY);
	 			}
			}
		}
		break;

		case MOUSE_MIDDLE:
		{
			m_SUserStartMousePosition = Vector(InputData.MousePos.x, InputData.MousePos.y, 0.f);
			m_bSMiddleButtonPressed = (InputData.event == MOUSE_Pressed);
		}break;
	};
	*/
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool IDrawInterfaceUI::ProcessController(const MouseMoveInputData &InputData)
{
	bool bResult = false;

	/*if (m_pNode->m_pKey->GetControlState() == ActorState_Locked)
	{
		switch (m_pNode->m_pKey->GetControlMode())
 		{
 			// local object movement 
 		case SOEvent_FreeMove:
 		case SOEvent_ControlLockOrigin: // free object move
 			{	
 				if (InputData.ModifKey & MK_Shift){
 					ProcessControllerScaleLocal(InputData);
 				}
 				else{
					ProcessControllerTranslate(InputData);
 				}
				bResult = true;
 			}break;
 
 		case SOEvent_ControlLockX:
 			{
 				if (InputData.ModifKey & MK_Control) { // rotate by axis
 					ProcessControllerRotateLocal(InputData);
 				}
 				else if (InputData.ModifKey & MK_Shift) { // scale by X axis
 					ProcessControllerScaleLocal(InputData);
 				}
 				else { // move object by [X] axis
 					ProcessControllerTranslate(InputData);
 				}
				bResult = true;
 			}break;
 
 		case SOEvent_ControlLockY:
 			{
 				if (InputData.ModifKey & MK_Control){ // rotate by axis
 					ProcessControllerRotateLocal(InputData);
 				}
 				else if (InputData.ModifKey & MK_Shift){ // scale
 					ProcessControllerScaleLocal(InputData);
 				}
 				else{
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
		
		if (bResult)
		{
			m_pCoreSDK->GetViewportManager()->RebuildTransform(const_cast<CActor*>(m_pNode->m_pKey));
			const_cast<CActor*>(m_pNode->m_pKey)->BroadcastEvent(Event_OnChangePivot);
		}
	}*/
	return bResult;
}

//----------------------------------------------------------------------------------------------
void IDrawInterfaceUI::ProcessControllerTranslate(const MouseMoveInputData &InputData)
{
 /*	Vector OutLocalPoint;
 	Vector2f WorldMousePosition = InputData.MousePos - Vector2f(m_SUserStartMouseDisplace.x, m_SUserStartMouseDisplace.y);
 
	bool bTransformed = false;

 	switch (m_pNode->m_pKey->GetControlMode())
 	{
 	case SOEvent_FreeMove:
 	case SOEvent_ControlLockOrigin:
 		{
 			OutLocalPoint = GlobalToLocalTransform(OutLocalPoint, Vector(WorldMousePosition.x, WorldMousePosition.y, 0.f));
 			SetPosition_(OutLocalPoint);

			bTransformed = true;
 		}break;
 
 	case SOEvent_ControlLockX:
 		{
 			WorldMousePosition = Vector2f(WorldMousePosition.x, m_SUserStartMousePosition.y - m_SUserStartMouseDisplace.y);
 			OutLocalPoint = GlobalToLocalTransform(OutLocalPoint, Vector(WorldMousePosition.x, WorldMousePosition.y, 0.f));
 
 			SetPosition_(OutLocalPoint);
 			m_SUserStartMousePosition.x = InputData.MousePos.x;

			bTransformed = true;
 		}break;
 
 	case SOEvent_ControlLockY:
 		{
 			WorldMousePosition = Vector2f(m_SUserStartMousePosition.x - m_SUserStartMouseDisplace.x, WorldMousePosition.y);
 			OutLocalPoint = GlobalToLocalTransform(OutLocalPoint, Vector(WorldMousePosition.x, WorldMousePosition.y, 0.f));
 				
 			SetPosition_(OutLocalPoint);
 			m_SUserStartMousePosition.y = InputData.MousePos.y;

			bTransformed = true;
 		}break;
 	};*/
}

//----------------------------------------------------------------------------------------------
void IDrawInterfaceUI::ProcessControllerRotateLocal(const MouseMoveInputData &InputData)
{
	/*Matrix WorldMatrixTransform = GetTransformedWTM_();

 	Vector2f Mouse = InputData.MousePos - WorldMatrixTransform.t2;
 	Vector2f Prev = Vector2f(m_SUserStartMousePosition.x, m_SUserStartMousePosition.y) - WorldMatrixTransform.t2;
 
 	Prev.Normalize();
 	Mouse.Normalize();
 		
 	float fDot = Prev.Dot(Mouse);
 		
 	if (fDot < 1.f) 
 	{
		float Sign = GetHalfSpace(Mouse, Vector2f(0.f, 0.f), Prev);
		
		m_YawPitchRoll.z += ((Sign > 0.f) ? 1.f : -1.f) * ::acosf(fDot);

		Quaternion rot(0.f, 0.f, 0.f, 1.f);
		rot.set_rot(m_YawPitchRoll.x, m_YawPitchRoll.y, m_YawPitchRoll.z);

		Matrix M;
		rot.Normalize();
		rot.ToMatrix(&M);

		M.t = GetPosition_();
		SetLTM_(M);
 	}
 
 	m_SUserStartMousePosition.x = InputData.MousePos.x;
 	m_SUserStartMousePosition.y = InputData.MousePos.y;*/
}

//----------------------------------------------------------------------------------------------
void IDrawInterfaceUI::ProcessControllerScaleLocal(const MouseMoveInputData &InputData)
{
    core_sdk_api::TIViewport *pInterface = m_pCoreSDK->GetExplorerInstance()->GetExplorer2D();

	UIScreenObject *pNodeUI = (UIScreenObject*)m_pNode->m_pKey;
	Vector2f DT;

	switch (pNodeUI->GetControlMode())
	{
	case SOEvent_FreeMove:
	case SOEvent_ControlLockOrigin:
		{
			DT = InputData.DeltaPos;
		}break;

	case SOEvent_ControlLockX:
		{
			DT = Vector2f(InputData.DeltaPos.x, 0.f);
		}break;

	case SOEvent_ControlLockY:
		{
			DT = Vector2f(0, InputData.DeltaPos.y);
		}break;
	}

	// flag transformation
	switch (pNodeUI->GetSizePixelTransform_H())
	{
	case PixelOwner_Transform:
	case PixelViewport_Transform:
		// already in pixel
		break;

	case PersentageOwner_Transform:
		{
			float PWidth = GetParentWidth_();
			DT.x = (PWidth == 0.f) ? 0.f : (DT.x / PWidth);
		}break;

	case PersentageViewport_Transform:
		{
			float VWidth = (float)m_pCoreSDK->GetRenderSDK()->GetViewportWidth();
			DT.x = (VWidth == 0.f) ? 0.f : (DT.x / VWidth);
		}break;

	default:
		// error caused
		break;
	}

	switch (pNodeUI->GetSizePixelTransform_V())
	{
	case PixelOwner_Transform:
	case PixelViewport_Transform:
		// already in pixel
		break;

	case PersentageOwner_Transform:
		{
			float PHeight = GetParentHeight_();
			DT.y = (PHeight == 0.f) ? 0.f : (DT.y / PHeight);
		}break;

	case PersentageViewport_Transform:
		{
			float VHeight = (float)m_pCoreSDK->GetRenderSDK()->GetViewportHeight();
			DT.y = (VHeight == 0.f) ? 0.f : (DT.y / VHeight);
		}break;

	default:
		// error caused
		break;
	}

	m_Size += DT;
	//SetSize_(GetSize_() + DT);
}

//----------------------------------------------------------------------------------------------
bool IDrawInterfaceUI::OnMouseMove(const MouseMoveInputData &InputData)
{
	/*CActor *pActor = const_cast<CActor*>(m_pNode->m_pKey);

	if (pActor->HitTest(InputData.MousePos))
	{
		if (pActor->DoEventMove(InputData))
		{
			m_pCoreSDK->GetViewportManager()->SetMouseOver(const_cast<IDrawInterface*>(m_pNode->m_pValue));
			return true;
		}
	}*/
	return false;
}

//----------------------------------------------------------------------------------------------
bool IDrawInterfaceUI::ProcessPress(const MouseInputData &InputData)
{
	bool bResult = false;

	Bounds3f bounds = GetCompositeBounds_();

	//if (!bounds.HitTest(InputData.MousePos)){
	//	return false;
	//}

	/*CActor *pActor = const_cast<CActor*>(m_pNode->m_pKey);

	if (pActor->HitTest(InputData.MousePos)) // top most widget hit test
	{
		unsigned int State = m_pNode->m_pValue->GetVisualStates();
		
		const_cast<IDrawInterface*>(m_pNode->m_pValue)->SetVisualStates(State |= AS_PRESSED);
		
		m_pCoreSDK->GetViewportManager()->SetFocus(this); // set active state focused

		pActor->DoEventPressed(InputData); 
		
		bResult = true;
	}*/

	return bResult;
}

//----------------------------------------------------------------------------------------------
bool IDrawInterfaceUI::ProcessRelease(const MouseInputData &InputData)
{
	bool bResult = false;

	/*unsigned int State = m_pNode->m_pValue->GetVisualStates();

	if (State & AS_PRESSED)
	{
		CActor *pActor = const_cast<CActor*>(m_pNode->m_pKey);

		//pActor->KillFocus();

		m_pCoreSDK->GetViewportManager()->SetFocus(this, false);

		const_cast<IDrawInterface*>(m_pNode->m_pValue)->SetVisualStates(State &= ~AS_PRESSED);

		if (pActor->HitTest(InputData.MousePos)) // top most widget hit test
		{
			pActor->DoEventReleased(InputData);
		}
		bResult = true;
	}*/
	return bResult;
}

//----------------------------------------------------------------------------------------------
void IDrawInterfaceUI::DoBuildWorldTransform_(const Matrix &WTM)
{
	m_WorldMatrixTransform = GetPivot_() * WTM * GetSTM_();

	Vector2f Scale = GetTransformedSize_();

	Matrix TWTM = m_WorldMatrixTransform;
	scale(TWTM, Scale.x, Scale.y, 1.f);

	std::vector<oes::common_base::IRenderInterface*> &VecRenderEntities = const_cast<CActor*>(m_pNode->m_pKey)->m_VecRenderEntities;
	for (std::vector<oes::common_base::IRenderInterface*>::iterator Iter = VecRenderEntities.begin(); Iter != VecRenderEntities.end(); ++Iter)
	{
		(*Iter)->SetRWTM(TWTM);
	}
}


//----------------------------------------------------------------------------------------------
void IDrawInterfaceUI::DoBuildSizeTransform()
{
	m_SizeTransformed = m_Size;

	// transform size values
	switch (m_SizeTransform[0])
	{
	case PixelOwner_Transform:
		{
		}break;

	case PixelViewport_Transform:
		{
		}break;

	case PersentageOwner_Transform:
		{
			m_SizeTransformed.x *= GetParentWidth_();
		}break;

	case PersentageViewport_Transform:
		{
			m_SizeTransformed.x *= m_pCoreSDK->GetRenderSDK()->GetViewportWidth();
		}break;

	default:
		// caused error
		break;
	}

	// transform size values
	switch (m_SizeTransform[1])
	{
	case PixelOwner_Transform:
		{
		}break;

	case PixelViewport_Transform:
		{
		}break;

	case PersentageOwner_Transform:
		{
			m_SizeTransformed.y *= GetParentHeight_();
		}break;

	case PersentageViewport_Transform:
		{
			m_SizeTransformed.y *= m_pCoreSDK->GetRenderSDK()->GetViewportHeight();
		}break;

	default:
		// caused error
		break;
	}
}

//----------------------------------------------------------------------------------------------
float IDrawInterfaceUI::GetParentWidth_() const
{	
	return m_pNode->GetRootNode()->m_pValue->GetTransformedSize_().x;
}

//----------------------------------------------------------------------------------------------
float IDrawInterfaceUI::GetParentHeight_() const
{	
	return m_pNode->GetRootNode()->m_pValue->GetTransformedSize_().y;
}

//----------------------------------------------------------------------------------------------
Matrix IDrawInterfaceUI::GetPivot_() const
{
	// get original transform
	Matrix LTMCopy = GetLTM_();

	// transform into common pixel values
	switch(m_PixelTransform[0])
	{
	case PixelOwner_Transform:
		break;

	case PixelViewport_Transform:
		LTMCopy.t.x = LTMCopy.t.x - m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.x;
		break;

	case PersentageOwner_Transform:
		LTMCopy.t.x = GetParentWidth_() * LTMCopy.t.x;
		break;

	case PersentageViewport_Transform:
		{
			float PosX = m_pCoreSDK->GetRenderSDK()->GetViewportWidth() * LTMCopy.t.x;
			LTMCopy.t.x = PosX - m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.x;
		}break;

	default:
		break;
	};

	switch(m_PixelTransform[1])
	{
	case PixelViewport_Transform:
		LTMCopy.t.y = LTMCopy.t.y - m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.y;
		break;

	case PersentageOwner_Transform:
		LTMCopy.t.y = GetParentHeight_() * LTMCopy.t.y;
		break;

	case PersentageViewport_Transform:
		{
			float PosY = m_pCoreSDK->GetRenderSDK()->GetViewportHeight() * LTMCopy.t.y;
			LTMCopy.t.y = PosY - m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.y;
		}break;

	default:
		break;
	};

	return LTMCopy;
}

//----------------------------------------------------------------------------------
void IDrawInterfaceUI::SetPivot_(const Matrix& matrix) const 
{ 
	m_LTM_ = matrix; 
}

//----------------------------------------------------------------------------------
void IDrawInterfaceUI::ConvertPixelTransform_H(EPixelTransform ValueFrom, EPixelTransform ValueTo)
{
	// get original transform
	Matrix LTMCopy = GetLTM_();

	// transform size values
	switch (ValueFrom)
	{
	case PixelOwner_Transform:
		{
			// transform 2
			if (ValueTo == PixelViewport_Transform)
			{
				LTMCopy.t.x = GetTransformedWTM_().t.x;
			}
			else if (ValueTo == PersentageOwner_Transform)
			{
				LTMCopy.t.x = (GetParentWidth_() != 0.f) ? LTMCopy.t.x / GetParentWidth_() : 0.f;
			}
			else if (ValueTo == PersentageViewport_Transform)
			{
				float VWidth = (float)m_pCoreSDK->GetRenderSDK()->GetViewportWidth();
				LTMCopy.t.x = (VWidth != 0.f) ? GetTransformedWTM_().t.x / VWidth : 0.f;
			}
		}break;

	case PixelViewport_Transform:
		{
			// transform 2
			if (ValueTo == PixelOwner_Transform)
			{
				LTMCopy.t.x = GetTransformedWTM_().t.x - m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.x;
			}
			else if (ValueTo == PersentageOwner_Transform)
			{
				float PixelShift = GetTransformedWTM_().t.x - m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.x;
				LTMCopy.t.x = (GetParentWidth_() != 0.f) ? PixelShift / GetParentWidth_() : 0.f;
			}
			else if (ValueTo == PersentageViewport_Transform)
			{
				float VWidth = (float)m_pCoreSDK->GetRenderSDK()->GetViewportWidth();
				LTMCopy.t.x = (VWidth != 0.f) ? GetTransformedWTM_().t.x / VWidth : 0.f;
			}
		}break;

	case PersentageOwner_Transform:
		{
			// transform 2
			if (ValueTo == PixelOwner_Transform)
			{
				LTMCopy.t.x = GetParentWidth_() * LTMCopy.t.x;
			}
			else if (ValueTo == PixelViewport_Transform)
			{
				LTMCopy.t.x = GetTransformedWTM_().t.x;
			}
			else if (ValueTo == PersentageViewport_Transform)
			{
				float VWidth = (float)m_pCoreSDK->GetRenderSDK()->GetViewportWidth();
				LTMCopy.t.x = (VWidth != 0.f) ? GetTransformedWTM_().t.x / VWidth : 0.f;
			}
		}break;

	case PersentageViewport_Transform:
		{
			if (ValueTo == PixelOwner_Transform)
			{
				LTMCopy.t.x = GetTransformedWTM_().t.x - m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.x;
			}
			else if (ValueTo == PixelViewport_Transform)
			{
				LTMCopy.t.x = GetTransformedWTM_().t.x;
			}
			else if (ValueTo == PersentageOwner_Transform)
			{
				float PWidth = GetParentWidth_();
				LTMCopy.t.x = (PWidth != 0.f) ? (GetTransformedWTM_().t.x - m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.x) / PWidth : 0.f;
			}
		}break;

	default:
		{
		}
	}

	SetLTM_(LTMCopy);
}

//----------------------------------------------------------------------------------
void IDrawInterfaceUI::ConvertPixelTransform_V(EPixelTransform ValueFrom, EPixelTransform ValueTo)
{
	// get original transform
	Matrix LTMCopy = GetLTM_();

	// transform size values
	switch (ValueFrom)
	{
	case PixelOwner_Transform:
		{
			// transform 2
			if (ValueTo == PixelViewport_Transform)
			{
				LTMCopy.t.y = GetTransformedWTM_().t.y;
			}
			else if (ValueTo == PersentageOwner_Transform)
			{
				LTMCopy.t.y = (GetParentHeight_() != 0.f) ? LTMCopy.t.y / GetParentHeight_() : 0.f;
			}
			else if (ValueTo == PersentageViewport_Transform)
			{
				float VHeight = (float)m_pCoreSDK->GetRenderSDK()->GetViewportHeight();
				LTMCopy.t.y = (VHeight != 0.f) ? GetTransformedWTM_().t.y / VHeight : 0.f;
			}
		}break;

	case PixelViewport_Transform:
		{
			// transform 2
			if (ValueTo == PixelOwner_Transform)
			{
				LTMCopy.t.y = GetTransformedWTM_().t.y -  m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.y;
			}
			else if (ValueTo == PersentageOwner_Transform)
			{
				float PixelShift = GetTransformedWTM_().t.y -  m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.y;
				LTMCopy.t.y = (GetParentHeight_() != 0.f) ? PixelShift / GetParentHeight_() : 0.f;
			}
			else if (ValueTo == PersentageViewport_Transform)
			{
				float VHeight = (float)m_pCoreSDK->GetRenderSDK()->GetViewportHeight();
				LTMCopy.t.y = (VHeight != 0.f) ? GetTransformedWTM_().t.y / VHeight : 0.f;
			}
		}break;

	case PersentageOwner_Transform:
		{
			// transform 2
			if (ValueTo == PixelOwner_Transform)
			{
				LTMCopy.t.y = GetParentHeight_() * LTMCopy.t.y;
			}
			else if (ValueTo == PixelViewport_Transform)
			{
				LTMCopy.t.y = GetTransformedWTM_().t.y;
			}
			else if (ValueTo == PersentageViewport_Transform)
			{
				float VHeight = (float)m_pCoreSDK->GetRenderSDK()->GetViewportHeight();
				LTMCopy.t.y = (VHeight != 0.f) ? GetTransformedWTM_().t.y / VHeight : 0.f;
			}
		}break;

	case PersentageViewport_Transform:
		{
			if (ValueTo == PixelOwner_Transform)
			{
				LTMCopy.t.y = GetTransformedWTM_().t.y -  m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.y;
			}
			else if (ValueTo == PixelViewport_Transform)
			{
				LTMCopy.t.y = GetTransformedWTM_().t.y;
			}
			else if (ValueTo == PersentageOwner_Transform)
			{
				float PHeight = GetParentHeight_();
				LTMCopy.t.y = (PHeight != 0.f) ? (GetTransformedWTM_().t.x - m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.x) / PHeight : 0.f;
			}
		}break;

	default:
		{
		}
	}

	SetLTM_(LTMCopy);
}

//----------------------------------------------------------------------------------
void IDrawInterfaceUI::ConvertSizeTransform_H(EPixelTransform ValueFrom, EPixelTransform ValueTo)
{
	// transform size values
	switch (ValueFrom)
	{
	case PixelOwner_Transform:
		{
			// transform 2
			if (ValueTo == PersentageOwner_Transform)
			{
				m_Size.x = (GetParentWidth_() != 0.f) ? (m_Size.x / GetParentWidth_()) : 0.f;
			}
			else if (ValueTo == PersentageViewport_Transform)
			{
				float VWidth = (float)m_pCoreSDK->GetRenderSDK()->GetViewportWidth();
				m_Size.x = (VWidth != 0.f) ? (m_Size.x / VWidth) : 0.f;
			}
		}break;

	case PixelViewport_Transform:
		{
			// transform 2
			if (ValueTo == PersentageOwner_Transform)
			{
				m_Size.x = (GetParentWidth_() != 0.f) ? (m_Size.x / GetParentWidth_()) : 0.f;
			}
			else if (ValueTo == PersentageViewport_Transform)
			{
				float VWidth = (float)m_pCoreSDK->GetRenderSDK()->GetViewportWidth();
				m_Size.x = (VWidth != 0.f) ? (m_Size.x / VWidth) : 0.f;
			}
		}break;

	case PersentageOwner_Transform:
		{
			// transform 2
			if (ValueTo == PixelOwner_Transform)
			{
				m_Size.x = GetParentWidth_() * m_Size.x;
			}
			else if (ValueTo == PixelViewport_Transform)
			{
				m_Size.x = m_pCoreSDK->GetRenderSDK()->GetViewportWidth() * m_Size.x;
			}
			else if (ValueTo == PersentageViewport_Transform)
			{
				float PixelAmount = GetParentWidth_() * m_Size.x;
				float VWidth = (float)m_pCoreSDK->GetRenderSDK()->GetViewportWidth();
				m_Size.x = (VWidth != 0.f) ? (PixelAmount / VWidth) : 0.f;
			}
		}break;

	case PersentageViewport_Transform:
		{
			if (ValueTo == PixelOwner_Transform)
			{
				float PixelAmount = m_pCoreSDK->GetRenderSDK()->GetViewportWidth() * m_Size.x;
				m_Size.x = PixelAmount;
			}
			else if (ValueTo == PixelViewport_Transform)
			{
				float PixelAmount = m_pCoreSDK->GetRenderSDK()->GetViewportWidth() * m_Size.x;
				m_Size.x = PixelAmount;
			}
			else if (ValueTo == PersentageOwner_Transform)
			{
				float PixelAmount = m_pCoreSDK->GetRenderSDK()->GetViewportWidth() * m_Size.x;
				m_Size.x = (GetParentWidth_() != 0.f) ? (PixelAmount / GetParentWidth_()) : 0.f;
			}
		}break;

	default:
		{
		}
	}
}

//----------------------------------------------------------------------------------
void IDrawInterfaceUI::ConvertSizeTransform_V(EPixelTransform ValueFrom, EPixelTransform ValueTo)
{
	// transform size values
	switch (ValueFrom)
	{
	case PixelOwner_Transform:
		{
			// transform 2
			if (ValueTo == PersentageOwner_Transform)
			{
				m_Size.y = (GetParentHeight_() != 0.f) ? (m_Size.y / GetParentHeight_()) : 0.f;
			}
			else if (ValueTo == PersentageViewport_Transform)
			{
				float VHeight = (float)m_pCoreSDK->GetRenderSDK()->GetViewportHeight();
				m_Size.y = (VHeight != 0.f) ? (m_Size.y / VHeight) : 0.f;
			}
		}break;

	case PixelViewport_Transform:
		{
			// transform 2
			if (ValueTo == PersentageOwner_Transform)
			{
				m_Size.y = (GetParentHeight_() != 0.f) ? (m_Size.y / GetParentHeight_()) : 0.f;
			}
			else if (ValueTo == PersentageViewport_Transform)
			{
				float VHeight = (float)m_pCoreSDK->GetRenderSDK()->GetViewportHeight();
				m_Size.y = (VHeight != 0.f) ? (m_Size.y / VHeight) : 0.f;
			}
		}break;

	case PersentageOwner_Transform:
		{
			// transform 2
			if (ValueTo == PixelOwner_Transform)
			{
				m_Size.y = GetParentHeight_() * m_Size.y;
			}
			else if (ValueTo == PixelViewport_Transform)
			{
				m_Size.y = m_pCoreSDK->GetRenderSDK()->GetViewportHeight() * m_Size.y;
			}
			else if (ValueTo == PersentageViewport_Transform)
			{
				float PixelAmount = GetParentHeight_() * m_Size.y;
				float VHeight = (float)m_pCoreSDK->GetRenderSDK()->GetViewportHeight();
				m_Size.y = (VHeight != 0.f) ? (PixelAmount / VHeight) : 0.f;
			}
		}break;

	case PersentageViewport_Transform:
		{
			if (ValueTo == PixelOwner_Transform)
			{
				float PixelAmount = m_pCoreSDK->GetRenderSDK()->GetViewportHeight() * m_Size.y;
				m_Size.y = PixelAmount;
			}
			else if (ValueTo == PixelViewport_Transform)
			{
				float PixelAmount = m_pCoreSDK->GetRenderSDK()->GetViewportHeight() * m_Size.y;
				m_Size.y = PixelAmount;
			}
			else if (ValueTo == PersentageOwner_Transform)
			{
				float PixelAmount = m_pCoreSDK->GetRenderSDK()->GetViewportHeight() * m_Size.y;
				m_Size.y = (GetParentHeight_() != 0.f) ? (PixelAmount / GetParentHeight_()) : 0.f;
			}
		}break;

	default:
		{
		}
	}
}


//----------------------------------------------------------------------------------------------
Vector&	IDrawInterfaceUI::GlobalToLocalTransform(Vector& OutLocalPoint, const Vector& InGlobalPoint) const
{
	IDrawInterface::GlobalToLocalTransform(OutLocalPoint, InGlobalPoint);

	switch (m_PixelTransform[0])
	{
	case PixelOwner_Transform:
		// already in pixel
		break;

	case PixelViewport_Transform:
		OutLocalPoint.x = InGlobalPoint.x - m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.x;
		break;

	case PersentageOwner_Transform:
		OutLocalPoint.x = (GetParentWidth_() == 0.f) ? 0.f : (OutLocalPoint.x / GetParentWidth_());
		break;

	case PersentageViewport_Transform:
		{
			float VWidth = (float)m_pCoreSDK->GetRenderSDK()->GetViewportWidth();
			OutLocalPoint.x = (VWidth == 0.f) ? 0.f : (InGlobalPoint.x / VWidth);
		}break;

	default:
		// error caused
		break;
	}

	switch (m_PixelTransform[1])
	{
	case PixelOwner_Transform:
		// already in pixel
		break;

	case PixelViewport_Transform:
		OutLocalPoint.y = InGlobalPoint.y - m_pNode->GetRootNode()->m_pValue->GetTransformedWTM_().t.y;
		break;

	case PersentageOwner_Transform:
		OutLocalPoint.y = (GetParentHeight_() == 0.f) ? 0.f : (OutLocalPoint.y / GetParentHeight_());
		break;

	case PersentageViewport_Transform:
		{
			float VHeight = (float)m_pCoreSDK->GetRenderSDK()->GetViewportHeight();
			OutLocalPoint.y = (VHeight == 0.f) ? 0.f : (InGlobalPoint.y / VHeight);
		}break;

	default:
		// error caused
		break;
	}

	return OutLocalPoint;
}
