#include "LauncherStdAfx.h"
/*
#pragma warning(disable: 4312) // disable type conversion warning
#pragma warning(disable: 4311)

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
		CLIENTCREATESTRUCT MDIClientCreateStruct; // Structure to be used for MDI client area

 		switch(messg)
		{
		case WM_CREATE:
			{
				CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
				SStateInfo *pState = reinterpret_cast<SStateInfo*>(pCreate->lpCreateParams);

				CWindowManager *pWndMgr = pState->pMgrWnd;

				MDIClientCreateStruct.hWindowMenu = NULL;
				MDIClientCreateStruct.idFirstChild = IDM_FIRSTCHILD;

				CWindowChildLayoutTop<CWindowController> *pTopWnd = new CWindowChildLayoutTop<CWindowController>(0);
				pTopWnd->Initialize(hWnd, rWindowTop, MDIClientCreateStruct, pWndMgr);
				pWndMgr->m_pCtrlWindowTop = pTopWnd;

				CWindowChildLayoutLeft<CWindowController> *pLeftWnd = new CWindowChildLayoutLeft<CWindowController>(0);
				pLeftWnd->Initialize(hWnd, rWindowLeft, MDIClientCreateStruct, pWndMgr);
				pWndMgr->m_pCtrlWindowLeft = pLeftWnd;

				CWindowChildLayoutRight<CWindowController> *pRight = new CWindowChildLayoutRight<CWindowController>(0);
				pRight->Initialize(hWnd, rWindowRight, MDIClientCreateStruct, pWndMgr);
				pWndMgr->m_pCtrlWindowRight = pRight;

				CWindowChildLayout<CWindowControllerMain> *pChildLayout = new CWindowChildLayout<CWindowControllerMain>(0);
				pChildLayout->Initialize(hWnd, rWindowCenter, MDIClientCreateStruct, pWndMgr);
				pWndMgr->m_pCtrlWindow = pChildLayout;

				CWindowChildBottom *pBottom = new CWindowChildBottom(0);
				pBottom->Initialize(hWnd, rWindowBottom, MDIClientCreateStruct, pWndMgr);
				pWndMgr->m_pCtrlWindowBootom = pBottom;

				{
					CWindowChildBottomTop *pBootomTop = new CWindowChildBottomTop(0);

					RECT rcb;
					rcb.left = rcb.top = 0;
					rcb.right = rWindowBottom.right - rWindowBottom.left;
					rcb.bottom = rWindowBottom.bottom / 2;

					pBootomTop->Initialize(pWndMgr->m_pCtrlWindowBootom->GetHWND(), rcb, MDIClientCreateStruct, pWndMgr);
					pWndMgr->m_pCtrlWindowBootomTop = pBootomTop;
				}

				{
					CWindowChildBottomBottom *pBootomBottom = new CWindowChildBottomBottom(0);
				
					RECT rcbb;
					rcbb.left = 0;
					rcbb.top = rWindowBottom.bottom / 2;
					rcbb.right = rWindowBottom.right - rWindowBottom.left;
					rcbb.bottom = rWindowBottom.bottom / 2;
				
					pBootomBottom->Initialize(pWndMgr->m_pCtrlWindowBootom->GetHWND(), rcbb, MDIClientCreateStruct, pWndMgr);
					pWndMgr->m_pCtrlWindowBootomBottom = pBootomBottom;
				}

				CWindowChildLayoutRightTop<CWindowController> *pRightTop = new CWindowChildLayoutRightTop<CWindowController>(0);
				RECT rc;
				rc.left = rc.top = 0;
				rc.right = rWindowRight.right;
				rc.bottom = rWindowRight.bottom / 2;

				pRightTop->Initialize(pWndMgr->m_pCtrlWindowRight->GetHWND(), rc, MDIClientCreateStruct, pWndMgr);
				pWndMgr->m_pCtrlWindowRightTop = pRightTop;

				CWindowChildLayoutRightBottom *pRightBottom = new CWindowChildLayoutRightBottom(0);
				
				RECT rcrb;
				rcrb.left = 0;
				rcrb.top = rWindowRight.bottom / 2;
				rcrb.right = rWindowRight.right;
				rcrb.bottom = rWindowRight.bottom - rWindowRight.bottom / 2;

				pRightBottom->Initialize(pWndMgr->m_pCtrlWindowRight->GetHWND(), rcrb, MDIClientCreateStruct, pWndMgr);
				pWndMgr->m_pCtrlWindowRightBottom = pRightBottom;

			}break;

			case WM_INITMENU:
			{
				CWindowManager *pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));

				HMENU hmenuBar = GetMenu(hWnd);
				HMENU hmenuPopup;

				assert(hmenuBar);

				MENUITEMINFO mii;
				mii.fMask = MIIM_SUBMENU;
				GetMenuItemInfo(hmenuBar, ID_MAINMENU_FILE_OPEN, FALSE, &mii);
				hmenuPopup = mii.hSubMenu;

				HICON hIcon = pWndMgr->pfnGetOperationBitmap("Open");
				if (hIcon)
				{
					ICONINFO iconinfo;
					GetIconInfo(hIcon, &iconinfo);
					SetMenuItemBitmaps(hmenuBar, ID_MAINMENU_FILE_OPEN, MF_BYCOMMAND, iconinfo.hbmColor, iconinfo.hbmColor);
				}

				hIcon = pWndMgr->pfnGetOperationBitmap("Save");
				if (hIcon)
				{
					ICONINFO iconinfo;
					GetIconInfo(hIcon, &iconinfo);
					SetMenuItemBitmaps(hmenuBar, ID_MAINMENU_FILE_SAVE, MF_BYCOMMAND, iconinfo.hbmColor, iconinfo.hbmColor);
				}
			}break;

		case WM_SIZING:
			{
				bool bSizing = true;
				ESizeDirection Dir = SZ_BottomRight;

				switch (wParam)
				{
				case WMSZ_BOTTOM:
					Dir = SZ_Bottom;
					break;
				case WMSZ_BOTTOMLEFT:
					Dir = SZ_BottomLeft;
					break;
				case WMSZ_BOTTOMRIGHT:
					Dir = SZ_BottomRight;
					break;
				case WMSZ_LEFT:
					Dir = SZ_Left;
					break;
				case WMSZ_RIGHT:
					Dir = SZ_Right;
					break;
				case WMSZ_TOP:
					Dir = SZ_Top;
					break;
				case WMSZ_TOPLEFT:
					Dir = SZ_TopLeft;
					break;
				case WMSZ_TOPRIGHT:
					Dir = SZ_TopRight;
					break;
				default:
					bSizing = false;
					break;
				};

				if (bSizing)
				{
					CWindowManager *pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));
					pWndMgr->Resize((LPRECT)lParam, Dir);
				}
				return bSizing;
			}break;

		case WM_SIZE:
			{
				switch (wParam)
				{
				case SIZE_MAXIMIZED:
				case SIZE_RESTORED:
					{
						CWindowManager *pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));
						pWndMgr->OnWndMaximized(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
						return 0;
					}
					break;
				default:
					break;
				};
			}break;

		case WM_COMMAND:
			{
				CWindowManager *pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));

				switch (LOWORD(wParam)) 
				{
				case ID_MAINMENU_FILE_OPEN:
					{
						OPENFILENAME ofn;
						WCHAR szFileName[MAX_PATH] = L"";
						ZeroMemory(&ofn, sizeof(ofn));
						ofn.lStructSize = sizeof(ofn); 
						ofn.hwndOwner = hWnd;
						ofn.lpstrFilter = L"XML Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
						ofn.lpstrFile = szFileName;
						ofn.nMaxFile = MAX_PATH;
						ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
						ofn.lpstrDefExt = L"txt";

						CHAR chFileName[MAX_PATH] = "";
						if (GetOpenFileName(&ofn) && ConvertWideStringToAnsiCch(chFileName, szFileName, MAX_PATH))
						{
							CWindowManager *pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));
							pWndMgr->GetCoreSDK()->Deserialize(chFileName, NULL);
						}
					}break;

				case ID_FILE_OPENMODEL:
					{
						OPENFILENAME ofn;
						WCHAR szFileName[MAX_PATH] = L"";
						ZeroMemory(&ofn, sizeof(ofn));
						ofn.lStructSize = sizeof(ofn); 
						ofn.hwndOwner = hWnd;
						ofn.lpstrFilter = L"MAT Files (*.mat)\0*.mat\0All Files (*.*)\0*.*\0";
						ofn.lpstrFile = szFileName;
						ofn.nMaxFile = MAX_PATH;
						ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
						ofn.lpstrDefExt = L"txt";

						CHAR chFileName[MAX_PATH] = "";
						if (GetOpenFileName(&ofn) && ConvertWideStringToAnsiCch(chFileName, szFileName, MAX_PATH))
						{
							std::string sFilename;
							//if (FindFile("template_model.xml", &sFilename))
							//{
							//	if (m_pAppMain->Deserialize(sFilename, NULL))
							//	{
							//		OpenFileName = std::string(chFileName);
							//	}
							//}
						}
					}break;

				case ID_MAINMENU_FILE_SAVE:
					{
						OPENFILENAME ofn;
						WCHAR szFileName[MAX_PATH] = L"";
						ZeroMemory(&ofn, sizeof(ofn));
						ofn.lStructSize = sizeof(ofn); 
						ofn.hwndOwner = hWnd;
						ofn.lpstrFilter = L"XML Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
						ofn.lpstrFile = szFileName;
						ofn.nMaxFile = MAX_PATH;
						ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
						ofn.lpstrDefExt = L"txt";

						CHAR chFileName[MAX_PATH] = "";
						if (GetSaveFileName(&ofn) && ConvertWideStringToAnsiCch(chFileName, szFileName, MAX_PATH))
						{
							//m_pAppMain->Serialize(std::string(chFileName));
						}
					}break;

				case ID_OBJECTS_WIREFRAME:
					{
						pWndMgr->GetCoreSDK()->GetRenderSDK()->SetWireframeMode(!pWndMgr->GetCoreSDK()->GetRenderSDK()->GetWireframeMode());
						pWndMgr->UpdateFlagsState();
					}break;

				case ID_OBJECTS_BOUNDS:
					{
						pWndMgr->GetCoreSDK()->SetObjectBoundsVisible(!pWndMgr->GetCoreSDK()->GetObjectBoundsVisible());
						pWndMgr->UpdateFlagsState();
					}break;

				case ID_OBJECTS_SPARITALSUB:
					{
						pWndMgr->GetCoreSDK()->SetSparitalSubdivisionVisible(!pWndMgr->GetCoreSDK()->GetSparitalSubdivisionVisible());
						pWndMgr->UpdateFlagsState();
					}break;

				case ID_OBJECTS_COLLISION:
					{
						pWndMgr->GetCoreSDK()->SetCollisionDebugRender(!pWndMgr->GetCoreSDK()->GetCollisionDebugRender());
						pWndMgr->UpdateFlagsState();
					}break;

				case ID_OBJECTS_GRID:
					{
						pWndMgr->GetCoreSDK()->SetShowGrid(!pWndMgr->GetCoreSDK()->GetShowGrid());
						pWndMgr->UpdateFlagsState();
					}break;

				case ID_BUILD_PLAY:
					{
						pWndMgr->GetCoreSDK()->SetPause(!pWndMgr->GetCoreSDK()->GetPause());
						pWndMgr->UpdateFlagsState();
					}break;

				case ID_MODE_MOVE:
					{
						if (pWndMgr->GetCoreSDK()->GetEditControlMode() != ECM_Move)
						{
							pWndMgr->GetCoreSDK()->SetEditControlMode(ECM_Move);
							pWndMgr->UpdateFlagsState();

							if (CActor* pSelected = static_cast<CWindowChildBottomBottom*>(pWndMgr->m_pCtrlWindowBootomBottom)->GetWidgetPlacement()->GetSelected()){
								pSelected->BroadcastEvent(Event_ChangeEditCtrMode);
							}
						}
					}break;

				case ID_MODE_ROTATE:
					{
						if (pWndMgr->GetCoreSDK()->GetEditControlMode() != ECM_Rotate)
						{
							pWndMgr->GetCoreSDK()->SetEditControlMode(ECM_Rotate);
							pWndMgr->UpdateFlagsState();

							if (CActor* pSelected = static_cast<CWindowChildBottomBottom*>(pWndMgr->m_pCtrlWindowBootomBottom)->GetWidgetPlacement()->GetSelected()){
								pSelected->BroadcastEvent(Event_ChangeEditCtrMode);
							}
						}
					}break;

				case ID_MODE_SCALE:
					{
						if (pWndMgr->GetCoreSDK()->GetEditControlMode() != ECM_Scale)
						{
							pWndMgr->GetCoreSDK()->SetEditControlMode(ECM_Scale);
							pWndMgr->UpdateFlagsState();

							if (CActor* pSelected = static_cast<CWindowChildBottomBottom*>(pWndMgr->m_pCtrlWindowBootomBottom)->GetWidgetPlacement()->GetSelected()){
								pSelected->BroadcastEvent(Event_ChangeEditCtrMode);
							}
						}
					}break;
				default:
					break;
				};
			}break;

		case WM_LBUTTONDOWN:
			{
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
	LRESULT CALLBACK WndProcTop(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		CWindowManager * pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));
		if (pWndMgr){
			return pWndMgr->m_pCtrlWindowTop->WndProc(hWnd, messg, wParam, lParam);
		}
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}

//----------------------------------------------------------------------------------------------
	LRESULT CALLBACK WndProcCenter(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		CWindowManager * pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));
		if (pWndMgr){
			return pWndMgr->m_pCtrlWindow->WndProc(hWnd, messg, wParam, lParam);
		}
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}

//----------------------------------------------------------------------------------------------
	LRESULT CALLBACK WndProcBottom(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		CWindowManager * pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));
		if (pWndMgr){
			return pWndMgr->m_pCtrlWindowBootom->WndProc(hWnd, messg, wParam, lParam);
		}
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}

//----------------------------------------------------------------------------------------------
	LRESULT CALLBACK WndProcBottomTop(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		CWindowManager * pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));
		if (pWndMgr){
			return pWndMgr->m_pCtrlWindowBootomTop->WndProc(hWnd, messg, wParam, lParam);
		}
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}

	//----------------------------------------------------------------------------------------------
	LRESULT CALLBACK WndProcBottomBottom(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		CWindowManager * pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));
		if (pWndMgr){
			return pWndMgr->m_pCtrlWindowBootomBottom->WndProc(hWnd, messg, wParam, lParam);
		}
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}

//----------------------------------------------------------------------------------------------	
	LRESULT CALLBACK WndProcRight(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}

//----------------------------------------------------------------------------------------------
	LRESULT CALLBACK WndProcRightTop(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		CWindowManager * pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));
		if (pWndMgr){
			return pWndMgr->m_pCtrlWindowRightTop->WndProc(hWnd, messg, wParam, lParam);
		}
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}

//----------------------------------------------------------------------------------------------
	LRESULT CALLBACK WndProcRightBottom(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		CWindowManager * pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));
		if (pWndMgr){
			return pWndMgr->m_pCtrlWindowRightBottom->WndProc(hWnd, messg, wParam, lParam);
		}
		return DefWindowProc(hWnd, messg, wParam, lParam);
	}

//----------------------------------------------------------------------------------------------	
	LRESULT CALLBACK WndProcLeft(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		CWindowManager * pWndMgr = reinterpret_cast<CWindowManager*>(GetWindowLong(hWnd, GWL_USERDATA));
		if (pWndMgr){
			return pWndMgr->m_pCtrlWindowLeft->WndProc(hWnd, messg, wParam, lParam);
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

					str = ConvertStringToWideString(STR_SOURCE_COPYRIGHT);
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

//----------------------------------------------------------------------------------------------
	void HandlePopupDeepthSelection(HWND hwnd, POINT pt, std::vector<CActor*> SelectedActors)
	{
		HMENU hmenu;
		HMENU hmenuTrackPopup;

		hmenu = LoadMenu((HINSTANCE)::GetModuleHandle(0), MAKEINTRESOURCE(IDR_USER_MENU));
		if (hmenu == NULL) return;

		hmenuTrackPopup = GetSubMenu(hmenu, 0);

		for (size_t Index = 0; Index < SelectedActors.size(); ++Index)
		{
			MENUITEMINFO iInfo;
			memset(&iInfo, 0, sizeof(MENUITEMINFO));

			iInfo.cbSize = sizeof(MENUITEMINFO);
			iInfo.wID = (unsigned int)Index;
			iInfo.fMask = MIIM_STRING | MIIM_ID;
			iInfo.fType = MFT_STRING;

			wchar_t wbuf[100] = {0};
			MultiByteToWideChar(CP_ACP, 0, SelectedActors[Index]->GetName(), -1, wbuf, 100);

			iInfo.dwTypeData = wbuf;
			iInfo.cch = (unsigned int)strlen(SelectedActors[Index]->GetName());
			InsertMenuItem(hmenuTrackPopup, 0, TRUE, (LPCMENUITEMINFO) &iInfo);
		}
		
		TrackPopupMenuEx(hmenuTrackPopup, TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, hwnd, NULL);
		DestroyMenu(hmenu);
	}

//----------------------------------------------------------------------------------------------
	bool WndProcPopupDeepthSelection(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		bool bResult = false;

		switch (messg)
		{
		case WM_COMMAND:
			{
				INT Index = (INT)(wParam);
				if (Index >= 0 && Index < (int)SelectedActorList.size())
				{
					//SelectedActorList[Index]->GainFocus();
					return true;
				}
			}
			break;
		}
		return false;
	}
}

//----------------------------------------------------------------------------------------------
CWindowManager::CWindowManager(CCoreSDK * appMain)
: m_pAppMain(appMain)
, m_displayModeWidth(1024)
, m_displayModeHeight(768)
, OpenFileName("")
, pfnContextMenu(NULL)
, m_SplHeight(400)
, m_SplWidth(600)
, MousePosPrevX(0)
, MousePosPrevY(0)
, m_bSizing(false)
, m_ESizeDirection(SZ_Left)
, m_hcSizeEW(0)
, m_pImageList(0)
, m_bShiftPressed(false)
, m_bCtrlPressed(false)
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
	m_wcApp.lpszMenuName  = MAKEINTRESOURCE(IDR_MAIN_MENU);
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
	return 0;

	const unsigned int ScrWidth = ::GetSystemMetrics(SM_CXSCREEN);
	const unsigned int ScrHeight = ::GetSystemMetrics(SM_CYSCREEN);

	const unsigned int TopWndHeight = 32;
	const unsigned int LeftWndWidth = 50;
	const unsigned int RightWndWidth = 250;
	const unsigned int BottomWndHeight = 150;

	RECT RectMainClient;
	RectMainClient.left = 0;
	RectMainClient.top = 0;
	RectMainClient.right = ScrWidth;
	RectMainClient.bottom = ScrHeight;

	const DWORD WndStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
	AdjustWindowRect(&RectMainClient, WndStyle, TRUE);

	LONG MainWidth = RectMainClient.right - RectMainClient.left;
	LONG MainHeight = RectMainClient.bottom - RectMainClient.top;

	rWindowMain.left = 0;
	rWindowMain.top = 0;
	rWindowMain.right = MainWidth;
	rWindowMain.bottom = MainHeight;

	rWindowTop.left = 0;
	rWindowTop.top = 0;
	rWindowTop.right = ScrWidth;
	rWindowTop.bottom = TopWndHeight;

	rWindowLeft.left = 0;
	rWindowLeft.top = TopWndHeight;
	rWindowLeft.right = LeftWndWidth;
	rWindowLeft.bottom = ScrHeight - (TopWndHeight + BottomWndHeight);

	rWindowRight.left = ScrWidth - RightWndWidth;
	rWindowRight.top = TopWndHeight;
	rWindowRight.right = RightWndWidth;
	rWindowRight.bottom = ScrHeight - (TopWndHeight + BottomWndHeight);

	rWindowBottom.left = 0;
	rWindowBottom.top = ScrHeight - BottomWndHeight;
	rWindowBottom.right = ScrWidth;
	rWindowBottom.bottom = BottomWndHeight;

	rWindowCenter.left = LeftWndWidth;
	rWindowCenter.top = TopWndHeight;
	rWindowCenter.right = ScrWidth - (RightWndWidth + LeftWndWidth);
	rWindowCenter.bottom = ScrHeight - (TopWndHeight + BottomWndHeight);

	HMODULE HMod = GetModuleHandle(NULL);
	DWORD Err = GetLastError();

	 SStateInfo *pState = new (std::nothrow) SStateInfo;
	 assert(pState);
	 pState->pMgrWnd = this;
		
	m_hWnd = CreateWindow(
		m_szAppName,
		APP_NAME,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		rWindowMain.left,
		rWindowMain.top,
		rWindowMain.right,
		rWindowMain.bottom,
		NULL, 
		NULL,
		HMod,
		pState);

	delete pState;
	
	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_pCtrlWindowTop->GetHWND(), GWL_USERDATA, (LONG)this);
	SetWindowLong(m_pCtrlWindowBootom->GetHWND(), GWL_USERDATA, (LONG)this);
	SetWindowLong(m_pCtrlWindowBootomTop->GetHWND(), GWL_USERDATA, (LONG)this);
	SetWindowLong(m_pCtrlWindowBootomBottom->GetHWND(), GWL_USERDATA, (LONG)this);
	SetWindowLong(m_pCtrlWindowRightBottom->GetHWND(), GWL_USERDATA, (LONG)this);

	SetWindowLong(m_pCtrlWindow->GetHWND(), GWL_WNDPROC, (LONG)WindowManagerEx::WndProcCenter);
	SetWindowLong(m_pCtrlWindowTop->GetHWND(), GWL_WNDPROC, (LONG)WindowManagerEx::WndProcTop);
	SetWindowLong(m_pCtrlWindowBootom->GetHWND(), GWL_WNDPROC, (LONG)WindowManagerEx::WndProcBottom);
	SetWindowLong(m_pCtrlWindowRight->GetHWND(), GWL_WNDPROC, (LONG)WindowManagerEx::WndProcRight);
	SetWindowLong(m_pCtrlWindowLeft->GetHWND(), GWL_WNDPROC, (LONG)WindowManagerEx::WndProcLeft);
	SetWindowLong(m_pCtrlWindowBootomTop->GetHWND(), GWL_WNDPROC, (LONG)WindowManagerEx::WndProcBottomTop);
	SetWindowLong(m_pCtrlWindowBootomBottom->GetHWND(), GWL_WNDPROC, (LONG)WindowManagerEx::WndProcBottomBottom);
	SetWindowLong(m_pCtrlWindowRightTop->GetHWND(), GWL_WNDPROC, (LONG)WindowManagerEx::WndProcRightTop);
	SetWindowLong(m_pCtrlWindowRightBottom->GetHWND(), GWL_WNDPROC, (LONG)WindowManagerEx::WndProcRightBottom);

	LONG lExStyle = GetWindowLong(m_pCtrlWindow->GetHWND(), GWL_EXSTYLE);
	lExStyle &= ~(WS_BORDER);
	SetWindowLong(m_pCtrlWindow->GetHWND(), GWL_EXSTYLE, lExStyle);

	m_hcSizeEW = LoadCursor(NULL, IDC_SIZEWE);
	m_hcSizeNS = LoadCursor(NULL, IDC_SIZENS);
	m_hcSizeNWSE = LoadCursor(NULL, IDC_SIZENWSE);
	m_hcSizeNESW = LoadCursor(NULL, IDC_SIZENESW);

	m_pImageList = new Win32ImageList(m_pCtrlWindowBootomTop->GetHWND(), this);

	m_pWndPackageExplorer = new CWindowPackageExplorer(0, m_hWnd, this);
	m_pWndPackageExplorer->Initialize(GetWindowManagerHWND(), this);

	//ChangeWindowMessageFilter (WM_DROPFILES, MSGFLT_ADD);
	//ChangeWindowMessageFilter (WM_COPYDATA, MSGFLT_ADD);
	//ChangeWindowMessageFilter (0x0049, MSGFLT_ADD);

	SetActiveWindow(m_pCtrlWindow->GetHWND());
	return 1;
}

//----------------------------------------------------------------------------------------------
int CWindowManager::CreateSplashWindow()
{
	InitclassexSplash();

	m_hSplashBitmap = (HBITMAP)::LoadImage(NULL, L"resources/SplashScreen.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (m_hSplashBitmap)
	{
		BITMAP bitmap;
		::GetObject(m_hSplashBitmap, sizeof(BITMAP), &bitmap);

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
bool CWindowManager::Initialize(ContextMenuFunction pfMenu, ContextMenuProcessor pfMenuContext, GetOperationBitmap pfnOperationBitmap)
{
	// splash screen
	CreateSplashWindow();

	pfnContextMenu			= pfMenu;
	pfnContextMenuProcessor = pfMenuContext;
	pfnGetOperationBitmap	= pfnOperationBitmap;

	Initclassex();
	Create();

	return true;
}

//----------------------------------------------------------------------------------------------
void CWindowManager::Destroy()
{	
	m_pCtrlWindow->Free();
	m_pCtrlWindowTop->Free();
	m_pCtrlWindowBootom->Free();
	m_pCtrlWindowBootomTop->Free();
	m_pCtrlWindowBootomBottom->Free();
	m_pCtrlWindowLeft->Free();
	m_pCtrlWindowRight->Free();
	m_pCtrlWindowRightTop->Free();
	m_pCtrlWindowRightBottom->Free();
	m_pWndPackageExplorer->Free();

	DestroyWindow(m_hWnd);

	DeleteObject(m_hcSizeEW);
	DeleteObject(m_hcSizeNS);
	DeleteObject(m_hcSizeNWSE);
	DeleteObject(m_hcSizeNESW);

	SAFE_DELETE(m_pImageList);
	SAFE_DELETE(m_pWndPackageExplorer);

	SAFE_DELETE(m_pCtrlWindowTop);
	SAFE_DELETE(m_pCtrlWindowBootom);
	SAFE_DELETE(m_pCtrlWindowBootomTop);
	SAFE_DELETE(m_pCtrlWindowBootomBottom);
	SAFE_DELETE(m_pCtrlWindowLeft);
	SAFE_DELETE(m_pCtrlWindowRight);
	SAFE_DELETE(m_pCtrlWindowRightTop);
	SAFE_DELETE(m_pCtrlWindowRightBottom);
	SAFE_DELETE(m_pCtrlWindow);
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
void CWindowManager::GetHitChilds(CActor* InActor, Matrix& WTM, Vector2f& MousePos, std::vector<class CActor*> &OutVector)
{
	//IDrawInterface *pIFocused = GetCoreSDK()->GetViewportManager()->GetByActor(InActor);

	//if (pIFocused)
	//{
		// calc object's world matrix
	//	Matrix WorldMatrix = WTM * pIFocused->GetPivot_() * pIFocused->GetSTM_();

	//	if (InActor->HitTest(MousePos))
	//	{
	//		OutVector.push_back(InActor);
	//	}

	//	const Registry * Reg = GetCoreSDK()->GetRegistry();
	//	assert(Reg);

	//	for (TVecActorChildIterator Iter = InActor->ChildNodes.begin(); Iter != InActor->ChildNodes.end(); ++Iter)
	//	{
	//		if ((*Iter)->GetVisible() &&
	//			Reg->IsEditorVisible((*Iter)->GetType()) && 
	//			!dynamic_cast<Brush_AbstractInterface*>(*Iter))
	//		{
	//			GetHitChilds((*Iter), WorldMatrix, MousePos, OutVector);
	//		}
	//	}
	//}
}

//----------------------------------------------------------------------------------------------
void CWindowManager::UpdateFlagsState() const
{
	CheckMenuItem(GetMenu(m_hWnd), ID_OBJECTS_WIREFRAME, m_pAppMain->GetRenderSDK()->GetWireframeMode() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	//CheckMenuItem(GetMenu(m_hWnd), ID_WINDOW_BROWSER, CWin32ObjectBrowser::IsVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	//CheckMenuItem(GetMenu(m_hWnd), ID_WINDOW_TOOLBOX, CWin32ObjectToolbox::IsVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_WINDOW_THREADMONITOR, CWin32ThreadMonitor::IsVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_WINDOWS_CAMERAMANAGER, CWin32CameraManager::IsVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_WINDOWS_ACTIONPLAY, CWin32ActionPlayEditor::IsVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_OBJECTS_BOUNDS, m_pAppMain->GetObjectBoundsVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_OBJECTS_SPARITALSUB, m_pAppMain->GetSparitalSubdivisionVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_OBJECTS_COLLISION, m_pAppMain->GetCollisionDebugRender() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_OBJECTS_GRID, m_pAppMain->GetShowGrid() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_EDITMODE_2D, m_pAppMain->GetEditorInputFlag() & EF_EDIT_2D ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_EDITMODE_3D, m_pAppMain->GetEditorInputFlag() & EF_EDIT_3D ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_OBJECTS_FREEZERENDER, m_pAppMain->GetRenderSDK()->GetFreezeRender() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_OBJECTS_DRAWHELPERS, m_pAppMain->GetRenderSDK()->GetDrawHelpers() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_BUILD_PLAY, m_pAppMain->GetPause() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);

	// ECM State
	CheckMenuItem(GetMenu(m_hWnd), ID_MODE_MOVE, m_pAppMain->GetEditControlMode() == ECM_Move ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_MODE_ROTATE, m_pAppMain->GetEditControlMode() == ECM_Rotate ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(GetMenu(m_hWnd), ID_MODE_SCALE, m_pAppMain->GetEditControlMode() == ECM_Scale ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
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

//----------------------------------------------------------------------------------------------
void CWindowManager::Resize(LPRECT NewRect, ESizeDirection Dir)
{
	const int MinWidth = 40;

	RECT rWindowMain, rWindowMainCl, rWindowTop, rWindowLeft,
		 rWindowRight, rWindowCenter, rWindowBottom, rWindowBottomTop, rWindowBottomBottom;

	GetWindowRect(m_hWnd, &rWindowMain);
	GetClientRect(m_hWnd, &rWindowMainCl);
	GetWindowRect(m_pCtrlWindow->GetHWND(), &rWindowCenter);
	GetWindowRect(m_pCtrlWindowTop->GetHWND(), &rWindowTop);
	GetWindowRect(m_pCtrlWindowLeft->GetHWND(), &rWindowLeft);
	GetWindowRect(m_pCtrlWindowRight->GetHWND(), &rWindowRight);
	GetWindowRect(m_pCtrlWindowBootom->GetHWND(), &rWindowBottom);
	GetWindowRect(m_pCtrlWindowBootomTop->GetHWND(), &rWindowBottomTop);
	GetWindowRect(m_pCtrlWindowBootomBottom->GetHWND(), &rWindowBottomBottom);

	LONG Width = rWindowMain.right - rWindowMain.left;
	LONG Height = rWindowMain.bottom - rWindowMain.top;
  
	LONG NewWidth = NewRect->right - NewRect->left;
	LONG NewHeight = NewRect->bottom - NewRect->top;
 
	if (NewWidth != Width)
	{
		if (NewWidth < Width) // scale down by X
		{
			LONG Delta = Width - NewWidth;

			if (rWindowCenter.right - rWindowCenter.left > MinWidth)
			{
				rWindowCenter.right -= Delta; // scale down central window at first
				rWindowBottom.right -= Delta;
			}
			else
			{
				// else depends of direction scale right or left window
				if (Dir == SZ_Right || 	Dir == SZ_TopRight || Dir == SZ_BottomRight)
				{
					if (rWindowRight.right - rWindowRight.left > MinWidth) // check right window scalability
					{
						rWindowRight.right -= Delta;
					}
					else // otherwise scale opposite window
					{
						if (rWindowLeft.right - rWindowLeft.left > MinWidth)
						{
							rWindowLeft.right -= Delta;
						}
					}
				}
				else if (Dir == SZ_Left || Dir == SZ_TopLeft || Dir == SZ_BottomLeft)
				{
					if (rWindowLeft.right - rWindowLeft.left > MinWidth) // check right window scalability
					{
						rWindowLeft.right -= Delta;
					}
					else // otherwise scale opposite window
					{
						if (rWindowRight.right - rWindowRight.left > MinWidth)
						{
							rWindowRight.right -= Delta;
						}
					}
				}
			}
		}
		else
		{
			unsigned int Delta = NewWidth - Width;
			rWindowCenter.right += Delta;
			//rWindowBottom.right += Delta;
		}
	}

	if (NewHeight != Height) // scale by Y
	{
		if (NewHeight < Height)
		{
			LONG Delta = Height - NewHeight;

			if (rWindowCenter.bottom - rWindowCenter.top > MinWidth)
			{
				rWindowCenter.bottom -= Delta; // scale down central window at first
				rWindowLeft.bottom -= Delta;
				rWindowRight.bottom -= Delta;
			}
			else
			{
				if (rWindowBottom.bottom - rWindowBottom.top > MinWidth)
				{
					rWindowBottom.bottom -= Delta;
					rWindowLeft.bottom -= Delta;
					rWindowRight.bottom -= Delta;
				}
			}
		}
		else
		{
			LONG ClHeight = rWindowMainCl.bottom - rWindowMainCl.top;
			LONG TopHeight = rWindowTop.bottom - rWindowTop.top;

			LONG Delta = NewHeight - Height;

			if (ClHeight - TopHeight >= (MinWidth * 2) + Delta)
			{
				rWindowCenter.bottom += Delta;
				rWindowLeft.bottom = rWindowLeft.top + (ClHeight - TopHeight) + Delta;
				rWindowRight.bottom = rWindowRight.top + (ClHeight - TopHeight) + Delta;
				rWindowBottom.bottom = rWindowBottom.top + (ClHeight - TopHeight - (rWindowCenter.bottom - rWindowCenter.top) + Delta);
			}
		}
	}

	LONG NewLeftWidth = rWindowLeft.right - rWindowLeft.left;
	LONG NewLeftHeight = rWindowLeft.bottom - rWindowLeft.top;

	LONG NewRightWidth = rWindowRight.right - rWindowRight.left;
	LONG NewRightHeight = rWindowRight.bottom - rWindowRight.top;

	LONG WindowTopHeight = rWindowTop.bottom - rWindowTop.top;
	
	LONG NewCentralWidth = rWindowCenter.right - rWindowCenter.left;
	LONG NewCentralHeight = rWindowCenter.bottom - rWindowCenter.top;

	LONG NewBottomWidth = rWindowBottom.right - rWindowBottom.left;
	LONG NewBottomHeight = rWindowBottom.bottom - rWindowBottom.top;

	LONG NewBottomTopWidth = rWindowBottomTop.right - rWindowBottomTop.left;
	LONG NewBottomTopHeight = rWindowBottomTop.bottom - rWindowBottomTop.top;

	LONG NewBottomBottomWidth = rWindowBottomBottom.right - rWindowBottomBottom.left;
	LONG NewBottomBottomHeight = rWindowBottomBottom.bottom - rWindowBottomBottom.top;

	MoveWindow(m_pCtrlWindowLeft->GetHWND(), 0,	WindowTopHeight, NewLeftWidth, NewLeftHeight, TRUE);
	MoveWindow(m_pCtrlWindow->GetHWND(), NewLeftWidth, WindowTopHeight, NewCentralWidth, NewCentralHeight, TRUE);
	MoveWindow(m_pCtrlWindowRight->GetHWND(), NewLeftWidth + NewCentralWidth, WindowTopHeight, NewRightWidth, NewRightHeight, TRUE);
	MoveWindow(m_pCtrlWindowBootom->GetHWND(), 0, WindowTopHeight + NewCentralHeight, NewBottomWidth, NewBottomHeight, TRUE);
	MoveWindow(m_pCtrlWindowBootomTop->GetHWND(), 0, 0, NewBottomTopWidth, NewBottomTopHeight, TRUE);
	MoveWindow(m_pCtrlWindowBootomBottom->GetHWND(), 0, NewBottomTopHeight,	NewBottomBottomWidth, NewBottomBottomHeight, TRUE);
}

//----------------------------------------------------------------------------------------------
void CWindowManager::OnNCLBClick(HWND hWnd, int xPos, int yPos)
{
	RECT rWindowRect;
	if (GetWindowRect(hWnd, &rWindowRect))
	{
		int FrameSize = GetSystemMetrics(SM_CYSIZEFRAME);

		bool bLeft = (xPos >= rWindowRect.left && xPos <= rWindowRect.left + FrameSize);
		bool bRight = (xPos >= rWindowRect.right - FrameSize && xPos <= rWindowRect.right);
		bool bTop = (yPos >= rWindowRect.top && yPos <= rWindowRect.top + FrameSize);
		bool bBottom = (yPos >= rWindowRect.bottom - FrameSize && yPos <= rWindowRect.bottom);

		m_bSizing = bLeft || bRight || bTop || bBottom;
		m_nBorderOffsetX = m_nBorderOffsetY = 0;

		if (m_bSizing)
		{
			if (bLeft && !bBottom && !bTop)
			{
				m_ESizeDirection = SZ_Left;
				m_nBorderOffsetX = xPos - rWindowRect.left;
				SetCursorDir(CD_EW);
			}
			else if (bRight && !bBottom && !bTop)
			{
				m_ESizeDirection = SZ_Right;
				m_nBorderOffsetX = rWindowRect.right - xPos;
				SetCursorDir(CD_EW);
			}
			else if (bTop && !bLeft && !bRight)
			{
				m_ESizeDirection = SZ_Top;
				m_nBorderOffsetY = yPos - rWindowRect.top;
				SetCursorDir(CD_NS);
			}
			else if (bTop && bLeft)
			{
				m_ESizeDirection = SZ_TopLeft;
				m_nBorderOffsetY = yPos - rWindowRect.top;
				m_nBorderOffsetX = xPos - rWindowRect.left;
				SetCursorDir(CD_NWSE);
			}
			else if (bTop && bRight)
			{
				m_ESizeDirection = SZ_TopRight;
				m_nBorderOffsetY = yPos - rWindowRect.top;
				m_nBorderOffsetX = rWindowRect.right - xPos;
				SetCursorDir(CD_NESW);
			}
			else if (bBottom && !bLeft && !bRight)
			{
				m_ESizeDirection = SZ_Bottom;
				m_nBorderOffsetY = rWindowRect.bottom - yPos;
				SetCursorDir(CD_NS);
			}
			else if (bBottom && bLeft)
			{
				m_ESizeDirection = SZ_BottomLeft;
				m_nBorderOffsetY = rWindowRect.bottom - yPos;
				m_nBorderOffsetX = xPos - rWindowRect.left;
				SetCursorDir(CD_NESW);
			}
			else if (bBottom && bRight)
			{
				m_ESizeDirection = SZ_BottomRight;
				m_nBorderOffsetY = rWindowRect.bottom - yPos;
				m_nBorderOffsetX = rWindowRect.right - xPos;
				SetCursorDir(CD_NWSE);
			}

			// Api to capture mouse input				
			m_hWndCaptured = hWnd;
			SetCapture(hWnd);
		}
	}
}

//----------------------------------------------------------------------------------------------
void CWindowManager::OnWndLBRelease(HWND hWnd, int xPos, int yPos)
{
	if (m_bSizing)
	{
		ResizeFinish(m_hWndCaptured, xPos, yPos);
		m_bSizing = false;
		ReleaseCapture();

		SetCursorDir(CD_MAX);
	}
}
//----------------------------------------------------------------------------------------------
void CWindowManager::ResizeFinish(HWND hWnd, int xPos, int yPos)
{
	RECT rWindowMain,
		 rWindowMainCl,
		 rWindowTop,
		 rWindowLeft,
		 rWindowRight,
		 rWindowCenter,
		 rWindowBottom,
		 rWindowBottomTop,
		 rWindowBottomBottom,
		 rWindowRightTop,
		 rWindowRightBottom;

	// actual system size
	GetWindowRect(m_hWnd, &rWindowMain);
	GetClientRect(m_hWnd, &rWindowMainCl);
	GetWindowRect(m_pCtrlWindowTop->GetHWND(), &rWindowTop);
	GetWindowRect(m_pCtrlWindowLeft->GetHWND(), &rWindowLeft);
	GetWindowRect(m_pCtrlWindowRight->GetHWND(), &rWindowRight);
	GetWindowRect(m_pCtrlWindow->GetHWND(), &rWindowCenter);
	GetWindowRect(m_pCtrlWindowBootom->GetHWND(), &rWindowBottom);
	GetWindowRect(m_pCtrlWindowBootomTop->GetHWND(), &rWindowBottomTop);
	GetWindowRect(m_pCtrlWindowBootomBottom->GetHWND(), &rWindowBottomBottom);
	GetWindowRect(m_pCtrlWindowRightTop->GetHWND(), &rWindowRightTop);
	GetWindowRect(m_pCtrlWindowRightBottom->GetHWND(), &rWindowRightBottom);
	
	// actual size
	LONG WindowCenterWidth = rWindowCenter.right - rWindowCenter.left;
	LONG WindowCenterHeight = rWindowCenter.bottom - rWindowCenter.top;
	LONG WindowBottomHeight = rWindowBottom.bottom - rWindowBottom.top;
	LONG WindowRightWidth = rWindowRight.right - rWindowRight.left;
	LONG WindowLeftWidth = rWindowLeft.right - rWindowLeft.left;
	LONG WindowBottomWidth = rWindowBottom.right - rWindowBottom.left;

	LONG WindowBottomTopWidth = rWindowBottomTop.right - rWindowBottomTop.left;
	LONG WindowBottomTopHeight = rWindowBottomTop.bottom - rWindowBottomTop.top;
	LONG WindowBottomBottomWidth = rWindowBottomBottom.right - rWindowBottomBottom.left;
	LONG WindowBottomBottomHeight = rWindowBottomBottom.bottom - rWindowBottomBottom.top;

	LONG WindowRightTopWidth = rWindowRightTop.right - rWindowRightTop.left;
	LONG WindowRightTopHeight = rWindowRightTop.bottom - rWindowRightTop.top;
	LONG WindowRightBottomWidth = rWindowRightBottom.right - rWindowRightBottom.left;
	LONG WindowRightBottomHeight = rWindowRightBottom.bottom - rWindowRightBottom.top;

	// size processing
	if (hWnd == m_pCtrlWindow->GetHWND())
	{
		ResizeFinishCentral(xPos, yPos,
			WindowCenterWidth,
			WindowCenterHeight,
			WindowBottomHeight,
			WindowRightWidth,
			WindowLeftWidth,
			WindowBottomWidth,
			WindowBottomTopWidth,
			WindowBottomTopHeight,
			WindowBottomBottomWidth,
			WindowBottomBottomHeight,
			WindowRightTopWidth,
			WindowRightTopHeight,
			WindowRightBottomWidth,
			WindowRightBottomHeight,
			rWindowMain,
			rWindowMainCl,
			rWindowTop,
			rWindowLeft,
			rWindowRight,
			rWindowCenter,
			rWindowBottom,
			rWindowBottomTop,
			rWindowBottomBottom,
			rWindowRightTop,
			rWindowRightBottom);
	}
	else if (hWnd == m_pCtrlWindowRight->GetHWND())
	{
		// m_hWndRightTop & m_hWndRightBottom
	}
	else if (hWnd == m_pCtrlWindowLeft->GetHWND())
	{
		ResizeFinishLeft(xPos, yPos,
			WindowCenterWidth,
			WindowCenterHeight,
			WindowBottomHeight,
			WindowRightWidth,
			WindowLeftWidth,
			WindowBottomWidth,
			WindowBottomTopWidth,
			WindowBottomTopHeight,
			WindowBottomBottomWidth,
			WindowBottomBottomHeight,
			WindowRightTopWidth,
			WindowRightTopHeight,
			WindowRightBottomWidth,
			WindowRightBottomHeight,
			rWindowMain,
			rWindowMainCl,
			rWindowTop,
			rWindowLeft,
			rWindowRight,
			rWindowCenter,
			rWindowBottom,
			rWindowBottomTop,
			rWindowBottomBottom,
			rWindowRightTop,
			rWindowRightBottom);
	}
	else if (hWnd == m_pCtrlWindowBootom->GetHWND())
	{
		ResizeFinishBottom(xPos, yPos,
			WindowCenterWidth,
			WindowCenterHeight,
			WindowBottomHeight,
			WindowRightWidth,
			WindowLeftWidth,
			WindowBottomWidth,
			WindowBottomTopWidth,
			WindowBottomTopHeight,
			WindowBottomBottomWidth,
			WindowBottomBottomHeight,
			WindowRightTopWidth,
			WindowRightTopHeight,
			WindowRightBottomWidth,
			WindowRightBottomHeight,
			rWindowMain,
			rWindowMainCl,
			rWindowTop,
			rWindowLeft,
			rWindowRight,
			rWindowCenter,
			rWindowBottom,
			rWindowBottomTop,
			rWindowBottomBottom,
			rWindowRightTop,
			rWindowRightBottom);
	}
	else if (hWnd == m_pCtrlWindowBootomTop->GetHWND())
	{
		ResizeFinishBottomTop(xPos, yPos,
			WindowCenterWidth,
			WindowCenterHeight,
			WindowBottomHeight,
			WindowRightWidth,
			WindowLeftWidth,
			WindowBottomWidth,
			WindowBottomTopWidth,
			WindowBottomTopHeight,
			WindowBottomBottomWidth,
			WindowBottomBottomHeight,
			WindowRightTopWidth,
			WindowRightTopHeight,
			WindowRightBottomWidth,
			WindowRightBottomHeight,
			rWindowMain,
			rWindowMainCl,
			rWindowTop,
			rWindowLeft,
			rWindowRight,
			rWindowCenter,
			rWindowBottom,
			rWindowBottomTop,
			rWindowBottomBottom,
			rWindowRightTop,
			rWindowRightBottom);
	}
	else if (hWnd == m_pCtrlWindowBootomBottom->GetHWND())
	{
		ResizeFinishBottomBottom(xPos, yPos,
			WindowCenterWidth,
			WindowCenterHeight,
			WindowBottomHeight,
			WindowRightWidth,
			WindowLeftWidth,
			WindowBottomWidth,
			WindowBottomTopWidth,
			WindowBottomTopHeight,
			WindowBottomBottomWidth,
			WindowBottomBottomHeight,
			WindowRightTopWidth,
			WindowRightTopHeight,
			WindowRightBottomWidth,
			WindowRightBottomHeight,
			rWindowMain,
			rWindowMainCl,
			rWindowTop,
			rWindowLeft,
			rWindowRight,
			rWindowCenter,
			rWindowBottom,
			rWindowBottomTop,
			rWindowBottomBottom,
			rWindowRightTop,
			rWindowRightBottom);
	}
	else if (hWnd == m_pCtrlWindowRightTop->GetHWND())
	{
		ResizeFinishRightTop(xPos, yPos,
			WindowCenterWidth,
			WindowCenterHeight,
			WindowBottomHeight,
			WindowRightWidth,
			WindowLeftWidth,
			WindowBottomWidth,
			WindowBottomTopWidth,
			WindowBottomTopHeight,
			WindowBottomBottomWidth,
			WindowBottomBottomHeight,
			WindowRightTopWidth,
			WindowRightTopHeight,
			WindowRightBottomWidth,
			WindowRightBottomHeight,
			rWindowMain,
			rWindowMainCl,
			rWindowTop,
			rWindowLeft,
			rWindowRight,
			rWindowCenter,
			rWindowBottom,
			rWindowBottomTop,
			rWindowBottomBottom,
			rWindowRightTop,
			rWindowRightBottom);
	}
	else if (hWnd == m_pCtrlWindowRightBottom->GetHWND())
	{
		ResizeFinishRightBottom(xPos, yPos,
			WindowCenterWidth,
			WindowCenterHeight,
			WindowBottomHeight,
			WindowRightWidth,
			WindowLeftWidth,
			WindowBottomWidth,
			WindowBottomTopWidth,
			WindowBottomTopHeight,
			WindowBottomBottomWidth,
			WindowBottomBottomHeight,
			WindowRightTopWidth,
			WindowRightTopHeight,
			WindowRightBottomWidth,
			WindowRightBottomHeight,
			rWindowMain,
			rWindowMainCl,
			rWindowTop,
			rWindowLeft,
			rWindowRight,
			rWindowCenter,
			rWindowBottom,
			rWindowBottomTop,
			rWindowBottomBottom,
			rWindowRightTop,
			rWindowRightBottom);
	}

	// calculate new adjusted size
	LONG NewLeftWidth = rWindowLeft.right - rWindowLeft.left;
	LONG NewLeftHeight = rWindowLeft.bottom - rWindowLeft.top;

	LONG NewRightWidth = rWindowRight.right - rWindowRight.left;
	LONG NewRightHeight = rWindowRight.bottom - rWindowRight.top;

	LONG NewTopHeight = rWindowTop.bottom - rWindowTop.top;

	LONG NewCentralWidth = rWindowCenter.right - rWindowCenter.left;
	LONG NewCentralHeight = rWindowCenter.bottom - rWindowCenter.top;

	LONG NewBottomWidth = rWindowBottom.right - rWindowBottom.left;
	LONG NewBottomHeight = rWindowBottom.bottom - rWindowBottom.top;

	LONG NewBottomTopWidth = rWindowBottomTop.right - rWindowBottomTop.left;
	LONG NewBottomTopHeight = rWindowBottomTop.bottom - rWindowBottomTop.top;

	LONG NewBottomBottomWidth = rWindowBottomBottom.right - rWindowBottomBottom.left;
	LONG NewBottomBottomHeight = rWindowBottomBottom.bottom - rWindowBottomBottom.top;

	LONG NewRightTopHeight = rWindowRightTop.bottom - rWindowRightTop.top;
	LONG NewRightBottomHeight = rWindowRightBottom.bottom - rWindowRightBottom.top;

	// setup new window size
	MoveWindow(m_pCtrlWindowTop->GetHWND(), 0, 0, NewLeftWidth + NewCentralWidth + NewRightWidth, NewTopHeight, TRUE);
	MoveWindow(m_pCtrlWindowLeft->GetHWND(), 0, NewTopHeight, NewLeftWidth, NewLeftHeight, TRUE);
	MoveWindow(m_pCtrlWindow->GetHWND(), NewLeftWidth, NewTopHeight, NewCentralWidth, NewCentralHeight,	TRUE);
	MoveWindow(m_pCtrlWindowRight->GetHWND(), NewLeftWidth + NewCentralWidth, NewTopHeight, NewRightWidth, NewRightHeight, TRUE);
	MoveWindow(m_pCtrlWindowBootom->GetHWND(), 0, NewTopHeight + NewCentralHeight,	NewBottomWidth,	NewBottomHeight, TRUE);
	
	MoveWindow(m_pCtrlWindowBootomTop->GetHWND(), 0, 0, NewBottomTopWidth, NewBottomTopHeight, TRUE);
	MoveWindow(m_pCtrlWindowBootomBottom->GetHWND(), 0, NewBottomTopHeight, NewBottomBottomWidth, NewBottomHeight - NewBottomTopHeight, TRUE);
	MoveWindow(m_pCtrlWindowRightTop->GetHWND(), 0, 0, NewRightWidth, NewRightTopHeight, TRUE);
	MoveWindow(m_pCtrlWindowRightBottom->GetHWND(), 0, NewRightTopHeight, NewRightWidth, NewRightBottomHeight, TRUE);
}

//----------------------------------------------------------------------------------------------
void CWindowManager::ResizeFinishCentral(int xPos, int yPos,
										 LONG WindowCenterWidth,
										 LONG WindowCenterHeight,
										 LONG WindowBottomHeight,
										 LONG WindowRightWidth,
										 LONG WindowLeftWidth,
										 LONG WindowBottomWidth,
										 LONG WindowBottomTopWidth,
										 LONG WindowBottomTopHeight,
										 LONG WindowBottomBottomWidth,
										 LONG WindowBottomBottomHeight,
										 LONG WindowRightTopWidth,
										 LONG WindowRightTopHeight,
										 LONG WindowRightBottomWidth,
										 LONG WindowRightBottomHeight,
										 RECT &rWindowMain,
										 RECT &rWindowMainCl,
										 RECT &rWindowTop,
										 RECT &rWindowLeft,
										 RECT &rWindowRight,
										 RECT &rWindowCenter,
										 RECT &rWindowBottom,
										 RECT &rWindowBottomTop,
										 RECT &rWindowBottomBottom,
										 RECT &rWindowRightTop,
										 RECT &rWindowRightBottom)
{
	// Horizontal 
	if (m_ESizeDirection == SZ_Left || m_ESizeDirection == SZ_TopLeft || m_ESizeDirection == SZ_BottomLeft)
	{
		if (WindowCenterWidth + WindowLeftWidth > (MinWndSize * 2)) // check minimum size restriction
		{
			LONG NewWidth = WindowCenterWidth - (xPos - m_nBorderOffsetX);

			if (NewWidth < MinWndSize) { // check minimum size
				NewWidth = MinWndSize;
			}
			else if ((WindowLeftWidth + WindowCenterWidth) - NewWidth < MinWndSize) { // left window resize clamp
				NewWidth = (WindowLeftWidth + WindowCenterWidth) - MinWndSize; // new size out of bounds 
			}

			LONG Delta = WindowCenterWidth - NewWidth;

			// apply new dimensions
			rWindowCenter.left = rWindowLeft.right += Delta;
		}
	}

	if (m_ESizeDirection == SZ_Right ||	m_ESizeDirection == SZ_TopRight || m_ESizeDirection == SZ_BottomRight)
	{
		if (WindowCenterWidth + WindowRightWidth > (MinWndSize * 2))
		{
			LONG NewWidth = xPos + m_nBorderOffsetX;

			if (NewWidth < MinWndSize) {
				NewWidth = MinWndSize;
			}
			else if ((WindowCenterWidth + WindowRightWidth) - NewWidth < MinWndSize) { // right window resize clamp
				NewWidth = (WindowCenterWidth + WindowRightWidth) - MinWndSize; // new size out of bounds 
			}

			// apply new dimensions
			rWindowCenter.right = rWindowCenter.left + NewWidth;
			rWindowRight.left = rWindowCenter.right;
		}
	}

	// Vertical
	if (m_ESizeDirection == SZ_Bottom || m_ESizeDirection == SZ_BottomLeft || m_ESizeDirection == SZ_BottomRight)
	{
		if (WindowCenterHeight + WindowBottomHeight > (MinWndSize * 2))
		{
			LONG NewHeight = yPos + m_nBorderOffsetY;

			if (NewHeight < MinWndSize) { // clamp to mim size
				NewHeight = MinWndSize;
			}
			else if ((WindowCenterHeight + WindowBottomHeight) - NewHeight < MinWndSize) // bottom clamp
			{
				NewHeight = (WindowCenterHeight + WindowBottomHeight) - MinWndSize;
			}

			LONG Delta = NewHeight - WindowCenterHeight;

			rWindowCenter.bottom = rWindowBottom.top = rWindowLeft.bottom =	rWindowRight.bottom += Delta;

			AdjustRightFrameHeight(Delta, WindowRightTopHeight, WindowRightBottomHeight, rWindowRightTop, rWindowRightBottom);

			if (WindowBottomTopHeight + WindowBottomBottomHeight > MinWndSize)
			{
				// bottom adjusting
				if (WindowBottomBottomHeight - Delta > (MinWndSize / 2)) // bottom is scalable
				{
					rWindowBottomBottom.bottom -= Delta;
				}
				else
				{
					if (WindowBottomTopHeight - Delta > (MinWndSize / 2)) // top is scalable
					{
						rWindowBottomTop.bottom -= Delta;
						rWindowBottomBottom.top += Delta;
					}
					else
					{
						rWindowBottomTop.bottom = rWindowBottomTop.top + (MinWndSize / 2);
						rWindowBottomBottom.bottom = rWindowBottomBottom.top + (MinWndSize / 2);
					}
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void CWindowManager::ResizeFinishLeft(int xPos, int yPos,
						LONG WindowCenterWidth,
						LONG WindowCenterHeight,
						LONG WindowBottomHeight,
						LONG WindowRightWidth,
						LONG WindowLeftWidth,
						LONG WindowBottomWidth,
						LONG WindowBottomTopWidth,
						LONG WindowBottomTopHeight,
						LONG WindowBottomBottomWidth,
						LONG WindowBottomBottomHeight,
						LONG WindowRightTopWidth,
						LONG WindowRightTopHeight,
						LONG WindowRightBottomWidth,
						LONG WindowRightBottomHeight,
						RECT &rWindowMain,
						RECT &rWindowMainCl,
						RECT &rWindowTop,
						RECT &rWindowLeft,
						RECT &rWindowRight,
						RECT &rWindowCenter,
						RECT &rWindowBottom,
						RECT &rWindowBottomTop,
						RECT &rWindowBottomBottom,
						RECT &rWindowRightTop,
						RECT &rWindowRightBottom)
{
	if (m_ESizeDirection == SZ_Right ||	m_ESizeDirection == SZ_TopRight || m_ESizeDirection == SZ_BottomRight)
	{
		if (WindowLeftWidth + WindowCenterWidth > (MinWndSize * 2))
		{
			LONG NewWidth = xPos + m_nBorderOffsetX;

			if (NewWidth < MinWndSize){
				NewWidth = MinWndSize;
			}
			else if ((WindowLeftWidth + WindowCenterWidth) - NewWidth < MinWndSize){
				NewWidth = (WindowLeftWidth + WindowCenterWidth) - MinWndSize;
			}

			// apply new dimensions
			LONG Delta = NewWidth - WindowLeftWidth;

			rWindowCenter.left = rWindowBottom.left = rWindowLeft.right += Delta;

			AdjustRightFrameHeight(Delta, WindowRightTopHeight, WindowRightBottomHeight, rWindowRightTop, rWindowRightBottom);
		}
	}

	if (m_ESizeDirection == SZ_Bottom || m_ESizeDirection == SZ_BottomLeft || m_ESizeDirection == SZ_BottomRight)
	{
		if (WindowCenterHeight + WindowBottomHeight > (MinWndSize * 2))
		{
			LONG NewHeight = yPos + m_nBorderOffsetY;

			if (NewHeight < MinWndSize) { // clamp to mim size
				NewHeight = MinWndSize;
			}
			else if ((WindowCenterHeight + WindowBottomHeight) - NewHeight < MinWndSize){ // bottom clamp
				NewHeight = (WindowCenterHeight + WindowBottomHeight) - MinWndSize;
			}

			LONG Delta = NewHeight - WindowCenterHeight;

			rWindowCenter.bottom = rWindowBottom.top = rWindowLeft.bottom = rWindowRight.bottom += Delta;

			AdjustRightFrameHeight(Delta, WindowRightTopHeight, WindowRightBottomHeight, rWindowRightTop, rWindowRightBottom);

			if (WindowBottomTopHeight + WindowBottomBottomHeight > MinWndSize)
			{
				// bottom adjusting
				if (WindowBottomBottomHeight - Delta > (MinWndSize / 2)) // bottom is scalable
				{
					rWindowBottomBottom.bottom -= Delta;
				}
				else
				{
					if (WindowBottomTopHeight - Delta > (MinWndSize / 2)) // top is scalable
					{
						rWindowBottomTop.bottom -= Delta;
						rWindowBottomBottom.top += Delta;
					}
					else
					{
						rWindowBottomTop.bottom = rWindowBottomTop.top + (MinWndSize / 2);
						rWindowBottomBottom.bottom = rWindowBottomBottom.top + (MinWndSize / 2);
					}
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void CWindowManager::ResizeFinishBottom(int xPos, int yPos,
										LONG WindowCenterWidth,
										LONG WindowCenterHeight,
										LONG WindowBottomHeight,
										LONG WindowRightWidth,
										LONG WindowLeftWidth,
										LONG WindowBottomWidth,
										LONG WindowBottomTopWidth,
										LONG WindowBottomTopHeight,
										LONG WindowBottomBottomWidth,
										LONG WindowBottomBottomHeight,
										LONG WindowRightTopWidth,
										LONG WindowRightTopHeight,
										LONG WindowRightBottomWidth,
										LONG WindowRightBottomHeight,
										RECT &rWindowMain,
										RECT &rWindowMainCl,
										RECT &rWindowTop,
										RECT &rWindowLeft,
										RECT &rWindowRight,
										RECT &rWindowCenter,
										RECT &rWindowBottom,
										RECT &rWindowBottomTop,
										RECT &rWindowBottomBottom,
										RECT &rWindowRightTop,
										RECT &rWindowRightBottom)
{
	if (m_ESizeDirection == SZ_Top || m_ESizeDirection == SZ_TopRight || m_ESizeDirection == SZ_TopLeft)
	{
		if (WindowCenterHeight + WindowBottomHeight > (MinWndSize * 2))
		{
			LONG NewHeight = WindowBottomHeight - (yPos + m_nBorderOffsetY);

			if (NewHeight < MinWndSize){
				NewHeight = MinWndSize;
			}
			else if ((WindowCenterHeight + WindowBottomHeight) - NewHeight < MinWndSize)
			{
				NewHeight = (WindowCenterHeight + WindowBottomHeight) - MinWndSize;
			}

			LONG Delta = WindowBottomHeight - NewHeight;

			rWindowBottom.top += Delta;
			rWindowLeft.bottom = rWindowRight.bottom = rWindowCenter.bottom += Delta;

			AdjustRightFrameHeight(Delta, WindowRightTopHeight, WindowRightBottomHeight, rWindowRightTop, rWindowRightBottom);

			if (WindowBottomTopHeight + WindowBottomBottomHeight > MinWndSize)
			{
				// bottom adjusting
				if (WindowBottomBottomHeight - Delta > (MinWndSize / 2)) // bottom is scalable
				{
					rWindowBottomBottom.bottom -= Delta;
				}
				else
				{
					if (WindowBottomTopHeight - Delta > (MinWndSize / 2)) // top is scalable
					{
						rWindowBottomTop.bottom -= Delta;
						rWindowBottomBottom.top += Delta;
					}
					else
					{
						rWindowBottomTop.bottom = rWindowBottomTop.top + (MinWndSize / 2);
						rWindowBottomBottom.bottom = rWindowBottomBottom.top + (MinWndSize / 2);
					}
				}
			}
		}
	}

	if (m_ESizeDirection == SZ_Left || m_ESizeDirection == SZ_TopLeft || m_ESizeDirection == SZ_BottomLeft)
	{
		if (WindowCenterWidth + WindowLeftWidth > (MinWndSize * 2))
		{
			LONG NewWidth = WindowBottomWidth - (xPos - m_nBorderOffsetX);

			if (NewWidth < MinWndSize){
				NewWidth = MinWndSize;
			}
			else if ((WindowLeftWidth + WindowCenterWidth) - NewWidth < MinWndSize){
				NewWidth = (WindowLeftWidth + WindowCenterWidth) - MinWndSize;
			}

			// apply new dimensions
			LONG Delta = WindowBottomWidth - NewWidth;

			rWindowLeft.right += Delta;
			rWindowCenter.left = rWindowBottom.left = rWindowLeft.right;

			rWindowBottomTop.right -= Delta;
			rWindowBottomBottom.right -= Delta;
		}
	}

	if (m_ESizeDirection == SZ_Right ||	m_ESizeDirection == SZ_TopRight || m_ESizeDirection == SZ_BottomRight)
	{
		if (WindowCenterWidth + WindowRightWidth > (MinWndSize * 2))
		{
			LONG NewWidth = xPos + m_nBorderOffsetX;

			if (NewWidth < MinWndSize){
				NewWidth = MinWndSize;
			}
			else if ((WindowCenterWidth + WindowRightWidth) - NewWidth < MinWndSize){
				NewWidth = (WindowCenterWidth + WindowRightWidth) - MinWndSize;
			}

			// apply new dimensions
			LONG Delta = WindowBottomWidth - NewWidth;

			rWindowRight.left -= Delta;
			rWindowCenter.right = rWindowBottom.right = rWindowRight.left;

			rWindowBottomTop.right -= Delta;
			rWindowBottomBottom.right -= Delta;
		}
	}
}

void CWindowManager::ResizeFinishBottomTop(int xPos, int yPos,
										   LONG WindowCenterWidth,
										   LONG WindowCenterHeight,
										   LONG WindowBottomHeight,
										   LONG WindowRightWidth,
										   LONG WindowLeftWidth,
										   LONG WindowBottomWidth,
										   LONG WindowBottomTopWidth,
										   LONG WindowBottomTopHeight,
										   LONG WindowBottomBottomWidth,
										   LONG WindowBottomBottomHeight,
										   LONG WindowRightTopWidth,
										   LONG WindowRightTopHeight,
										   LONG WindowRightBottomWidth,
										   LONG WindowRightBottomHeight,
										   RECT &rWindowMain,
										   RECT &rWindowMainCl,
										   RECT &rWindowTop,
										   RECT &rWindowLeft,
										   RECT &rWindowRight,
										   RECT &rWindowCenter,
										   RECT &rWindowBottom,
										   RECT &rWindowBottomTop,
										   RECT &rWindowBottomBottom,
										   RECT &rWindowRightTop,
										   RECT &rWindowRightBottom)
{
	if (m_ESizeDirection == SZ_Top || m_ESizeDirection == SZ_TopRight || m_ESizeDirection == SZ_TopLeft)
	{
		if (WindowCenterHeight + WindowBottomHeight > (MinWndSize * 2))
		{
			LONG NewHeight = WindowBottomHeight - (yPos + m_nBorderOffsetY);

			if (NewHeight < MinWndSize){
				NewHeight = MinWndSize;
			}
			else if ((WindowCenterHeight + WindowBottomHeight) - NewHeight < MinWndSize){
				NewHeight = (WindowCenterHeight + WindowBottomHeight) - MinWndSize;
			}

			// apply new dimensions
			LONG Delta = WindowBottomHeight - NewHeight;

			rWindowBottom.top += Delta;
			rWindowRight.bottom = rWindowLeft.bottom = rWindowCenter.bottom += Delta;

			AdjustRightFrameHeight(Delta, WindowRightTopHeight, WindowRightBottomHeight, rWindowRightTop, rWindowRightBottom);

			// bottom adjusting
			if (WindowBottomBottomHeight - Delta > (MinWndSize / 2)) // bottom is scalable
			{
				rWindowBottomBottom.bottom -= Delta;
			}
			else
			{
				if (WindowBottomTopHeight - Delta > (MinWndSize / 2)) // top is scalable
				{
					rWindowBottomTop.bottom -= Delta;
					rWindowBottomBottom.top += Delta;
				}
				else
				{
					rWindowBottomTop.bottom = rWindowBottomTop.top + (MinWndSize / 2);
					rWindowBottomBottom.bottom = rWindowBottomBottom.top + (MinWndSize / 2);
				}
			}
		}
	}

	if (m_ESizeDirection == SZ_Left || m_ESizeDirection == SZ_TopLeft || m_ESizeDirection == SZ_BottomLeft)
	{
		if (WindowCenterWidth + WindowLeftWidth > (MinWndSize * 2))
		{
			LONG NewWidth = WindowBottomWidth - (xPos - m_nBorderOffsetX);

			if (NewWidth < MinWndSize){
				NewWidth = MinWndSize;
			}
			else if ((WindowLeftWidth + WindowCenterWidth) - NewWidth < MinWndSize){
				NewWidth = (WindowLeftWidth + WindowCenterWidth) - MinWndSize;
			}

			// apply new dimensions
			LONG Delta = WindowBottomWidth - NewWidth;

			rWindowLeft.right += Delta;
			rWindowCenter.left = rWindowBottom.left = rWindowLeft.right;

			rWindowBottomTop.right -= Delta;
			rWindowBottomBottom.right -= Delta;
		}
	}

	if (m_ESizeDirection == SZ_Right ||	m_ESizeDirection == SZ_TopRight || m_ESizeDirection == SZ_BottomRight)
	{
		if (WindowCenterWidth + WindowRightWidth > (MinWndSize * 2))
		{
			LONG NewWidth = xPos + m_nBorderOffsetX;

			if (NewWidth < MinWndSize){
				NewWidth = MinWndSize;
			}
			else if ((WindowCenterWidth + WindowRightWidth) - NewWidth < MinWndSize){
				NewWidth = (WindowCenterWidth + WindowRightWidth) - MinWndSize;
			}

			// apply new dimensions
			LONG Delta = WindowBottomWidth - NewWidth;

			rWindowRight.left -= Delta;
			rWindowCenter.right = rWindowBottom.right = rWindowRight.left;

			rWindowBottomTop.right -= Delta;
			rWindowBottomBottom.right -= Delta;
		}
	}

	if (m_ESizeDirection == SZ_Bottom || m_ESizeDirection == SZ_BottomLeft || m_ESizeDirection == SZ_BottomRight)
	{
		if (WindowBottomTopHeight + WindowBottomBottomHeight > MinWndSize)
		{
			LONG NewHeight = yPos + m_nBorderOffsetY;

			if (NewHeight < MinWndSize / 2) {
				NewHeight = MinWndSize / 2;
			}
			else if ((WindowBottomTopHeight + WindowBottomBottomHeight) -  NewHeight < (MinWndSize / 2)) { // bottom clamp
				NewHeight = (WindowBottomTopHeight + WindowBottomBottomHeight) - (MinWndSize / 2);
			}
			rWindowBottomTop.bottom = rWindowBottomTop.top + NewHeight;
			rWindowBottomBottom.top = rWindowBottomTop.bottom;
		}
	}
}

void CWindowManager::ResizeFinishBottomBottom(int xPos, int yPos,
											  LONG WindowCenterWidth,
											  LONG WindowCenterHeight,
											  LONG WindowBottomHeight,
											  LONG WindowRightWidth,
											  LONG WindowLeftWidth,
											  LONG WindowBottomWidth,
											  LONG WindowBottomTopWidth,
											  LONG WindowBottomTopHeight,
											  LONG WindowBottomBottomWidth,
											  LONG WindowBottomBottomHeight,
											  LONG WindowRightTopWidth,
											  LONG WindowRightTopHeight,
											  LONG WindowRightBottomWidth,
											  LONG WindowRightBottomHeight,
											  RECT &rWindowMain,
											  RECT &rWindowMainCl,
											  RECT &rWindowTop,
											  RECT &rWindowLeft,
											  RECT &rWindowRight,
											  RECT &rWindowCenter,
											  RECT &rWindowBottom,
											  RECT &rWindowBottomTop,
											  RECT &rWindowBottomBottom,
											  RECT &rWindowRightTop,
											  RECT &rWindowRightBottom)
{
	if (m_ESizeDirection == SZ_Top || m_ESizeDirection == SZ_TopRight || m_ESizeDirection == SZ_TopLeft)
	{
		if (WindowBottomTopHeight + WindowBottomBottomHeight > (MinWndSize))
		{
			LONG NewHeight = WindowBottomBottomHeight - (yPos + m_nBorderOffsetY);

			if (NewHeight < MinWndSize / 2){
				NewHeight = MinWndSize / 2;
			}
			else if ((WindowBottomTopHeight + WindowBottomBottomHeight) -  NewHeight < (MinWndSize / 2)){
				NewHeight = (WindowCenterHeight + WindowBottomHeight) - (MinWndSize / 2);
			}

			// apply new dimensions
			LONG Delta = WindowBottomBottomHeight - NewHeight;

			rWindowBottomBottom.top += Delta;
			rWindowBottomTop.bottom += Delta;
		}
	}

	if (m_ESizeDirection == SZ_Left || m_ESizeDirection == SZ_TopLeft || m_ESizeDirection == SZ_BottomLeft)
	{
		if (WindowCenterWidth + WindowLeftWidth > (MinWndSize * 2))
		{
			LONG NewWidth = WindowBottomWidth - (xPos - m_nBorderOffsetX);

			if (NewWidth < MinWndSize){
				NewWidth = MinWndSize;
			}
			else if ((WindowLeftWidth + WindowCenterWidth) - NewWidth < MinWndSize){
				NewWidth = (WindowLeftWidth + WindowCenterWidth) - MinWndSize;
			}

			// apply new dimensions
			LONG Delta = WindowBottomWidth - NewWidth;

			rWindowLeft.right += Delta;
			rWindowCenter.left = rWindowBottom.left = rWindowLeft.right;

			rWindowBottomTop.right -= Delta;
			rWindowBottomBottom.right -= Delta;
		}
	}

	if (m_ESizeDirection == SZ_Right ||	m_ESizeDirection == SZ_TopRight ||	m_ESizeDirection == SZ_BottomRight)
	{
		if (WindowCenterWidth + WindowRightWidth > (MinWndSize * 2))
		{
			LONG NewWidth = xPos + m_nBorderOffsetX;

			if (NewWidth < MinWndSize){
				NewWidth = MinWndSize;
			}
			else if ((WindowCenterWidth + WindowRightWidth) - NewWidth < MinWndSize){
				NewWidth = (WindowCenterWidth + WindowRightWidth) - MinWndSize;
			}

			// apply new dimensions
			LONG Delta = WindowBottomWidth - NewWidth;

			rWindowRight.left -= Delta;
			rWindowCenter.right = rWindowBottom.right = rWindowRight.left;

			rWindowBottomTop.right -= Delta;
			rWindowBottomBottom.right -= Delta;
		}
	}
}

void CWindowManager::ResizeFinishRightTop(int xPos, int yPos,
										  LONG WindowCenterWidth,
										  LONG WindowCenterHeight,
										  LONG WindowBottomHeight,
										  LONG WindowRightWidth,
										  LONG WindowLeftWidth,
										  LONG WindowBottomWidth,
										  LONG WindowBottomTopWidth,
										  LONG WindowBottomTopHeight,
										  LONG WindowBottomBottomWidth,
										  LONG WindowBottomBottomHeight,
										  LONG WindowRightTopWidth,
										  LONG WindowRightTopHeight,
										  LONG WindowRightBottomWidth,
										  LONG WindowRightBottomHeight,
										  RECT &rWindowMain,
										  RECT &rWindowMainCl,
										  RECT &rWindowTop,
										  RECT &rWindowLeft,
										  RECT &rWindowRight,
										  RECT &rWindowCenter,
										  RECT &rWindowBottom,
										  RECT &rWindowBottomTop,
										  RECT &rWindowBottomBottom,
										  RECT &rWindowRightTop,
										  RECT &rWindowRightBottom)
{
	if (m_ESizeDirection == SZ_Left || m_ESizeDirection == SZ_TopLeft || m_ESizeDirection == SZ_BottomLeft)
	{	
		if (WindowCenterWidth + WindowRightWidth > (MinWndSize * 2))
		{
			LONG NewWidth = WindowRightWidth - (xPos - m_nBorderOffsetX);

			if (NewWidth < MinWndSize) {
				NewWidth = MinWndSize;
			}
			else if ((WindowCenterWidth + WindowRightWidth) - NewWidth < MinWndSize) {
				NewWidth = (WindowCenterWidth + WindowRightWidth) - MinWndSize;
			}

			// apply new dimensions
			unsigned int Delta = NewWidth - WindowRightWidth;

			rWindowRight.left -= Delta;
			rWindowRightTop.right += Delta;
			rWindowRightBottom.right += Delta;

			rWindowCenter.right = rWindowCenter.right - Delta;
		}
	}

	// Verrical
	if (m_ESizeDirection == SZ_Bottom || m_ESizeDirection == SZ_BottomLeft || m_ESizeDirection == SZ_BottomRight)
	{
		if (WindowRightTopHeight + WindowRightBottomHeight > MinWndSize * 2)
		{
			LONG NewHeight = yPos + m_nBorderOffsetY;

			if (NewHeight < MinWndSize) {
				NewHeight = MinWndSize;
			}
			else if ((WindowRightTopHeight + WindowRightBottomHeight) - NewHeight < MinWndSize) { // bottom clamp
				NewHeight = (WindowRightTopHeight + WindowRightBottomHeight) - MinWndSize;
			}
			int Delta = NewHeight - WindowRightTopHeight;

			rWindowRightTop.bottom += Delta;
			rWindowRightBottom.top = rWindowRightTop.bottom;
			//rWindowRightBottom.bottom -= Delta;
		}
	}
}

void CWindowManager::ResizeFinishRightBottom(int xPos, int yPos,
											 LONG WindowCenterWidth,
											 LONG WindowCenterHeight,
											 LONG WindowBottomHeight,
											 LONG WindowRightWidth,
											 LONG WindowLeftWidth,
											 LONG WindowBottomWidth,
											 LONG WindowBottomTopWidth,
											 LONG WindowBottomTopHeight,
											 LONG WindowBottomBottomWidth,
											 LONG WindowBottomBottomHeight,
											 LONG WindowRightTopWidth,
											 LONG WindowRightTopHeight,
											 LONG WindowRightBottomWidth,
											 LONG WindowRightBottomHeight,
											 RECT &rWindowMain,
											 RECT &rWindowMainCl,
											 RECT &rWindowTop,
											 RECT &rWindowLeft,
											 RECT &rWindowRight,
											 RECT &rWindowCenter,
											 RECT &rWindowBottom,
											 RECT &rWindowBottomTop,
											 RECT &rWindowBottomBottom,
											 RECT &rWindowRightTop,
											 RECT &rWindowRightBottom)
{
	if (m_ESizeDirection == SZ_Left || m_ESizeDirection == SZ_TopLeft || m_ESizeDirection == SZ_BottomLeft)
	{	
		if (WindowCenterWidth + WindowRightWidth > (MinWndSize * 2))
		{
			LONG NewWidth = WindowRightWidth - (xPos - m_nBorderOffsetX);

			if (NewWidth < MinWndSize) {
				NewWidth = MinWndSize;
			}
			else if ((WindowCenterWidth + WindowRightWidth) - NewWidth < MinWndSize) 
			{
				NewWidth = (WindowCenterWidth + WindowRightWidth) - MinWndSize;
			}

			// apply new dimensions
			unsigned int Delta = NewWidth - WindowRightWidth;

			rWindowRight.left -= Delta;
			rWindowRightTop.right += Delta;
			rWindowRightBottom.right += Delta;

			rWindowCenter.right = rWindowCenter.right - Delta;
		}
	}

	// Verrical
	if (m_ESizeDirection == SZ_Top || m_ESizeDirection == SZ_TopLeft ||	m_ESizeDirection == SZ_TopRight)
	{
		if (WindowRightTopHeight + WindowRightBottomHeight > MinWndSize * 2)
		{
			LONG NewHeight = WindowRightBottomHeight - (yPos - m_nBorderOffsetY);

			if (NewHeight < MinWndSize) {
				NewHeight = MinWndSize;
			}
			else if ((WindowRightTopHeight + WindowRightBottomHeight) - NewHeight < MinWndSize)
			{ // bottom clamp
				NewHeight = (WindowRightTopHeight + WindowRightBottomHeight) - MinWndSize;
			}
			// apply new dimensions
			int Delta = WindowRightBottomHeight - NewHeight;

			rWindowRightTop.bottom += Delta;
			rWindowRightBottom.top = rWindowRightTop.bottom;
			//rWindowRightBottom.bottom -= Del ta;
		}
	}
}

//----------------------------------------------------------------------------------------------
void CWindowManager::OnWndMaximized(int width, int height)
{
	RECT rWindowMain, rWindowTop, rWindowLeft,
		 rWindowRight, rWindowBottom, rWindowCenter,
		 rWindowBottomTop, rWindowBottomBottom,
		 rWindowRightTop, rWindowRightBottom;

	// retrieve size data
	GetWindowRect(m_hWnd, &rWindowMain);
	GetWindowRect(m_pCtrlWindowTop->GetHWND(), &rWindowTop);
	GetWindowRect(m_pCtrlWindowLeft->GetHWND(), &rWindowLeft);
	GetWindowRect(m_pCtrlWindowRight->GetHWND(), &rWindowRight);
	GetWindowRect(m_pCtrlWindow->GetHWND(), &rWindowCenter);
	GetWindowRect(m_pCtrlWindowBootom->GetHWND(), &rWindowBottom);
	GetWindowRect(m_pCtrlWindowBootomTop->GetHWND(), &rWindowBottomTop);
	GetWindowRect(m_pCtrlWindowBootomBottom->GetHWND(), &rWindowBottomBottom);
	GetWindowRect(m_pCtrlWindowRightTop->GetHWND(), &rWindowRightTop);
	GetWindowRect(m_pCtrlWindowRightBottom->GetHWND(), &rWindowRightBottom);

	// calculate current size
	LONG WindowTopHeight = rWindowTop.bottom - rWindowTop.top;
	LONG WindowCenterWidth = rWindowCenter.right - rWindowCenter.left;
	LONG WindowCenterHeight = rWindowCenter.bottom - rWindowCenter.top;
	LONG WindowRightWidth = rWindowRight.right - rWindowRight.left;
	LONG WindowLeftWidth = rWindowLeft.right - rWindowLeft.left;
	LONG WindowBottomHeight = rWindowBottom.bottom - rWindowBottom.top;
	LONG WindowBottomWidth = rWindowBottom.right - rWindowBottom.left;
	LONG WindowRightTopHeight = rWindowRightTop.bottom - rWindowRightTop.top;
	LONG WindowRightBottomHeight = rWindowRightBottom.bottom - rWindowRightBottom.top;
	
	// calculate new size
	LONG NewBottomWidth = rWindowBottom.right - rWindowBottom.left;
	LONG NewBottomHeight = rWindowBottom.bottom - rWindowBottom.top;

	LONG NewLeftWidth = rWindowLeft.right - rWindowLeft.left;
	LONG NewLeftHeight = height - (WindowTopHeight + NewBottomHeight);
	
	LONG NewRightWidth = rWindowRight.right - rWindowRight.left;
	LONG NewRightHeight = height - (WindowTopHeight + NewBottomHeight);

	LONG NewRightTopHeight = height - WindowTopHeight;

	LONG NewCentralWidth = rWindowCenter.right - rWindowCenter.left;
	LONG NewCentralHeight = rWindowCenter.bottom - rWindowCenter.top;

	LONG NewBottomTopWidth = rWindowBottomTop.right - rWindowBottomTop.left;
	LONG NewBottomTopHeight = rWindowBottomTop.bottom - rWindowBottomTop.top;
	
	LONG NewBottomBottomWidth = rWindowBottomTop.right - rWindowBottomTop.left;
	LONG NewBottomBottomHeight = rWindowBottomBottom.bottom - rWindowBottomBottom.top;

	if (width > MinWndSize * 3) // check minimum width
	{
		NewCentralWidth = (width - (WindowLeftWidth + WindowRightWidth));

		if (NewCentralWidth < MinWndSize)
		{
			NewCentralWidth = 
			WindowRightWidth = MinWndSize;

			NewLeftWidth = width - MinWndSize * 2;
		}
	}
	
	if (height > WindowTopHeight + MinWndSize * 2) // check minimum height
	{
		NewCentralHeight = height - (WindowTopHeight + WindowBottomHeight);
		if (NewCentralHeight < MinWndSize)
		{
			WindowBottomHeight = height - WindowTopHeight - MinWndSize;
			NewCentralHeight = MinWndSize;
		}
	}

	MoveWindow(m_pCtrlWindowTop->GetHWND(), 0, 0, width, WindowTopHeight, TRUE);
	MoveWindow(m_pCtrlWindowLeft->GetHWND(), 0, WindowTopHeight, NewLeftWidth, NewLeftHeight,	TRUE);
	MoveWindow(m_pCtrlWindow->GetHWND(), NewLeftWidth, WindowTopHeight, NewCentralWidth, NewCentralHeight, TRUE);
	MoveWindow(m_pCtrlWindowRight->GetHWND(), width - WindowRightWidth, WindowTopHeight, NewRightWidth, NewRightHeight, TRUE);
	MoveWindow(m_pCtrlWindowRightTop->GetHWND(), 0, 0, NewRightWidth, NewRightHeight - WindowRightBottomHeight, TRUE);
	MoveWindow(m_pCtrlWindowRightBottom->GetHWND(), 0, NewRightHeight - WindowRightBottomHeight, NewRightWidth, WindowRightBottomHeight, TRUE);
	MoveWindow(m_pCtrlWindowBootom->GetHWND(), 0, WindowTopHeight + NewCentralHeight, NewBottomWidth, NewBottomHeight,	TRUE);
	MoveWindow(m_pCtrlWindowBootomTop->GetHWND(), 0, 0, NewBottomTopWidth, NewBottomTopHeight, TRUE);
	MoveWindow(m_pCtrlWindowBootomBottom->GetHWND(), 0, NewBottomTopHeight, NewBottomBottomWidth, NewBottomBottomHeight, TRUE);
}

//----------------------------------------------------------------------------------------------
void CWindowManager::OnDrawSplitter(HWND hWnd, int xPos, int yPos)
{
	RECT focusrect;
	RECT rect;
	HDC  hdc;
		
	//m_pCtrlWindowRightTop->Redraw();
	//m_pCtrlWindowRightBottom->Redraw();

	int FrameSize = GetSystemMetrics(SM_CYSIZEFRAME);
	int HFrameSize = (int)((float)FrameSize * 0.5f);

	hdc = GetDC(hWnd);
	GetClientRect(hWnd, &rect);

	int Width = rect.right - rect.left;
	int Height = rect.bottom - rect.top;

	SetRect(&focusrect, rect.left + FrameSize, rect.top + HFrameSize, Width - HFrameSize, Height - HFrameSize);
	DrawFocusRect(hdc, &focusrect);
	
	if (m_ESizeDirection == SZ_Left || m_ESizeDirection == SZ_Right)
	{
		SetRect(&focusrect, xPos - HFrameSize, rect.top + HFrameSize, xPos + HFrameSize, Height - HFrameSize);
		DrawFocusRect(hdc, &focusrect);
	}
	else if (m_ESizeDirection == SZ_Bottom || m_ESizeDirection == SZ_Top)
	{
		SetRect(&focusrect, rect.left + HFrameSize, yPos - HFrameSize, Width - HFrameSize, yPos + HFrameSize);
		DrawFocusRect(hdc, &focusrect);
	}
	
	ReleaseDC(hWnd, hdc);
}

//----------------------------------------------------------------------------------------------
void CWindowManager::SetCursorDir(ECursorDir Dir)
{
	switch (Dir)
	{
	case CD_EW:
		SetCursor(m_hcSizeEW);
		break;
	case CD_NS:
		SetCursor(m_hcSizeNS);
		break;
	case CD_NWSE:
		SetCursor(m_hcSizeNWSE);
		break;
	case CD_NESW:
		SetCursor(m_hcSizeNESW);
		break;
	default:
		SetCursor(0);
		break;
	};
}

//----------------------------------------------------------------------------------------------
void CWindowManager::AdjustRightFrameHeight(LONG Delta, 
											LONG WindowRightTopHeight,
											LONG WindowRightBottomHeight,
											RECT &rWindowRightTop,
											RECT &rWindowRightBottom)
{
	RECT rWindowRight;

	GetWindowRect(m_pCtrlWindowRight->GetHWND(), &rWindowRight);

	LONG WindowRightHeight = rWindowRight.bottom - rWindowRight.top;

	// actual size
	LONG WindowCenterWidth = rWindowCenter.right - rWindowCenter.left;
	LONG TotalHeight = WindowRightHeight + Delta;

	if (WindowRightBottomHeight > MinWndSize)
	{
		rWindowRightBottom.bottom += Delta;

		if (rWindowRightBottom.bottom - rWindowRightBottom.top < MinWndSize)
		{
			rWindowRightBottom.bottom = rWindowRightBottom.top + MinWndSize;
		}
	}
	
	rWindowRightTop.bottom = rWindowRightTop.top + TotalHeight - (rWindowRightBottom.bottom - rWindowRightBottom.top);

	if (rWindowRightTop.bottom - rWindowRightTop.top < MinWndSize)
	{
		rWindowRightTop.bottom = rWindowRightTop.top + MinWndSize;
	}
}

*/