#ifndef __paneviewassetcontainer_h__
#define __paneviewassetcontainer_h__

#pragma once

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class CViewAssetContainer : public CWindowImpl<CPaneContainer>
{
public:
	BEGIN_MSG_MAP(CViewAssetContainer)
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLBDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLBUp)
		MESSAGE_HANDLER(WM_MBUTTONDOWN, OnMBDown)
		MESSAGE_HANDLER(WM_MBUTTONUP, OnMBUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
	END_MSG_MAP()

	//----------------------------------------------------------------------------------------------
	CViewAssetContainer(CCoreSDK *pAppMain)
		: m_MousePosPrevX(0)
		, m_MousePosPrevY(0)
		, m_pAppMain(pAppMain)
		, m_bPush(false)
		, m_pRenderContextRef(0)
	{
		
	}

	//----------------------------------------------------------------------------------------------
	~CViewAssetContainer()
	{

	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnActivate(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		SetFocus();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnSize(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		int Width = (int)GET_X_LPARAM(lParam);
		int Height = (int)GET_Y_LPARAM(lParam);

		if (m_pAppMain)
		{
			CModelViewer *pModelView = m_pAppMain->GetExplorerInstance()->GetModelViewer();
			assert(pModelView);

			CCamera *pCamera = pModelView->GetCamera();
			assert(pCamera);

			m_pAppMain->GetRenderSDK()->ResizeWindow(Width, Height, pCamera->GetRenderContext());
			m_pAppMain->GetCameraManager()->ViewportResized();
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnChar(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		if (wParam == 3){
			//m_pApp->ProcessEventCopy();
		}

		if (wParam == 22){
			//m_pApp->ProcessEventPaste();
		}

		switch (wParam)
		{
		case 'Y':
		case 'y':
			//m_pApp->ProcessInputKey(IE(KEY_Y, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed)); 
			break;

		case 'U':
		case 'u':
			//m_pApp->ProcessInputKey(IE(KEY_U, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed)); 
			break;

		case 'G':
		case 'g':
			//m_pApp->ProcessInputKey(IE(KEY_G, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));
			break;

		default:
			break;
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	void InitViewport()
	{
		CRect rc;
		GetClientRect(&rc);

		CRenderSDK *pRenderSDK = m_pAppMain->GetRenderSDK();
		assert(pRenderSDK);

		D3DDriver *pDriver = pRenderSDK->GetRenderDriver();
		assert(pDriver);

		CModelViewer *pModelView = m_pAppMain->GetExplorerInstance()->GetModelViewer();
		assert(pModelView);

		m_pRenderContextRef = pModelView->GetRenderContext();
		assert(m_pRenderContextRef);

		pDriver->InitRenderDriver(m_hWnd, rc.right - rc.left, rc.bottom - rc.top, m_pRenderContextRef);
	}

	//----------------------------------------------------------------------------------------------
	void Render()
	{
		m_pAppMain->Render(m_pRenderContextRef); // Render & change buffer
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnLBDown(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		int ModifKey = 0;
		if (wParam & MK_CONTROL){
			ModifKey |= MK_Control;
		}
		if (wParam & MK_SHIFT){
			ModifKey |= MK_Shift;
		}

		int xPos = (int)GET_X_LPARAM(lParam);
		int yPos = (int)GET_Y_LPARAM(lParam);

		m_pAppMain->ProcessInputMouse(MOUSE_Pressed, MOUSE_LEFT, xPos, yPos, ModifKey, m_pRenderContextRef);

		SetCapture();

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnLBUp(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		int xPos = (int)GET_X_LPARAM(lParam);
		int yPos = (int)GET_Y_LPARAM(lParam);

		RECT rectWindows;
		GetClientRect(&rectWindows);

		unsigned int wndWidth = rectWindows.right - rectWindows.left;
		unsigned int wndHeight = rectWindows.bottom - rectWindows.top;

		float xPosRel = (wndWidth > 0.f) ? xPos / (float)wndWidth : 0.f;
		float yPosRel = (wndHeight > 0.f) ? yPos / (float)wndHeight : 0.f;

		unsigned int vprtWidth = m_pRenderContextRef->m_displayModeWidth;
		unsigned int vprtHeight = m_pRenderContextRef->m_displayModeHeight;

		m_pAppMain->ProcessInputMouse(MOUSE_Released, MOUSE_LEFT, (int)(xPosRel * vprtWidth), (int)(yPosRel * vprtHeight), 0, m_pRenderContextRef);

		ReleaseCapture();

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnMBDown(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		m_pAppMain->ProcessInputMouse(MOUSE_Pressed, MOUSE_MIDDLE, 
										GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, m_pRenderContextRef);

		SetCapture();

		SetFocus();

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnMBUp(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		m_pAppMain->ProcessInputMouse(MOUSE_Released, MOUSE_MIDDLE,
										GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, m_pRenderContextRef);

		ReleaseCapture();

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnMouseMove(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		int PosX = (int)GET_X_LPARAM(lParam);
		int PosY = (int)GET_Y_LPARAM(lParam);

		int ModifKey = 0;
		if (wParam & MK_CONTROL){
			ModifKey |= MK_Control;
		}
		if (wParam & MK_SHIFT){
			ModifKey |= MK_Shift;
		}
		if (::GetAsyncKeyState(VK_MBUTTON)){
			ModifKey |= MK_MMButton;
		}

		int DX = PosX - m_MousePosPrevX;
		int DY = PosY - m_MousePosPrevY;

		if (DX != 0 || DY != 0)
		{	
			RECT rectWindow;
			GetClientRect(&rectWindow);

			unsigned int wndWidth = rectWindow.right - rectWindow.left;
			unsigned int wndHeight = rectWindow.bottom - rectWindow.top;

			unsigned int vprtWidth = m_pRenderContextRef->m_displayModeWidth;
			unsigned int vprtHeight = m_pRenderContextRef->m_displayModeHeight;

			float xPosRel = (wndWidth > 0.f) ? PosX / (float)wndWidth : 0.f;
			float yPosRel = (wndHeight > 0.f) ? PosY / (float)wndHeight : 0.f;

			float xDRel = (wndWidth > 0.f) ? ((DX / (float)wndWidth) * vprtWidth) : 0.f;
			float yDRel = (wndHeight > 0.f) ? ((DY / (float)wndHeight) * vprtHeight) : 0.f;

			MouseMoveInputData InputData;

			InputData.MousePos = Vector2f((float)xPosRel * vprtWidth, (float)yPosRel * vprtHeight);
			InputData.DeltaPos = Vector2f((float)xDRel, (float)yDRel);
			InputData.ModifKey = ModifKey;
			InputData.bMiddleButtonPressed = ModifKey & MK_MMButton ? true : false;
			InputData.pRenderContext = m_pRenderContextRef;

			m_pAppMain->ProcessMouseMove(xPosRel * vprtWidth, yPosRel * vprtHeight, xDRel, yDRel, ModifKey, m_pRenderContextRef);

			m_MousePosPrevX = PosX;
			m_MousePosPrevY = PosY;
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnMouseWheel(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		m_pAppMain->ProcessMouseWheel((float)zDelta, 0, 0, m_pRenderContextRef);

		bHandled = TRUE;
		return 0;
	}

public:
	class CCoreSDK *m_pAppMain;

	unsigned int m_MousePosPrevX;
	unsigned int m_MousePosPrevY;

	EObjEditControlMode m_SaveState;
	bool m_bShiftPressed;
	bool m_bCtrlPressed;
	bool m_bPush;

	SRenderContext	*m_pRenderContextRef;
};

#endif//__paneviewassetcontainer_h__