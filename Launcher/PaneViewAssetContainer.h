#ifndef __paneviewassetcontainer_h__
#define __paneviewassetcontainer_h__

#pragma once

#include "editors/scene_editor.h"

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
		, m_bPush(false)
        , m_pAppMain(pAppMain)
        , m_editor(nullptr)
	{
		
	}

	//----------------------------------------------------------------------------------------------
	~CViewAssetContainer()
	{

	}

    //----------------------------------------------------------------------------------------------
    void SetEditor(editors::TIEditor editor) { m_editor = editor; }

	//----------------------------------------------------------------------------------------------
	LRESULT OnActivate(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		SetFocus();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnSize(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		int width = (int)GET_X_LPARAM(lParam);
		int height = (int)GET_Y_LPARAM(lParam);

        if (m_editor)
        {
            m_editor->ResizeVeiwport((size_t)width, (size_t)height);
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

        m_editor->InitViewport(static_cast<void*>(m_hWnd), rc.right - rc.left, rc.bottom - rc.top);
	}

	//----------------------------------------------------------------------------------------------
	void Render()
	{
        m_editor->Render();
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

        SRenderContext *context = m_editor->GetRenderContext();

		m_pAppMain->ProcessInputMouse(MOUSE_Pressed, MOUSE_LEFT, xPos, yPos, ModifKey, context);

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

        SRenderContext *context = m_editor->GetRenderContext();

		unsigned int vprtWidth = context->m_displayModeWidth;
		unsigned int vprtHeight = context->m_displayModeHeight;

		m_pAppMain->ProcessInputMouse(MOUSE_Released, MOUSE_LEFT, (int)(xPosRel * vprtWidth), (int)(yPosRel * vprtHeight), 0, context);

		ReleaseCapture();

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnMBDown(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
        SRenderContext *context = m_editor->GetRenderContext();

		m_pAppMain->ProcessInputMouse(MOUSE_Pressed, MOUSE_MIDDLE, 
            GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, context);

		SetCapture();

		SetFocus();

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnMBUp(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
        SRenderContext *context = m_editor->GetRenderContext();

		m_pAppMain->ProcessInputMouse(MOUSE_Released, MOUSE_MIDDLE,
										GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, context);

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

            SRenderContext *context = m_editor->GetRenderContext();

			unsigned int vprtWidth = context->m_displayModeWidth;
			unsigned int vprtHeight = context->m_displayModeHeight;

			float xPosRel = (wndWidth > 0.f) ? PosX / (float)wndWidth : 0.f;
			float yPosRel = (wndHeight > 0.f) ? PosY / (float)wndHeight : 0.f;

			float xDRel = (wndWidth > 0.f) ? ((DX / (float)wndWidth) * vprtWidth) : 0.f;
			float yDRel = (wndHeight > 0.f) ? ((DY / (float)wndHeight) * vprtHeight) : 0.f;

			m_pAppMain->ProcessMouseMove(xPosRel * vprtWidth, yPosRel * vprtHeight, xDRel, yDRel, ModifKey, context);

			m_MousePosPrevX = PosX;
			m_MousePosPrevY = PosY;
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnMouseWheel(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

        SRenderContext *context = m_editor->GetRenderContext();

		m_pAppMain->ProcessMouseWheel((float)zDelta, 0, 0, context);

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

    editors::TIEditor    m_editor;
};

#endif//__paneviewassetcontainer_h__