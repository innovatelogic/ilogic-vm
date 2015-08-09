/*#include "LauncherStdAfx.h"

const int SPLITTER_BAR_WIDTH = 3;
const int WIDTH_ADJUST = 2;

//----------------------------------------------------------------------------------------------
namespace NMWin32ActionPlayEditor
{
	const CWindowManager	*pWindowManager = NULL;

	std::vector<HTREEITEM>	HTreeClearList;
	HTREEITEM				HTreeSelected = NULL;
	HTREEITEM				HTreeRename	= NULL;

	HWND			hwndDialog = NULL;
	HWND			hWndTop	= NULL;
	HWND			hWndBottom = NULL;
	HTREEITEM		hitTarget = NULL;
	HCURSOR			hcSizeEW = NULL;

	CActor* 		SelectedObject = NULL;
	POINT			PickPoint;

	CWin32ActionPlayEditor::ContextMenuFunction		pfnContextMenu;
	CWin32ActionPlayEditor::ContextMenuProcessor	pfnContextMenuProcessor;
	CWin32ActionPlayEditor::GetResourceIconIndex	pfnGetResourceIconIndex;

	CriticalSection InvokeActorCS;

	CWin32ActionPlayEditor::MapObjectProperty	ObjectProperties;

	Win32PropertyGrid	*pPropGrid;
	int					nTopHeight = 400;
	int					nTopWndHeight = 400;

	Matrix				ViewMatrixEditor;

	bool bYSizing = false;

	bool bMBtnPressed = false;
	Vector2f MousePos;

	SocketAction	*PicketSocket = 0;
	ActionBase		*PickedAction = 0;
	Vector2f		PickDelta;

}using namespace NMWin32ActionPlayEditor;

LRESULT CALLBACK ProcWndTop(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProcWndBottom(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

//----------------------------------------------------------------------------------------------
LRESULT CALLBACK ProcWnd(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	switch (messg) 
	{
	case WM_CREATE:
		{
			HINSTANCE hInst = ((LPCREATESTRUCT) lParam)->hInstance;

			RECT rect;
			GetClientRect(hWnd, &rect);

			int Width = rect.right - rect.left;
			int Height = rect.bottom - rect.top;

			CLIENTCREATESTRUCT MDIClientCreateStruct; // Structure to be used for MDI client area

			MDIClientCreateStruct.hWindowMenu	= NULL;
			MDIClientCreateStruct.idFirstChild	= IDM_FIRSTCHILD;

			hWndTop = CreateWindowEx(
				WS_EX_CLIENTEDGE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
				TEXT("MDIClient"),
				NULL,  
				WS_CHILD | WS_VISIBLE | SS_SUNKEN, 
				rect.left, rect.top,
				Width, nTopHeight, 
				hWnd,
				NULL,
				hInst,
				(void*) &MDIClientCreateStruct);

			hWndBottom = CreateWindowEx( 
				WS_EX_CLIENTEDGE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
				TEXT("MDIClient"), 
				NULL,  
				WS_CHILD | WS_VISIBLE | SS_SUNKEN, 
				rect.left, nTopHeight + SPLITTER_BAR_WIDTH,
				Width, Height - (nTopHeight + SPLITTER_BAR_WIDTH),
				hWnd,
				NULL,
				hInst,
				(void*) &MDIClientCreateStruct);

			pPropGrid = new Win32PropertyGrid(hWndBottom, pWindowManager);

			hcSizeEW = LoadCursor(NULL, IDC_SIZENS);

			SetWindowLong(hWndTop, GWL_WNDPROC, (LONG)ProcWndTop);
			SetWindowLong(hWndBottom, GWL_WNDPROC, (LONG)ProcWndBottom);
		}break;

	case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);

			int Width = rect.right - rect.left;
			int Height = rect.bottom - rect.top;

			MoveWindow(hWndTop, rect.left, rect.top, Width, nTopWndHeight, TRUE);
			MoveWindow(hWndBottom, rect.left, nTopWndHeight + SPLITTER_BAR_WIDTH, Width, Height - (nTopWndHeight + SPLITTER_BAR_WIDTH), TRUE);
		}break;

	case WM_LBUTTONDOWN: // context menu
		{
			int xPos = (int)LOWORD(lParam);
			int yPos = (int)HIWORD(lParam);

			bYSizing = (yPos > nTopWndHeight && yPos < nTopWndHeight + SPLITTER_BAR_WIDTH);

			// If the mouse is over the splitter then set mouse cursor 
			// image to IDC_SIZEWE which helps the user to drag the window
			if (bYSizing)
			{
				// Api to capture mouse input
				SetCapture(hWnd);
				SetCursor(hcSizeEW);
			}
		}break;

	case WM_LBUTTONUP:
		{
			if(bYSizing)
			{
				RECT    focusrect;
				HDC     hdc;
				RECT	rect;

				// Releases the captured mouse input
				ReleaseCapture();

				// Get the main window dc to draw a focus rectangle
				hdc = GetDC(hWnd);
				GetClientRect(hWnd, &rect);

				if (bYSizing)
				{
					int Width = rect.right - rect.left;
					int Height = rect.bottom - rect.top;

					SetRect(&focusrect, rect.left,	rect.top, Width, Height);

					// Call api to vanish the dragging rectangle 
					DrawFocusRect(hdc, &focusrect);

					bYSizing = FALSE;
				}
				// Release the dc once done 
				ReleaseDC(hWnd, hdc);
			}
			// Post a WM_SIZE message to redraw the windows
			PostMessage(hWnd, WM_SIZE, 0, 0);
		}break;

	case WM_MOUSEMOVE:
		{
			int   xPos = (int)LOWORD(lParam);
			int   yPos = (int)HIWORD(lParam);

			// Checks if the left button is pressed during dragging the splitter
			if (wParam == MK_LBUTTON)   
			{
				// If the window is d`agged using the splitter, get the
				// cursors current postion and draws a focus rectangle 
				if (bYSizing)
				{
					RECT    focusrect;
					RECT    rect;
					HDC     hdc;

					hdc = GetDC(hWnd);
					GetClientRect(hWnd, &rect);

					int Width = rect.right - rect.left;
					int Height = rect.bottom - rect.top;

					SetRect(&focusrect, rect.left, rect.top, Width, Height);

					// Get the size of the left window to increase
					nTopWndHeight = yPos;

					ReleaseDC(hWnd, hdc);
				}
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			POINT pt;
			::GetCursorPos(&pt);
			::ScreenToClient(hWndTop,&pt);
			
			float ds = GET_WHEEL_DELTA_WPARAM(wParam) < 0 ? 0.95f : 1.15f;

			ViewMatrixEditor.t += (Vector((float)pt.x, (float)pt.y, 0.f) - ViewMatrixEditor.t) * (1.f - ds);

			scale(ViewMatrixEditor, ds, ds, ds);

			InvalidateRect(hWndTop, NULL, TRUE);
		}break;

	case WM_COMMAND:
		{
			if (!PicketSocket){
				CWin32ActionPlayEditor::ContextMenuProcessorAction(hWnd, messg, wParam, lParam);
			}
			else{
				CWin32ActionPlayEditor::ContextMenuProcessorLink(hWnd, messg, wParam, lParam);
			}
			return 0;
		}break;

	case WM_NOTIFY:
		{
			if (((LPNMHDR)lParam)->code == NM_CLICK)
			{
				if (((LPNMHDR)lParam)->hwndFrom == pPropGrid->GetHWNDTab())
				{
					pPropGrid->ProcessTabCtrl(hWnd, messg, wParam, lParam);
				}
			}
		}break;

	case WM_CLOSE:
	case WM_QUIT:
		{
			CWin32ActionPlayEditor::ShowDialogWindow(false);
			pWindowManager->UpdateFlagsState();
			return 0;
		}break;
	}
	return DefWindowProc(hWnd, messg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
LRESULT CALLBACK ProcWndTop(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	switch (messg) 
	{
	case WM_CREATE:
		{
		}break;

	case WM_SIZE:
		{
		}break;

	case WM_PAINT:
		{	
			PAINTSTRUCT ps;
			HDC         hdc;

			hdc = BeginPaint(hWnd, &ps);
			InvokeActorCS.enter();

			if (SelectedObject){
				pWindowManager->GetCoreSDK()->GetScriptDriver()->Render(SelectedObject, hdc, ViewMatrixEditor);
			}

			InvokeActorCS.leave();
			EndPaint(hWnd, &ps);
		}break;

	case WM_MBUTTONDOWN:
		{
			MousePos = Vector2f((float)LOWORD(lParam), (float)HIWORD(lParam));
			bMBtnPressed = true;
			SetCapture(hWnd);
		}break;

	case WM_MBUTTONUP:
		{
			bMBtnPressed = false;
			ReleaseCapture();
		}break;

	case WM_MOUSEMOVE:
		{
			int xPos = (int)LOWORD(lParam);
			int yPos = (int)HIWORD(lParam);

			Vector2f NewMousePos = Vector2f((float)LOWORD(lParam), (float)HIWORD(lParam));

			if (wParam == MK_LBUTTON && PickedAction)
			{
				Vector2f Point((float)NewMousePos.x, NewMousePos.y);

				Matrix IViewMatrix;
				Vector2f P = transform_coord(Point - ViewMatrixEditor.t2, invert(IViewMatrix, ViewMatrixEditor));

				//PickedAction->SetPosition2f(P - PickDelta);
				InvalidateRect(hWndTop, NULL, TRUE);
			
			} else if (wParam == MK_MBUTTON && bMBtnPressed)
			{
				ViewMatrixEditor.t.x -= MousePos.x - NewMousePos.x;
				ViewMatrixEditor.t.y -= MousePos.y - NewMousePos.y;

				InvalidateRect(hWndTop, NULL, TRUE);
			}

			MousePos = NewMousePos;
		}
		break;

	case WM_LBUTTONDOWN: // context menu
		{
			POINT pt;
			if (::GetCursorPos(&pt))
			{
				TVHITTESTINFO tvht;
				tvht.pt = pt;
				ScreenToClient(hWnd, &tvht.pt);

				Vector2f Point((float)tvht.pt.x, (float)tvht.pt.y);

				Matrix IViewMatrix;
				Vector2f P = transform_coord(Point - ViewMatrixEditor.t2, invert(IViewMatrix, ViewMatrixEditor));

				if (SelectedObject)
				{
					if (SocketAction * Socket = pWindowManager->GetCoreSDK()->GetScriptDriver()->ProcessClickSocket(SelectedObject, P, true))
					{
						PicketSocket = Socket;
					}
					if (SocketAction * Socket = pWindowManager->GetCoreSDK()->GetScriptDriver()->ProcessClickSocket(SelectedObject, P))
					{
						PicketSocket = Socket;
					}
					else if (const ActionBase * Action = pWindowManager->GetCoreSDK()->GetScriptDriver()->ProcessClickAction(SelectedObject, P))
					{
						PickDelta = P - Action->GetPosition2f();
						PickedAction = const_cast<ActionBase*>(Action);
					}
				}
			}
		}break;


	case WM_LBUTTONUP: // context menu
		{
			POINT pt;
			if (::GetCursorPos(&pt))
			{
				TVHITTESTINFO tvht;
				tvht.pt = pt;
				ScreenToClient(hWnd, &tvht.pt);

				Vector2f Point((float)tvht.pt.x, (float)tvht.pt.y);

				Matrix IViewMatrix;
				Vector2f P = transform_coord(Point - ViewMatrixEditor.t2, invert(IViewMatrix, ViewMatrixEditor));

				if (SelectedObject)
				{
					if (PicketSocket)
					{
						if (SocketAction * Socket = pWindowManager->GetCoreSDK()->GetScriptDriver()->ProcessClickSocket(SelectedObject, P))
						{
							if (PicketSocket->Dir != Socket->Dir)
							{
								PicketSocket->LinkSocket = Socket;
								InvalidateRect(hWndTop, NULL, TRUE);
							}
						}

						PicketSocket = NULL;
					}
					else if (PickedAction)
					{
						if (const ActionBase * Action = pWindowManager->GetCoreSDK()->GetScriptDriver()->ProcessClickAction(SelectedObject, P))
						{
							int k = 0;
						}

						PickedAction = NULL;
					}
				}
			}
		}break;

	case WM_RBUTTONDOWN: // context menu
		{
			POINT pt;
			if (::GetCursorPos(&pt)) // address of structure for cursor position
			{
				TVHITTESTINFO tvht;
				tvht.pt = pt;
				ScreenToClient(hWnd, &tvht.pt);

				Vector2f Point((float)tvht.pt.x, (float)tvht.pt.y);

				Matrix IViewMatrix;
				Vector2f P = transform_coord(Point - ViewMatrixEditor.t2, invert(IViewMatrix, ViewMatrixEditor));

				PickPoint.x = (int)P.x;
				PickPoint.y = (int)P.y;

				if (SelectedObject)
				{
					if (SocketAction * Socket = pWindowManager->GetCoreSDK()->GetScriptDriver()->ProcessClickSocket(SelectedObject, P, true))
					{
						PicketSocket = Socket;
						CWin32ActionPlayEditor::HandleLinkContextMenu(hwndDialog, pt);
					}
					else
					{
						PicketSocket = NULL;
						CWin32ActionPlayEditor::HandlePopupContextMenu(hwndDialog, pt);
					}
				}
			}
		}break;

	case WM_COMMAND:
		{
		}break;

	default:
		break;
	};
	return DefWindowProc(hWnd, messg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
LRESULT CALLBACK ProcWndBottom(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	switch (messg) 
	{
	case WM_CREATE:
		{
		}break;

	case WM_SIZE:
		{
			pPropGrid->Size(hWnd, messg, wParam, lParam);
		}break;

	case WM_PAINT:
		{	
		}break;

	case WM_LBUTTONDOWN: // context menu
		{
		}break;

	case WM_RBUTTONDOWN: // context menu
		{
		}break;

	case WM_COMMAND:
		{
		}break;

	case WM_NOTIFY:
		{
			{
				long lResult = pPropGrid->HandleNotify(hWnd, (int)wParam, (LPNMHDR)lParam); // handle customdraw
				if (lResult > 0)
				{
					SetWindowLong(hWnd, DWL_MSGRESULT, lResult); 
					return lResult;
				}
			}

			switch (((LPNMHDR)lParam)->code)
			{
				// process fill list
			case LVN_GETDISPINFO:
				{
					if (((LPNMHDR)lParam)->hwndFrom == pPropGrid->GetHWNDListProperty())
					{
						pPropGrid->GETDISPINFO_FillList(lParam);
					}
					else if (((LPNMHDR)lParam)->hwndFrom == pPropGrid->GetHWNDCombobox())
					{
						pPropGrid->GETDISPINFO_FillComboList(lParam);
					}
				}break;

				case NM_CLICK:
					{
						if (((LPNMHDR)lParam)->hwndFrom == pPropGrid->GetHWNDTab())
						{
							pPropGrid->ProcessTabCtrl(hWnd, messg, wParam, lParam);
						}
						else if (((LPNMHDR)lParam)->hwndFrom == pPropGrid->GetHWNDListProperty())
						{
							pPropGrid->ClickListProperties((LPNMLISTVIEW)lParam);
						}
					}break;

			default:
				break;
			};
		}break;

	case WM_USER_DELETE_ALL:
		{
			pPropGrid->PropertyCS.enter();

			pPropGrid->ClearTabPages();
			pPropGrid->ClearListProperties();

			pPropGrid->PropertyCS.leave();
		}break;

	case WM_USER_FILL_PROPS:
		{
			CWin32ActionPlayEditor::FillPropertyTabs();
			pPropGrid->FillListProperties();
		}break;

	case WM_USER_UPDATE_PROPS:
		{
			pPropGrid->UpdatePreview();
		}break;

	default:
		break;
	};
	return DefWindowProc(hWnd, messg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
void CWin32ActionPlayEditor::Initialize(HWND hWndParent,
										ContextMenuFunction pfMenu,
										ContextMenuProcessor pfnMenuProcessor,
										GetResourceIconIndex pfnGetResourceIconIdx,
										HIMAGELIST hImageList,
										const CWindowManager *WMgr)
{
	pWindowManager	= WMgr;

	WNDCLASSEX m_wcAppSplash;

	m_wcAppSplash.cbSize        = sizeof(m_wcAppSplash);
	m_wcAppSplash.style         = CS_OWNDC|CS_BYTEALIGNWINDOW|CS_BYTEALIGNCLIENT;
	m_wcAppSplash.lpfnWndProc   = ProcWnd;
	m_wcAppSplash.cbClsExtra    = 0;
	m_wcAppSplash.cbWndExtra    = 0;
	m_wcAppSplash.hInstance     = GetModuleHandle(NULL);
	m_wcAppSplash.hIcon         = NULL;
	m_wcAppSplash.hCursor       = LoadCursor(0, IDC_ARROW);
	m_wcAppSplash.hbrBackground =  (HBRUSH)GetStockObject(GRAY_BRUSH);
	m_wcAppSplash.lpszMenuName  = NULL;
	m_wcAppSplash.lpszClassName = TEXT("ActionPlayEditor");
	m_wcAppSplash.hIconSm       = NULL;

	if (!RegisterClassEx(&m_wcAppSplash)){
		return;
	}

	HWND hWnd = CreateWindow(TEXT("ActionPlayEditor"),
		TEXT("ActionPlayEditor"),
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		1024,
		768,
		hWndParent, 
		NULL,
		GetModuleHandle(NULL),
		NULL);

	if (hWnd != NULL)
	{
		ShowWindow(hWnd, SW_SHOWNORMAL);
		hwndDialog				= hWnd;
		pfnContextMenu			= pfMenu;
		pfnContextMenuProcessor = pfnMenuProcessor;
		pfnGetResourceIconIndex = pfnGetResourceIconIdx;

		SendDlgItemMessage(hwndDialog, IDC_TREE_CAMERAS, TVM_SETIMAGELIST, 0, (LPARAM)hImageList);
	}
}

//----------------------------------------------------------------------------------------------
void CWin32ActionPlayEditor::ShowDialogWindow(bool bShow)
{
	if (hwndDialog){
		ShowWindow(hwndDialog, (bShow == true) ? SW_SHOW : SW_HIDE);
	}
}

//----------------------------------------------------------------------------------------------
bool CWin32ActionPlayEditor::IsVisible()
{
	if (hwndDialog){
		return IsWindowVisible(hwndDialog) ? true : false;
	}
	return false;
}

//----------------------------------------------------------------------------------------------
void CWin32ActionPlayEditor::HandlePopupContextMenu(HWND hwnd, POINT pt)
{
	HMENU hmenu = LoadMenu((HINSTANCE)::GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU_ACTION));
	if (hmenu == NULL) return;

	HMENU subhmenu = GetSubMenu(hmenu, 0);
	HMENU addhmenu = GetSubMenu(subhmenu, 0);
	HMENU addActionhmenu = CreatePopupMenu();
	HMENU addObjecthmenu = CreatePopupMenu();

	InsertMenu(subhmenu, 0, MF_STRING | MF_POPUP, (UINT) addActionhmenu, TEXT("Add Action")); 
	InsertMenu(subhmenu, 0, MF_STRING | MF_POPUP, (UINT) addObjecthmenu, TEXT("Add Object")); 

	// adds creation
	NObjectFactory::TClassFactory * classFactory = GetClassFactoryA();
	assert(classFactory);

	ClassNode * InheritanceActions = classFactory->ClassTree.Find("ActionBase");

	if (InheritanceActions)
	{
		int NumChilds = 0;
		for (std::vector<ClassNode*>::iterator Iter = InheritanceActions->Childs.begin(); Iter != InheritanceActions->Childs.end(); ++Iter, ++NumChilds)
		{
			MENUITEMINFO iInfo;
			memset(&iInfo, 0, sizeof(MENUITEMINFO));
			iInfo.cbSize	= sizeof(MENUITEMINFO);
			iInfo.hSubMenu  = NULL;
			iInfo.wID		= (UINT)NumChilds + 5000;
			iInfo.fMask		= MIIM_ID | MIIM_STRING;
			iInfo.fType		= MFT_STRING;

			wchar_t wbuf[100] = {0};
			MultiByteToWideChar(CP_ACP, 0, (*Iter)->Name.c_str(), INDEX_NONE, wbuf, 100);

			iInfo.dwTypeData = wbuf;
			iInfo.cch = (UINT)strlen((*Iter)->Name.c_str());

			InsertMenuItem(addActionhmenu, NumChilds + 5000, TRUE, (LPCMENUITEMINFO) &iInfo);
		}
	}

	HMENU hmenuTrackPopup = GetSubMenu(hmenu, 0);
	TrackPopupMenuEx(hmenuTrackPopup, 0, pt.x, pt.y, hwnd, NULL);

	DestroyMenu(hmenu);
}

//----------------------------------------------------------------------------------------------
bool CWin32ActionPlayEditor::ContextMenuProcessorAction(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	bool bResult = false;

	// process commands
	if (((INT)(wParam) >= 5000 && (INT)(wParam) < 7000) && SelectedObject)
	{
		INT CheckedIndex = (INT)(wParam) - 5000;

		// adds creation
		NObjectFactory::TClassFactory * classFactory = GetClassFactoryA();
		assert(classFactory);

		ClassNode * InheritanceActions = classFactory->ClassTree.Find("ActionBase");

		if (InheritanceActions)
		{
			int NumChilds = 0;
			for (std::vector<ClassNode*>::iterator Iter = InheritanceActions->Childs.begin(); Iter != InheritanceActions->Childs.end(); ++Iter, ++NumChilds)
			{
				if (CheckedIndex == NumChilds)
				{
					ActionBase* ParentAction = pWindowManager->GetCoreSDK()->GetScriptDriver()->GetActionByActor(SelectedObject);

					if (ParentAction)
					{
						ActionBase* NewActionObject = static_cast<ActionBase*>(GENERATE_OBJECT_SDK((*Iter)->Name.c_str(), (*Iter)->Name.c_str(), ParentAction));
						ParentAction->AddChildNode(NewActionObject);

						NewActionObject->SetOwnerActor(SelectedObject);
						NewActionObject->SetAppMain(pWindowManager->GetCoreSDK());

						//NewActionObject->SetPosition2f(Vector2f((float)PickPoint.x, (float)PickPoint.y));
						//pWindowManager->GetCoreSDK()->GetScriptDriver()->RegisterAction(NewActionObject);

						NewActionObject->Initialize();
					}

					InvalidateRect(hwndDialog, NULL, TRUE);
					break;
				}
			}
		}
	}

	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CWin32ActionPlayEditor::ContextMenuProcessorLink(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	bool bResult = false;

	// process commands
	if ((INT)(wParam) == ID_UNLINK && SelectedObject && PicketSocket)
	{
		PicketSocket->LinkSocket = NULL;
		PicketSocket = NULL;
		InvalidateRect(hWndTop, NULL, TRUE);
	}

	return bResult;
}

//----------------------------------------------------------------------------------------------
void CWin32ActionPlayEditor::HandleLinkContextMenu(HWND hwnd, POINT pt)
{
	HMENU hmenu = LoadMenu((HINSTANCE)::GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU_LINK));
	if (hmenu == NULL) return;

	HMENU hmenuTrackPopup = GetSubMenu(hmenu, 0);
	TrackPopupMenuEx(hmenuTrackPopup, 0, pt.x, pt.y, hwnd, NULL);

	DestroyMenu(hmenu);
}

//----------------------------------------------------------------------------------------------
bool CWin32ActionPlayEditor::FillPropertyTabs()
{
 	bool bResult = false;

	InvokeActorCS.enter();

	bResult = SelectedObject ? AddActorProperties(SelectedObject) : false;

	InvokeActorCS.leave();

 	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CWin32ActionPlayEditor::AddActorProperties(CActor * Actor)
{
	if (Actor != NULL)
	{
		pPropGrid->ClearTabPages();
		pPropGrid->FillPropertyData(Actor);
	}
	return true;
}

//----------------------------------------------------------------------------------------------
void CWin32ActionPlayEditor::Update(const CActor * Sender, ESystemEventID EventId)
{
	if (hWndBottom && Sender)
	{
		InvokeActorCS.enter();

		bool bReleasedSibling = false;

		CActor * actor = const_cast<CActor*>(Sender);
		while (actor)
		{
			if (actor == SelectedObject){
				bReleasedSibling = true;
				break;
			}
			actor = actor->GetParent();
		}

		if (EventId == Event_PreRelease) // release selected
		{	
			if (SelectedObject == Sender)
			{
				SelectedObject = NULL;
				::PostMessage(hWndBottom, WM_USER_DELETE_ALL, 0, 0);
			}
			else if (bReleasedSibling)
			{
				::PostMessage(hWndBottom, WM_USER_DELETE_ALL, 0, 0);
				::PostMessage(hWndBottom, WM_USER_FILL_PROPS, 0, 0);
			}
		}
		else
		{
			if (EventId == Event_OnSelected)
			{
				SelectedObject = const_cast<CActor*>(Sender);

				::PostMessage(hWndBottom, WM_USER_DELETE_ALL, 0, 0);
				::PostMessage(hWndBottom, WM_USER_FILL_PROPS, 0, 0);

				::PostMessage(hWndTop, WM_PAINT, 0, 0);
				InvalidateRgn(hWndTop, NULL, TRUE);
			}
			else if (EventId == Event_PropertyChanged)
			{
				::PostMessage(hWndBottom, WM_USER_UPDATE_PROPS, 0, 0);
			}
		}		
		InvokeActorCS.leave();
	}
}

//----------------------------------------------------------------------------------------------
void CWin32ActionPlayEditor::Release()
{
	delete pPropGrid;
}*/