#include "gamestdafx.h"

#define ID_BUTTON_NEWPROJECT 1045
#define ID_BUTTON_ADDNEWITEM 1046
#define ID_BUTTON_OPENFILE	 1047	 
#define ID_BUTTON_SAVEFILE   1048

#pragma warning(disable: 4312) // disable type conversion warning
#pragma warning(disable: 4311)

const int MinWndSize = 40;

namespace nmWindowManager
{
	std::vector<CActor*> SelectedActorList;
	std::string			 SplashScreenMessage;
}
using namespace nmWindowManager;

RECT rWindowMain,
	rWindowTop,
	rWindowLeft,
	rWindowRight,
	rWindowBottom,
	rWindowCenter;

struct SStateInfo
{
   CWindowManager* pMgrWnd;
};

//----------------------------------------------------------------------------------------------
namespace WindowManagerEx
{
	LRESULT CALLBACK WndProcMain(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		CWindowManager * WndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));

		if (WndMgr)
		{
			return WndMgr->WndProcMain(hWnd, messg, wParam, lParam);
		}
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}

//----------------------------------------------------------------------------------------------
	LRESULT CALLBACK SplashWndProcMain(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		CWindowManager * WndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));

		if (WndMgr)
		{
			switch (messg) 
			{
			case WM_PAINT:
				{	
					HDC         hdc;
					HFONT hCustomFont;
					HFONT hCustomFont2;

					hdc = GetDC(hWnd);

					//hdc = BeginPaint(hWnd, &PaintStruct);

					hCustomFont = CreateFont(
						16,7,
						0,0,
						500,0,
						0,0,
						ANSI_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH,
						L"Verdana");

					hCustomFont2 = CreateFont(
						12,5,
						0,0,
						500,0,
						0,0,
						ANSI_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH,
						L"Verdana");

  					if (hdc)
  					{
  						HDC     hMemDC;
  						HBITMAP hOldBitmap;
  
  						hMemDC = CreateCompatibleDC(hdc);

  						hOldBitmap = (HBITMAP)::SelectObject(hMemDC, WndMgr->GetSplashBitmap());
   						BitBlt(hdc, 0, 0, WndMgr->GetSplashWidth(), WndMgr->GetSplashHeight(), hMemDC, 0, 0, SRCCOPY);

						hOldBitmap = (HBITMAP)::SelectObject(hMemDC, WndMgr->GetSplashLogo());
						BitBlt(hdc, WndMgr->GetSplashWidth() - WndMgr->GetLogoWidth() - 15, WndMgr->GetSplashHeight() - WndMgr->GetLogoHeight() * 2, WndMgr->GetLogoWidth(), WndMgr->GetLogoHeight(), hMemDC, 0, 0, SRCCOPY);

   						SelectObject(hMemDC, hOldBitmap);
  						DeleteDC(hMemDC);
  					}
					
					SelectObject(hdc, hCustomFont);
					SetTextColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, TRANSPARENT);
						
					std::wstring str = ConvertStringToWideString(STR_COPYRIGHT);
					::TextOut(hdc, (int)(WndMgr->GetSplashWidth() - 160), (int)(WndMgr->GetSplashHeight() - 20), str.c_str(), (int)wcslen(str.c_str()));

					str = ConvertStringToWideString("www.innovatelogic.com");
					::TextOut(hdc, (int)(WndMgr->GetSplashWidth() - 234), (int)(WndMgr->GetSplashHeight() - 45), str.c_str(), (int)wcslen(str.c_str()));

					SelectObject(hdc, hCustomFont2);
					std::wstring strlog = ConvertStringToWideString(SplashScreenMessage);
					::TextOut(hdc, 10, (int)(WndMgr->GetSplashHeight() - 35), strlog.c_str(), (int)wcslen(strlog.c_str()));

					DeleteObject(hCustomFont);
					DeleteObject(hCustomFont2);

					ReleaseDC(hWnd, hdc);

					//EndPaint(hWnd, &PaintStruct);
				}break;
			}
		}
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}
}

