#pragma once

#include "scene_editor_main.h"

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class CViewContainer : public CWindowImpl<CPaneContainer>
{
public:
	BEGIN_MSG_MAP(CViewContainer)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnActivate)
		MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLBDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLBUp)
		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRBDown)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnRBUp)
		MESSAGE_HANDLER(WM_MBUTTONDOWN, OnMBDown)
		MESSAGE_HANDLER(WM_MBUTTONUP, OnMBUp)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLBDClick)
		MESSAGE_HANDLER(WM_RBUTTONDBLCLK, OnRBDClick)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeydown)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
	END_MSG_MAP()

public:
    class CCoreSDK *m_pApp;

    unsigned int m_MousePosPrevX;
    unsigned int m_MousePosPrevY;

    EObjEditControlMode m_SaveState;
    bool m_bShiftPressed;
    bool m_bCtrlPressed;
    bool m_bPush;

    class SRenderContext *m_pRenderContext;

private:
    std::shared_ptr<editors::IEditor> m_editor;

public:
	//----------------------------------------------------------------------------------------------
	CViewContainer(editors::TIEditor &editor)
		: m_MousePosPrevX(0)
		, m_MousePosPrevY(0)
		, m_pApp(0)
		, m_bPush(false)
		, m_pRenderContext(0)
	{
        m_editor = editor;
        m_pApp = editor->GetApp();
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnActivate(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		SetFocus();
		return 0;
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

		m_editor->InputMouse(MOUSE_Pressed, MOUSE_LEFT, xPos, yPos, ModifKey);

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

		unsigned int vprtWidth = m_pApp->GetRenderSDK()->GetViewportWidth();
		unsigned int vprtHeight = m_pApp->GetRenderSDK()->GetViewportHeight();

		m_editor->InputMouse(MOUSE_Released, MOUSE_LEFT, (int)(xPosRel * vprtWidth), (int)(yPosRel * vprtHeight), 0);
		
		ReleaseCapture();

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnRBDown(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		int ModifKey = 0;
		if (wParam & MK_CONTROL){
			ModifKey |= MK_Control;
		}
		if (wParam & MK_SHIFT){
			ModifKey |= MK_Shift;
		}

        m_editor->InputMouse(MOUSE_Pressed, MOUSE_RIGHT, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), ModifKey);
		
		SetCapture();

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnRBUp(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		int ModifKey = 0;
		if (wParam & MK_CONTROL){
			ModifKey |= MK_Control;
		}
		if (wParam & MK_SHIFT){
			ModifKey |= MK_Shift;
		}

		m_editor->InputMouse(MOUSE_Released, MOUSE_RIGHT, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), ModifKey);
		
        ReleaseCapture();

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnMBDown(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
        m_editor->InputMouse(MOUSE_Pressed, MOUSE_MIDDLE, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0);
		
		SetCapture();

		SetFocus();

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnMBUp(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
        m_editor->InputMouse(MOUSE_Released, MOUSE_MIDDLE, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0);
		
		ReleaseCapture();
		
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnLBDClick(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		int ModifKey = 0;
		if (wParam & MK_CONTROL){
			ModifKey |= MK_Control;
		}
		if (wParam & MK_SHIFT){
			ModifKey |= MK_Shift;
		}
        m_editor->InputMouse(MOUSE_DoubleClick, MOUSE_LEFT, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), ModifKey);

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnRBDClick(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		int ModifKey = 0;
		if (wParam & MK_CONTROL){
			ModifKey |= MK_Control;
		}
		if (wParam & MK_SHIFT){
			ModifKey |= MK_Shift;
		}
        m_editor->InputMouse(MOUSE_DoubleClick, MOUSE_RIGHT, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), ModifKey);

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

        RECT rectWindow;
        GetClientRect(&rectWindow);

        const size_t wndWidth = rectWindow.right - rectWindow.left;
        const size_t wndHeight = rectWindow.bottom - rectWindow.top;

        m_editor->MouseMove(PosX, PosY, wndWidth, wndHeight, ModifKey);

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnMouseWheel(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		m_editor->MouseWheel((float)GET_WHEEL_DELTA_WPARAM(wParam), 0, 0);

		bHandled = TRUE;
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnSize(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
        if (m_editor)
        {
            m_editor->ResizeVeiwport((int)GET_X_LPARAM(lParam), (int)GET_Y_LPARAM(lParam));
        }
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnKeydown(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		switch ((DWORD)wParam) 
		{
		case VK_ESCAPE:	m_editor->InputKey( IE(KEY_ESCAPE, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));	break;
		case VK_RETURN: m_editor->InputKey( IE(KEY_ENTER, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed)); break;
		case VK_F1:		m_editor->InputKey( IE(KEY_F1, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));	break;
		case VK_F2:		m_editor->InputKey( IE(KEY_F2, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));	break;
		case VK_F3:		m_editor->InputKey( IE(KEY_F3, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));	break;
		case VK_F4:		m_editor->InputKey( IE(KEY_F4, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));	break;
		case VK_F5:		m_editor->InputKey( IE(KEY_F5, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));	break;
		case VK_F6:		m_editor->InputKey( IE(KEY_F6, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));	break;
		case VK_F7:		m_editor->InputKey( IE(KEY_F7, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));	break;
		case VK_F8:		m_editor->InputKey( IE(KEY_F8, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));	break;
		case VK_F9:		m_editor->InputKey( IE(KEY_F9, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));	break;
		case VK_F10:	m_editor->InputKey( IE(KEY_F10, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed)); break;
		case VK_F11:	m_editor->InputKey( IE(KEY_F11, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed)); break;
		case VK_F12:	m_editor->InputKey( IE(KEY_F12, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed)); break;
		case VK_DELETE:	m_editor->InputKey( IE(KEY_DELETE, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed)); break;
		case VK_UP:		m_editor->InputKey( IE(KEY_UP, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed)); break;
		case VK_DOWN:	m_editor->InputKey( IE(KEY_DOWN, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed)); break;
		case VK_PRIOR:
			{
				//CActor * SelectedActor = m_pAppMain->GetExplorerInstance()->GetFocused();
				//if (SelectedActor)
				//{
				//	if (m_bCtrlPressed){
				//		CActor::MoveObjectHierarchyBound(SelectedActor, true);
				//	}else{
				//		ActorAllocator::MoveObjectHierarchy(SelectedActor, m_pAppMain->GetRegistry(), true, true);
				//	}
				//}
				//m_pAppMain->ProcessInputKey( IE(KEY_PAGEUP, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));
			}break;
		case VK_NEXT:
			{
				//CActor * SelectedActor = m_pAppMain->GetExplorerInstance()->GetFocused();
				//if (SelectedActor)
				//{
				//	if (CtrlPressed){
				//		CActor::MoveObjectHierarchyBound(SelectedActor, false);
				//	}else{
				//		ActorAllocator::MoveObjectHierarchy(SelectedActor, m_pAppMain->GetRegistry(), false, true);
				//	}
				//}
				//m_pAppMain->ProcessInputKey( IE(KEY_PAGEDOWN, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));
			}break;

		case VK_LEFT:   m_editor->InputKey( IE(KEY_LEFT, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));  break;
		case VK_RIGHT:  m_editor->InputKey( IE(KEY_RIGHT, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed)); break;
		case VK_SPACE:  m_editor->InputKey( IE(KEY_SPACE, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed)); break;

		case VK_SHIFT:	
			{
				m_bShiftPressed = true;
				m_editor->InputKey( IE(KEY_LSHIFT, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));
				PushCtrlState(ECM_Scale);
			}break;

		case VK_CONTROL:
			{
				m_bCtrlPressed = true;
				m_pApp->ProcessInputKey( IE(KEY_LCONTROL, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));
				PushCtrlState(ECM_Rotate);
			}break;
		};
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnChar(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		if (wParam == 3){
			m_pApp->ProcessEventCopy();
		}

		if (wParam == 22){
			m_pApp->ProcessEventPaste();
		}

		switch (wParam)
		{
		case 'Y':
		case 'y':
			m_editor->InputKey(IE(KEY_Y, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed)); 
			break;

		case 'U':
		case 'u':
			m_editor->InputKey(IE(KEY_U, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed)); 
			break;

		case 'G':
		case 'g':
			m_editor->InputKey(IE(KEY_G, KEY_Pressed, m_bShiftPressed, m_bCtrlPressed));
			break;

		default:
			break;
		}
		return 0;
	}

private:
	//----------------------------------------------------------------------------------------------
	void PushCtrlState(EObjEditControlMode State)
	{
		EObjEditControlMode OldState = m_pApp->GetEditControlMode();

		if (OldState != State && !m_bPush)
		{
			m_SaveState = OldState;

			m_pApp->SetEditControlMode(State);
			
			//pWndMgr->UpdateFlagsState();

			//if (CActor* pSelected = static_cast<CWindowChildBottomBottom*>(pWndMgr->m_pCtrlWindowBootomBottom)->GetWidgetPlacement()->GetSelected()){
			//	pSelected->BroadcastEvent(Event_ChangeEditCtrMode);
			//}
			m_bPush = true;
		}
	}

	//----------------------------------------------------------------------------------------------
	void PopCtrlState()
	{
		if (m_bPush)
		{
			m_pApp->SetEditControlMode(m_SaveState);
			
			//UpdateFlagsState();

			//if (CActor* pSelected = static_cast<CWindowChildBottomBottom*>(pWndMgr->m_pCtrlWindowBootomBottom)->GetWidgetPlacement()->GetSelected()){
			//	pSelected->BroadcastEvent(Event_ChangeEditCtrMode);
			//}
			m_bPush = false;
		}
	}
};