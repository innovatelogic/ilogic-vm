#pragma once

#include "scene_editor_main.h"
#include "MSelectTree.h"

#define WM_USER_INSERTOBJECT				WM_APP+1
#define WM_USER_REMOVEOBJECT_BRWSR			WM_APP+2
#define WM_USER_SELECTOBJECT_BRWSR			WM_APP+3
#define WM_USER_RENAMEOBJECT_BRWSR			WM_APP+4
#define WM_USER_MOVEOBJECT					WM_APP+5
#define WM_USER_REARRANGE					WM_APP+6

template<class T_CLASS>
class Win32ObjectBrowserWidget
{
	typedef typename void (*pContextMenuFunction) (HWND hwnd, POINT pt, T_CLASS *pActor);
	typedef typename bool (*pContextMenuProcessor)(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	typedef typename int  (*pGetResourceIconIndex)(const std::string &TypeName);

public:
	typedef typename std::map<const T_CLASS*, HTREEITEM>  TTreeMapActor;
	typedef typename TTreeMapActor::iterator			  TTreeMapActorIterator;
	typedef typename TTreeMapActor::const_iterator		  TTreeMapActorConstIterator;

	typedef int (*CALLBACK_FN) (const T_CLASS *pSender);

	struct SwapHTree
	{
		const T_CLASS *ActorMove;
		bool bUp;
	};

public:

	HWND GetHWndTree() const { return m_hwndTree; }

    Win32ObjectBrowserWidget(HWND hWndParent,
        pContextMenuFunction pfMenu,
        pContextMenuProcessor pfnMenuProcessor,
        pGetResourceIconIndex pfnGetResourceIconIndex,
        CALLBACK_FN pfnInvokeObject,
        CALLBACK_FN pfnDirectInvokeObject,
        CALLBACK_FN pfnClearObject,
        CALLBACK_FN pfnDirectClearObject,
        HIMAGELIST hImageList,
        SRenderContext *pRenderContext = 0);

    ~Win32ObjectBrowserWidget();

    void SetEditor(editors::TIEditor editor);

	void SetRenderContext(SRenderContext *pContext) { m_pRenderContext = pContext; }

    int InvokeActor(const T_CLASS *pSender);

    void Update(const T_CLASS *pSender, ESystemEventID EventId);

    bool ClearActor(const T_CLASS *pSender);

    int SelectActor(const T_CLASS * Sender);

    int RenameActor(const T_CLASS * Sender);

    int MoveActor(const T_CLASS * Sender, bool bUp);

    int RearrangeActor(const T_CLASS * Sender);

    void WndProcessInsertObject();

    void WndRemoveObject();

    void WndSelectObject();

    void WndRenameObject();

    void WndMoveObject();

    void WndReArrange();

    int DirectInvokeActor(const T_CLASS * Sender);

    int	DirectClearActor(const T_CLASS * Sender);

    T_CLASS* GetActorByData(const HTREEITEM lpnmtv);

    bool SelChangedTreeObject();

//------------------------------------------------------------------------
/** object rename operation*/
bool RenameTreeObject()
{
	bool bResult = false;

	HTREEITEM hTreeItem = TreeView_GetSelection(m_hwndTree);

	T_CLASS* Actor = GetActorByData(hTreeItem);

	if (Actor) // do not allow rename root node
	{
		wchar_t wbuf[100] = {0};
		char ascii[100] = {0};

		MultiByteToWideChar(CP_ACP, 0, Actor->GetName(), -1, wbuf, 100);

		if (CWin32InputBox::InputBox(L"Rename", L"Please enter new name", wbuf, 100, false) == IDOK)
		{
			wcstombs(ascii, wbuf, wcslen(wbuf));
			Actor->Rename(ascii);
		}
		bResult = true; // operation clear
	}
	return bResult;
}

	//----------------------------------------------------------------------------------------------
	virtual LRESULT WndProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		int bResult = 0;

		switch (message) 
		{
		case WM_NOTIFY:
			{
				{
					long lResult = HandleNotify(hwndDlg, (int)wParam, (LPNMHDR)lParam);
					if (lResult > 0)
					{
						SetWindowLong(hwndDlg, DWL_MSGRESULT, lResult);
						return TRUE;
					}
				}

				//case IDC_TREE1:
				if (((LPNMHDR)lParam)->code == NM_RCLICK) // context menu
				{	
					ProcessRightClick();
				}

				if (((LPNMHDR)lParam)->code == TVN_KEYDOWN)
				{
					LPNMTVKEYDOWN ptvkd = (LPNMTVKEYDOWN) lParam;
					switch ((DWORD)ptvkd->wVKey) 
					{
						// move Up/Down
					//case 'U': { MoveObjectTree(TreeView_GetSelection(::GetDlgItem(hwndDialogBrowser, IDC_TREE1)), true); } break;
					//case 'J': { MoveObjectTree(TreeView_GetSelection(::GetDlgItem(hwndDialogBrowser, IDC_TREE1)), false); } break;

					case VK_DELETE:{
						//DeleteTreeObject((HTREEITEM)::SendDlgItemMessage(hwndDialogBrowser, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CARET, 0));
								   }break;

					case VK_F2:{
						RenameTreeObject();
							   }break;
					}break;
				}

				// select item handler
				if (((LPNMHDR)lParam)->code == TVN_SELCHANGED)
				{
					bool bSelChanged = SelChangedTreeObject();

					if (!bSelChanged)
						Update(0, Event_OnSelected);

					return bSelChanged;
				}

				if (((LPNMHDR)lParam)->code == TVN_BEGINDRAG)
				{
					return BeginDragTreeObject((LPNMTREEVIEW)lParam);
				}
			}break;

		case WM_SIZING:
		case WM_SIZE:
			{
			/*	RECT rect;
				GetClientRect(GetHWND(), &rect);

				int Width = rect.right - rect.left;
				int Height = rect.bottom - rect.top;

				MoveWindow(GetHWndTree(), rect.left, rect.top, Width, Height, TRUE);*/
			}break;

		case WM_MOUSEMOVE:
			{
				MouseMoveTreeObject(lParam);
			}break;

		case WM_LBUTTONUP:
			{
				EndDragTreeObject();
			}break;

		case WM_COMMAND: 
			// process context menu
			/*if (pfnContextMenuProcessor(GetHWND(), message, wParam, lParam)){
				break;
			}*/
			switch (LOWORD(wParam)) 
			{
			case IDOK: 
				// Fall through. 
			case IDCANCEL: 
				//EndDialog(hwndDlg, wParam); 
				return TRUE; 
			}
			break;
/*
		case WM_USER_REMOVEOBJECT:
			{
				::LockWindowUpdate(hWnd);

				WndRemoveObject();

				::LockWindowUpdate(0);
			}break;
			
		case WM_USER_SELECTOBJECT:
			{
				WndSelectObject();
			}break;
*/
/*		case WM_USER_RENAMEOBJECT:
			{
				WndRenameObject();
			}break;*/

			// old
		case WM_USER_INSERTOBJECT:
			{
				WndProcessInsertObject();
				bResult = 1;
			}break;

		case WM_USER_REMOVEOBJECT_BRWSR:
			{
				WndRemoveObject();
				bResult = 1;
			}break;

		case WM_USER_SELECTOBJECT_BRWSR:
			{
				WndSelectObject();
				bResult = 1;
			}break;

		case WM_USER_RENAMEOBJECT_BRWSR:
			{
				WndRenameObject();
				bResult = 1;
			}break;

		case WM_USER_MOVEOBJECT:
			{
				WndMoveObject();
				bResult = 1;
			}break;

		case WM_USER_REARRANGE:
			{
				WndReArrange();
				bResult = 1;
			}break;

		default:
			break;
		};
		return bResult;
	}

//------------------------------------------------------------------------
long HandleNotify(HWND hWndDlg, int nIDCtrl, LPNMHDR pNMHDR)
{
	if (pNMHDR->hwndFrom == m_hwndTree)
	{
		if (pNMHDR->code == NM_CUSTOMDRAW)
		{
			LPNMTVCUSTOMDRAW pNMTVCD = (LPNMTVCUSTOMDRAW) pNMHDR;
			HWND hWndTreeView = pNMHDR->hwndFrom;

			return HandleCustomDraw(hWndTreeView, pNMTVCD);
		}
	}
	return 0;
}

//------------------------------------------------------------------------
long HandleCustomDraw(HWND hWndTreeView, LPNMTVCUSTOMDRAW pNMTVCD)
{
	if (pNMTVCD == NULL){
		return -1;
	}
	switch (pNMTVCD->nmcd.dwDrawStage)
	{ 
	case CDDS_PREPAINT:
		return (CDRF_NOTIFYPOSTPAINT | CDRF_NOTIFYITEMDRAW);

	case CDDS_ITEMPREPAINT:
		{
			if (pNMTVCD->nmcd.uItemState & CDIS_SELECTED){ // selected
				pNMTVCD->clrTextBk = RGB(200, 200, 200);
			}
			return (CDRF_NOTIFYPOSTPAINT | CDRF_NEWFONT);
		}
	case CDDS_ITEMPOSTPAINT:
		{
			return CDRF_DODEFAULT;
		}
		break;
	default:
		break;
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
static BOOL SubClassProcTree(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	Win32ObjectBrowserWidget *pBrowser = reinterpret_cast<Win32ObjectBrowserWidget*>(GetWindowLong(hWnd, GWL_USERDATA));

	switch (messg) 
	{
	case WM_CREATE:
		{
		}break;

	case WM_COMMAND:
		{
			if (pBrowser->ContextMenuProcessor(hWnd, messg, wParam, lParam)){
				break;
			}
		}break;

	case WM_MOUSEWHEEL:
	case WM_VSCROLL:
		{
			
		}break;

	case WM_MOUSEMOVE:
		{
			//pBrowser->MouseMoveTreeObject(lParam);
		}break;

	case WM_LBUTTONUP:
		{
			//pBrowser->EndDragTreeObject();
		}break;

	case WM_NOTIFY:
		{
			if (((LPNMHDR)lParam)->code == TVN_KEYDOWN)
			{
				LPNMTVKEYDOWN ptvkd = (LPNMTVKEYDOWN) lParam;
				switch ((DWORD)ptvkd->wVKey) 
				{
					// move Up/Down
				//case 'U': { MoveObjectTree(TreeView_GetSelection(::GetDlgItem(hwndDialogBrowser, IDC_TREE1)), true); } break;
				//case 'J': { MoveObjectTree(TreeView_GetSelection(::GetDlgItem(hwndDialogBrowser, IDC_TREE1)), false); } break;

				case VK_DELETE:{
						//DeleteTreeObject((HTREEITEM)::SendDlgItemMessage(hwndDialogBrowser, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CARET, 0));
					}break;

				case VK_F2:{
						pBrowser->RenameTreeObject();
					}break;
				}break;
			}
		}break;
	}

	// Call the original window procedure for default processing. 
	return CallWindowProc(pBrowser->m_lpfnTreeProc, hWnd, messg, wParam, lParam);
}

//------------------------------------------------------------------------
bool Copy(const HTREEITEM hTreeItem)
{
	bool bResult = false;

	T_CLASS * ActorCopy = GetActorByData(hTreeItem);

	if (ActorCopy) // do not allow rename root node
	{
		T_CLASS * RootNode = ActorCopy->GetRootNode();
		if (RootNode)
		{
			RootNode->Copy(ActorCopy);
		}
	}
	return bResult;
}

//------------------------------------------------------------------------
bool Paste(const HTREEITEM hTreeItem)
{
	bool bResult = false;

	T_CLASS * ActorPaste = GetActorByData(hTreeItem);

	if (ActorPaste) // do not allow rename root node
	{
		T_CLASS * RootNode = ActorPaste->GetRootNode();
		if (RootNode)
		{
			RootNode->Paste(ActorPaste);
		}
	}
	return bResult;
}

//------------------------------------------------------------------------
bool BeginDragTreeObject(const LPNMTREEVIEW lpnmtv)
{
	bool bResult = false;

	if (lpnmtv != NULL)
	{
		HIMAGELIST hImg;
		m_bDragging = true; // set global flag

		hImg = TreeView_CreateDragImage(m_hwndTree, lpnmtv->itemNew.hItem);

		ImageList_BeginDrag(hImg, 0, 0, 0);

		ImageList_DragEnter(m_hwndTree, lpnmtv->ptDrag.x, lpnmtv->ptDrag.y);

		SetCapture(m_hwndTree);
		SetCursor(m_hCursHand);

		m_ActorPicked = GetActorByData((HTREEITEM)lpnmtv->itemNew.hItem);
	}
	return bResult;
}

//------------------------------------------------------------------------
bool MouseMoveTreeObject(const LPARAM lParam)
{
	bool bResult = false;

	if (m_bDragging && lParam)
	{
		TVHITTESTINFO tvht;

		POINTS Pos = MAKEPOINTS(lParam);

		ImageList_DragMove(Pos.x + 16, Pos.y); // where to draw the drag from
		ImageList_DragShowNolock(FALSE);

		tvht.pt.x = Pos.x; // the highlight items should be as the same points as the drag
		tvht.pt.y = Pos.y;

		m_hitTarget = (HTREEITEM)SendMessage(m_hwndTree, TVM_HITTEST, NULL, (LPARAM)&tvht);
		if (m_hitTarget) // if there is a hit
		{
			SendMessage(m_hwndTree, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)m_hitTarget);   // highlight it
		}
		ImageList_DragShowNolock(TRUE);
	}
	return bResult;
}

//------------------------------------------------------------------------
bool EndDragTreeObject()
{
	CCoreSDK *pCoreSDK = m_editor->GetApp();

	SHORT CtrlPressed = GetAsyncKeyState(VK_LCONTROL);

	if (m_bDragging) 
	{	
		HTREEITEM hTreeItem = TreeView_GetNextItem(m_hwndTree, 0, TVGN_DROPHILITE);
		
		ImageList_DragLeave(m_hwndTree);
		ImageList_EndDrag();

		HTREEITEM Selected = TreeView_GetNextItem(m_hwndTree, 0, TVGN_DROPHILITE);
		TreeView_Select(m_hwndTree, (LPARAM)Selected, TVGN_CARET);
		TreeView_Select(m_hwndTree, NULL, TVGN_DROPHILITE);

		ReleaseCapture();
		SetCursor(m_hCursArrow);

		T_CLASS * HitActor = m_ActorDrop = GetActorByData(hTreeItem);

		if (m_ActorDrop && CtrlPressed){ // Plain move means get target actor parent
			m_ActorDrop = m_ActorDrop->GetParent();
		}

		if (m_ActorDrop && m_ActorPicked && (m_ActorDrop != m_ActorPicked) && !m_ActorPicked->IsTransient())
		{
			T_CLASS * TempActor = m_ActorDrop->GetParent(); // Check ActorPicked not parent of ActorDrop
			while (TempActor)
			{
				if (TempActor == m_ActorPicked) { // unable put parent object to child directly
					return false;
				}
				TempActor = TempActor->GetParent();
			}

			Matrix LTM;
			/*if (T_CLASS::GetObjectAInLocalSpaceB(LTM, m_ActorDrop, m_ActorPicked))
			{
				// remove actor B from prev node and push to selected
				m_ActorPicked->GetParent()->RemoveChildNode(m_ActorPicked);
				m_ActorPicked->SetParent(m_ActorDrop);
				m_ActorPicked->SetPivot(LTM);
				m_ActorDrop->AddChildNode(m_ActorPicked);

				InvokeActor(m_ActorPicked);

				pCoreSDK->GetViewportManager()->RebuildTransform(m_ActorDrop);

				if (CtrlPressed){
					MoveObjectNearTo(m_ActorPicked, HitActor);
				}

				TTreeMapActorIterator Iter = m_TreeMap.find(m_ActorPicked);
				if (Iter != m_TreeMap.end()){
					TreeView_Select(m_hwndTree, Iter->second, TVGN_CARET);
				}
			}*/
		}
		m_ActorDrop = m_ActorPicked = NULL;
		m_bDragging = false; // drop global flag
	}
	return true;
}

//------------------------------------------------------------------------
bool MoveObjectNearTo(T_CLASS * Picked, const T_CLASS * Target, bool Up /*= false*/)
{
	if (Picked && Target && 
		Picked != Target && 
		Picked->GetParent() == Target->GetParent()) // check objects are in same plane
	{
		T_CLASS * ParentObject = Picked->GetParent();

		if (ParentObject != NULL)
		{
			TVecActorChildIterator IterActorMove   = std::find(ParentObject->m_ChildNodes.begin(), ParentObject->m_ChildNodes.end(), Picked);
			TVecActorChildIterator IterActorMoveTo = std::find(ParentObject->m_ChildNodes.begin(), ParentObject->m_ChildNodes.end(), Target);

			if (IterActorMove != ParentObject->m_ChildNodes.end() && 
				IterActorMoveTo != ParentObject->m_ChildNodes.end())
			{
				TVecActorChild::difference_type n = IterActorMoveTo - IterActorMove;

				int CountNum = abs(int(n));
				while (CountNum > 0)
				{
					ActorAllocator::MoveObjectHierarchy(Picked, m_pRegistry, (n < 0));
					CountNum--;
				}
			}
		}
	}
	return true;
}

//----------------------------------------------------------------------------------------------
void Redraw() const
{
	InvalidateRect(m_hwndTree, NULL, TRUE);
}

//----------------------------------------------------------------------------------------------
void ProcessRightClick()
{
	POINT pt;
	if (::GetCursorPos(&pt)) // address of structure for cursor position
	{
		TVHITTESTINFO tvht;
		tvht.pt = pt;
		ScreenToClient(m_hwndTree, &tvht.pt);

		m_hitTarget = (HTREEITEM)SendMessage(m_hwndTree, TVM_HITTEST, NULL, (LPARAM)&tvht);
		if (m_hitTarget != NULL) // if there is a hit
		{
			m_pfnContextMenu(m_hwndTree, pt, GetActorByData(m_hitTarget));
		}
	}
}

//----------------------------------------------------------------------------------------------
bool ContextMenuProcessor(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	SRenderContext *swapContext = 0;
	if (m_pRenderContext){
		m_editor->GetApp()->GetRenderSDK()->GetRenderDriver()->PushContext(m_pRenderContext);
	}

	bool bResult = m_pfnContextMenuProcessor(hWnd, messg, wParam, lParam);

	if (m_pRenderContext){
        m_editor->GetApp()->GetRenderSDK()->GetRenderDriver()->PopContext();
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
private:
	class Registry			*m_pRegistry;

	TVecActorChild			m_ActorAddList;
	std::vector<HTREEITEM>	m_HTreeClearList;

	HTREEITEM				m_HTreeSelected;
	HTREEITEM				m_HTreeRename;
	HTREEITEM				m_HTreeMove;
	HTREEITEM				m_HTreeMoveTo;

	TTreeMapActor			m_TreeMap;
	HTREEITEM				m_hitTarget;
	T_CLASS*				m_ActorDrop;
	T_CLASS*				m_ActorPicked;
	std::vector<T_CLASS*>	m_ActorsRearrange;

	std::vector<SwapHTree>  m_TMapSwapTree;

	HWND m_hwndParent;
	HWND m_hwndTree;

    CMultiSelectTreeCtrl m_hwndLeft;

	CriticalSection m_CS;

	pContextMenuFunction	m_pfnContextMenu;
	pContextMenuProcessor	m_pfnContextMenuProcessor;
	pGetResourceIconIndex	m_pfnGetResourceIconIndex;

	bool m_bDragging;
	HCURSOR	m_hCursHand, m_hCursArrow;     // cursor handles

	SRenderContext			*m_pRenderContext;

    std::shared_ptr<editors::IEditor> m_editor;

public:
	WNDPROC					m_lpfnTreeProc;
	CALLBACK_FN				m_pfnInvokeObject;
	CALLBACK_FN				m_pfnDirectInvokeObject;
	CALLBACK_FN				m_pfnClearObject;
	CALLBACK_FN				m_pfnDirectClearObject;
};

#include "Win32ObjectBrowserWidget.ipp"