//----------------------------------------------------------------------------------------------
CWindowManager::CWindowManager(CCoreSDK * appMain)
: m_pAppMain(appMain)
, ShiftPressed(false)
, CtrlPressed(false)
, m_displayModeWidth(1024)
, m_displayModeHeight(768)
, m_SplHeight(400)
, m_SplWidth(600)
, MousePosPrevX(0)
, MousePosPrevY(0)
{  
	wcscpy(m_szAppName, TEXT("ASD"));
	wcscpy(m_szAppSplashName, TEXT("Splash"));
}

//----------------------------------------------------------------------------------------------
CWindowManager::~CWindowManager()
{
	Destroy();
}

//----------------------------------------------------------------------------------------------
int CWindowManager::Initclassex()
{
	m_wcApp.cbSize        = sizeof(m_wcApp);
	m_wcApp.style         = CS_OWNDC|CS_BYTEALIGNWINDOW|CS_BYTEALIGNCLIENT|CS_DBLCLKS;//CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	m_wcApp.lpfnWndProc   = WindowManagerEx::WndProcMain;
	m_wcApp.cbClsExtra    = 0;
	m_wcApp.cbWndExtra    = 0;
	m_wcApp.hInstance     = GetModuleHandle(NULL);
	m_wcApp.hIcon         = LoadIcon(0, IDI_APPLICATION);
	m_wcApp.hCursor       = LoadCursor(0, IDC_ARROW);
	m_wcApp.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_wcApp.lpszMenuName  = 0;
	m_wcApp.lpszClassName = m_szAppName;
	m_wcApp.hIconSm       = NULL;

	if (!RegisterClassEx(&m_wcApp)){
		return 0;
	}
	return 1;
}
//----------------------------------------------------------------------------------------------
int CWindowManager::InitclassexSplash()
{	
	m_wcAppSplash.cbSize        = sizeof(m_wcAppSplash);
	m_wcAppSplash.style         = CS_OWNDC|CS_BYTEALIGNWINDOW|CS_BYTEALIGNCLIENT;
	m_wcAppSplash.lpfnWndProc   = WindowManagerEx::SplashWndProcMain;
	m_wcAppSplash.cbClsExtra    = 0;
	m_wcAppSplash.cbWndExtra    = 0;
	m_wcAppSplash.hInstance     = GetModuleHandle(NULL);
	m_wcAppSplash.hIcon         = NULL;
	m_wcAppSplash.hCursor       = LoadCursor(0, IDC_ARROW);
	m_wcAppSplash.hbrBackground = NULL;
	m_wcAppSplash.lpszMenuName  = NULL;
	m_wcAppSplash.lpszClassName = m_szAppSplashName;
	m_wcAppSplash.hIconSm       = NULL;

	if (!RegisterClassEx(&m_wcAppSplash)){
		return 0;
	}
	return 1;
}

//----------------------------------------------------------------------------------------------
int CWindowManager::Create()
{
	RECT rWindow;
	rWindow.left = 0;
	rWindow.top = 0;
	rWindow.right = (LONG)m_pAppMain->GetRegistry()->Resolution.x; //m_displayModeWidth;
	rWindow.bottom = (LONG)m_pAppMain->GetRegistry()->Resolution.y;

	const DWORD WndStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;

	AdjustWindowRect(&rWindow, WndStyle | WS_VISIBLE, FALSE);

	m_hWnd = CreateWindow(m_szAppName,
		APP_NAME,
		WndStyle,
		100, 0,
		(rWindow.right - rWindow.left),
		(rWindow.bottom - rWindow.top),
		NULL, NULL,
		GetModuleHandle(NULL),
		NULL);

	if (!m_hWnd){
		return 0;
	}

	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);

	LONG lExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	lExStyle &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, lExStyle);

	return 1;
}

//----------------------------------------------------------------------------------------------
int CWindowManager::CreateSplashWindow()
{
	InitclassexSplash();

	m_hSplashBitmap = (HBITMAP)::LoadImage(NULL, L"resources/SplashScreenGame.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (m_hSplashBitmap)
	{
		BITMAP bitmap;
		::GetObject(m_hSplashBitmap,sizeof(BITMAP),&bitmap);

		m_SplWidth = bitmap.bmWidth;
		m_SplHeight = bitmap.bmHeight;
	}

	m_hSplashLogo = (HBITMAP)::LoadImage(NULL, L"resources/Logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (m_hSplashLogo)
	{
		BITMAP bitmap;
		::GetObject(m_hSplashLogo, sizeof(BITMAP),&bitmap);

		m_LogoWidth = bitmap.bmWidth;
		m_LogoHeight = bitmap.bmHeight;
	}

	int width  = GetSystemMetrics(SM_CXFULLSCREEN);
	int height = GetSystemMetrics(SM_CYFULLSCREEN);

	width  = width / 2 - m_SplWidth / 2;
	height = height / 2 - m_SplHeight / 2;
	
	m_hWndSplash = CreateWindow(m_szAppSplashName,
								APP_NAME,
								WS_POPUPWINDOW,
								width, height,
								m_SplWidth, m_SplHeight,
								NULL, NULL,
								GetModuleHandle(NULL),
								NULL);

	if(!m_hWndSplash) {
		return 0;
	}

	SetWindowLong(m_hWndSplash, GWL_USERDATA, (LONG)this);
	ShowSplash();

	return 1;
}

//----------------------------------------------------------------------------------------------
void CWindowManager::Show()
{
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	MSG msg;
	msg.message = WM_NULL;
	PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE);
}

//----------------------------------------------------------------------------------------------
void CWindowManager::ShowSplash()
{
	ShowWindow(m_hWndSplash, SW_SHOWNORMAL);
	UpdateWindow(m_hWndSplash);
	Sleep(2000);
}

//----------------------------------------------------------------------------------------------
void CWindowManager::CloseSplash()
{
	DestroyWindow(m_hWndSplash);
	m_hWndSplash = NULL;
	Show();
	
}
//----------------------------------------------------------------------------------------------
bool CWindowManager::Initialize()
{
	// splash screen
	CreateSplashWindow();

	Initclassex();
	Create();

	return true;
}

//----------------------------------------------------------------------------------------------
void CWindowManager::Destroy()
{
	DestroyWindow(m_hWnd);
}

//----------------------------------------------------------------------------------------------
LRESULT CWindowManager::WndProcMain(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	switch(messg)
	{
	case WM_MOUSEACTIVATE:
	case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{

			}
			else
			{
				SetFocus(hWnd);
				return 0;
			}
		}break;

	case WM_SIZE:
		{
			int Width = (int)GET_X_LPARAM(lParam);
			int Height = (int)GET_Y_LPARAM(lParam);

			m_pAppMain->GetRenderSDK()->ResizeWindow(Width, Height);
		}break;

	case WM_KEYDOWN:
		{
			switch ((DWORD)wParam) 
			{
			case VK_ESCAPE:	m_pAppMain->ProcessInputKey( IE(KEY_ESCAPE, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_RETURN: m_pAppMain->ProcessInputKey( IE(KEY_ENTER, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_F1:		m_pAppMain->ProcessInputKey( IE(KEY_F1, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_F2:		m_pAppMain->ProcessInputKey( IE(KEY_F2, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_F3:		m_pAppMain->ProcessInputKey( IE(KEY_F3, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_F4:		m_pAppMain->ProcessInputKey( IE(KEY_F4, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_F5:		m_pAppMain->ProcessInputKey( IE(KEY_F5, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_F6:		m_pAppMain->ProcessInputKey( IE(KEY_F6, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_F7:		m_pAppMain->ProcessInputKey( IE(KEY_F7, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_F8:		m_pAppMain->ProcessInputKey( IE(KEY_F8, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_F9:		m_pAppMain->ProcessInputKey( IE(KEY_F9, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_F10:	m_pAppMain->ProcessInputKey( IE(KEY_F10, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_F11:	m_pAppMain->ProcessInputKey( IE(KEY_F11, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_F12:	m_pAppMain->ProcessInputKey( IE(KEY_F12, KEY_Pressed, ShiftPressed, CtrlPressed));	break;
			case VK_DELETE:	m_pAppMain->ProcessInputKey( IE(KEY_DELETE, KEY_Pressed, ShiftPressed, CtrlPressed)); break;
			case VK_UP:		m_pAppMain->ProcessInputKey( IE(KEY_UP, KEY_Pressed, ShiftPressed, CtrlPressed)); break;
			case VK_DOWN:	m_pAppMain->ProcessInputKey( IE(KEY_DOWN, KEY_Pressed, ShiftPressed, CtrlPressed)); break;
			case VK_PRIOR:
				{
				}break;
			case VK_NEXT:
				{
				}break;

			case VK_LEFT:   m_pAppMain->ProcessInputKey( IE(KEY_LEFT, KEY_Pressed, ShiftPressed, CtrlPressed));  break;
			case VK_RIGHT:  m_pAppMain->ProcessInputKey( IE(KEY_RIGHT, KEY_Pressed, ShiftPressed, CtrlPressed)); break;
			case VK_SPACE:  m_pAppMain->ProcessInputKey( IE(KEY_SPACE, KEY_Pressed, ShiftPressed, CtrlPressed)); break;

			case VK_SHIFT:	
				{
					ShiftPressed = true;
					m_pAppMain->ProcessInputKey( IE(KEY_LSHIFT, KEY_Pressed, ShiftPressed, CtrlPressed));
				}break;
			case VK_CONTROL:
				{
					CtrlPressed = true;
					m_pAppMain->ProcessInputKey( IE(KEY_LCONTROL, KEY_Pressed, ShiftPressed, CtrlPressed));
				}break;
		};
	}break;

	case WM_KEYUP:
		{
			switch ((DWORD)wParam) 
			{
			case VK_UP:		m_pAppMain->ProcessInputKey( IE(KEY_UP, KEY_Up, ShiftPressed, CtrlPressed)); break;
			case VK_DOWN:   m_pAppMain->ProcessInputKey( IE(KEY_DOWN, KEY_Up, ShiftPressed, CtrlPressed)); break;
			case VK_LEFT:   m_pAppMain->ProcessInputKey( IE(KEY_LEFT, KEY_Up, ShiftPressed, CtrlPressed));  break;
			case VK_RIGHT:  m_pAppMain->ProcessInputKey( IE(KEY_RIGHT, KEY_Up, ShiftPressed, CtrlPressed)); break;
			case VK_SPACE:  m_pAppMain->ProcessInputKey( IE(KEY_SPACE, KEY_Up, ShiftPressed, CtrlPressed)); break;

			case VK_SHIFT:
				{ 
					ShiftPressed = false;
					m_pAppMain->ProcessInputKey( IE(KEY_LSHIFT, KEY_Up, ShiftPressed, CtrlPressed)); 
				}break;
			case VK_CONTROL: 
				{ 
					CtrlPressed = false;	
					m_pAppMain->ProcessInputKey( IE(KEY_LCONTROL, KEY_Up, ShiftPressed, CtrlPressed)); 
				}break;
			};
		}break;

	case WM_CHAR:
		{
			if (wParam == 3){
				m_pAppMain->ProcessEventCopy();
			}

			if (wParam == 22){
				m_pAppMain->ProcessEventPaste();
			}

			switch (wParam)
			{
			case 'Y':
			case 'y':
				m_pAppMain->ProcessInputKey( IE(KEY_Y, KEY_Pressed, ShiftPressed, CtrlPressed)); 
				break;

			case 'U':
			case 'u':
				m_pAppMain->ProcessInputKey( IE(KEY_U, KEY_Pressed, ShiftPressed, CtrlPressed)); 
				break;
			}
		}break;

	case WM_LBUTTONDOWN:
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

			m_pAppMain->ProcessInputMouse(MOUSE_Pressed, MOUSE_LEFT, xPos, yPos, ModifKey);
		}break;

	case WM_NCLBUTTONDOWN:
		{
		}break;

	case WM_RBUTTONDOWN: // context menu
		{
			int ModifKey = 0;
			if (wParam & MK_CONTROL){
				ModifKey |= MK_Control;
			}
			if (wParam & MK_SHIFT){
				ModifKey |= MK_Shift;
			}

			m_pAppMain->ProcessInputMouse(MOUSE_Pressed, MOUSE_RIGHT, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), ModifKey);
		}break;

	case WM_LBUTTONUP:
		{
			int xPos = (int)GET_X_LPARAM(lParam);
			int yPos = (int)GET_Y_LPARAM(lParam);

			RECT rectWindows;
			GetClientRect(hWnd, &rectWindows);

			unsigned int wndWidth = rectWindows.right - rectWindows.left;
			unsigned int wndHeight = rectWindows.bottom - rectWindows.top;

			float xPosRel = (wndWidth > 0.f) ? xPos / (float)wndWidth : 0.f;
			float yPosRel = (wndHeight > 0.f) ? yPos / (float)wndHeight : 0.f;

			unsigned int vprtWidth = m_pAppMain->GetRenderSDK()->GetViewportWidth();
			unsigned int vprtHeight = m_pAppMain->GetRenderSDK()->GetViewportHeight();

			m_pAppMain->ProcessInputMouse(MOUSE_Released, MOUSE_LEFT, (int)(xPosRel * vprtWidth), (int)(yPosRel * vprtHeight));
		}break;

	case WM_LBUTTONDBLCLK:
		{
			int ModifKey = 0;
			if (wParam & MK_CONTROL){
				ModifKey |= MK_Control;
			}
			if (wParam & MK_SHIFT){
				ModifKey |= MK_Shift;
			}
			m_pAppMain->ProcessInputMouse(MOUSE_DoubleClick, MOUSE_LEFT, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}break;

	case WM_MBUTTONDOWN: 
			m_pAppMain->ProcessInputMouse(MOUSE_Pressed, MOUSE_MIDDLE, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_MBUTTONUP: 
			m_pAppMain->ProcessInputMouse(MOUSE_Released, MOUSE_MIDDLE, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_MOUSEMOVE:
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

			int DX = PosX - MousePosPrevX;
			int DY = PosY - MousePosPrevY;
				
			if (DX != 0 || DY != 0)
			{	
				RECT rectWindow;
				GetClientRect(hWnd, &rectWindow);

				unsigned int wndWidth = rectWindow.right - rectWindow.left;
				unsigned int wndHeight = rectWindow.bottom - rectWindow.top;

				unsigned int vprtWidth = m_pAppMain->GetRenderSDK()->GetViewportWidth();
				unsigned int vprtHeight = m_pAppMain->GetRenderSDK()->GetViewportHeight();

				float xPosRel = (wndWidth > 0.f) ? PosX / (float)wndWidth : 0.f;
				float yPosRel = (wndHeight > 0.f) ? PosY / (float)wndHeight : 0.f;

				float xDRel = (wndWidth > 0.f) ? ((DX / (float)wndWidth) * vprtWidth) : 0.f;
				float yDRel = (wndHeight > 0.f) ? ((DY / (float)wndHeight) * vprtHeight) : 0.f;

				m_pAppMain->ProcessMouseMove(xPosRel * vprtWidth, yPosRel * vprtHeight, xDRel, yDRel, ModifKey);
					
				MousePosPrevX = PosX;
				MousePosPrevY = PosY;
			}

		}break;

		// wheel 
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			::GetCursorPos(&pt);
			::ScreenToClient(m_hWnd,&pt);
			int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			m_pAppMain->ProcessMouseWheel(zDelta < 0 ? 0.95f : 1.05f, pt.x, pt.y);
		}break; 

	case WM_CLOSE:
		{
			CWindowManager * pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));
			pWndMgr->QuitApplication();
		}break;
	}
	return DefWindowProc(hWnd, messg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
bool CWindowManager::PeekMessageWindow()
{
	while (m_Msg.message != WM_QUIT)
	{
		if (PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&m_Msg);
			DispatchMessage(&m_Msg);
		}
		else
		{
			return true;  
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------------
void CWindowManager::OutWindowMessageLog(std::string& message)
{
	if (m_hWndSplash != NULL)
	{	
		SplashScreenMessage = message;

		::SendMessage(m_hWndSplash, WM_PAINT, 0, 0);
	}
}

//----------------------------------------------------------------------------------------------
void CWindowManager::QuitApplication()
{
	PostQuitMessage(0);
}

