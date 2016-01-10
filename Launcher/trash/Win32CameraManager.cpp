#include "LauncherStdAfx.h"
/*
#define WM_USER_REMOVEOBJECT				WM_APP+12
#define WM_USER_RENAMEOBJECT				WM_APP+13
#define WM_USER_SELECTOBJECT				WM_APP+14

namespace NMWin32CameraManager
{
	const CWindowManager	*pWindowManager = NULL;
	CameraManager			*pCameraManager = NULL;

 	typedef std::map<const class CActor*, HTREEITEM>  TTreeMapActor;
 	typedef TTreeMapActor::iterator					  TTreeMapActorIterator;

 	std::vector<HTREEITEM>	HTreeClearList;
 	HTREEITEM				HTreeSelected = NULL;
 	HTREEITEM				HTreeRename	= NULL;


	HWND			hwndDialog = NULL;
	TTreeMapActor	TreeMap;
 	HTREEITEM		hitTarget = NULL;

	CWin32CameraManager::ContextMenuFunction	pfnContextMenu;
	CWin32CameraManager::ContextMenuProcessor	pfnContextMenuProcessor;
	CWin32CameraManager::GetResourceIconIndex	pfnGetResourceIconIndex;

	CriticalSection InvokeActorCS;

}using namespace NMWin32CameraManager;

//------------------------------------------------------------------------
void CWin32CameraManager::Initialize(HWND hWndParent,
									 ContextMenuFunction pfMenu,
									 ContextMenuProcessor pfnMenuProcessor,
									 GetResourceIconIndex pfnGetResourceIconIdx,
									 HIMAGELIST hImageList,
									 const CWindowManager *WMgr)
{
	HWND hWnd = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG_CAMERAS), hWndParent, (DLGPROC)CWin32CameraManager::DlgProc);

	if (hWnd != NULL)
	{
		pWindowManager			= WMgr;
		pCameraManager			= WMgr->GetCoreSDK()->GetCameraManager();
		hwndDialog				= hWnd;
		pfnContextMenu			= pfMenu;
		pfnContextMenuProcessor = pfnMenuProcessor;
		pfnGetResourceIconIndex = pfnGetResourceIconIdx;

		SendDlgItemMessage(hwndDialog, IDC_TREE_CAMERAS, TVM_SETIMAGELIST, 0, (LPARAM)hImageList);
	}
}

//------------------------------------------------------------------------
void CWin32CameraManager::ShowDialogWindow(bool bShow)
{
	if (hwndDialog)
	{
		ShowWindow(hwndDialog, (bShow == true) ? SW_SHOW : SW_HIDE);
	}
}

//------------------------------------------------------------------------
bool CWin32CameraManager::IsVisible()
{
	if (hwndDialog)
	{
		return IsWindowVisible(hwndDialog) ? true : false;
	}
	return false;
}


//------------------------------------------------------------------------
BOOL CALLBACK CWin32CameraManager::DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message) 
	{
	case WM_INITDIALOG:
		{
		}break;

	case WM_NOTIFY:
		{
		case IDC_TREE_CAMERAS:
			{
				if (((LPNMHDR)lParam)->code == NM_RCLICK) // context menu
				{	
					POINT pt;
					if (::GetCursorPos(&pt)) // address of structure for cursor position
					{
						TVHITTESTINFO tvht;
						tvht.pt = pt;
						ScreenToClient(::GetDlgItem(hwndDialog, IDC_TREE_CAMERAS), &tvht.pt);

						hitTarget = (HTREEITEM)SendMessage(::GetDlgItem(hwndDialog, IDC_TREE_CAMERAS), TVM_HITTEST, NULL, (LPARAM)&tvht);
						if (hitTarget != NULL) // if there is a hit
						{
							pfnContextMenu(hwndDlg, pt, GetActorByData(hitTarget));
						}
					}
				}

				if (((LPNMHDR)lParam)->code == TVN_KEYDOWN)
				{
					LPNMTVKEYDOWN ptvkd = (LPNMTVKEYDOWN) lParam;
					switch ((DWORD)ptvkd->wVKey) 
					{
					case VK_F2:
						{
							RenameTreeObject((HTREEITEM)::SendDlgItemMessage(hwndDialog, IDC_TREE_CAMERAS, TVM_GETNEXTITEM, TVGN_CARET, 0));
						}break;
					}break;
				}

				// select item handler
				if (((LPNMHDR)lParam)->code == TVN_SELCHANGED)
				{
					//return SelChangedTreeObject((HTREEITEM)::SendDlgItemMessage(hwndDialog, IDC_TREE_CAMERAS, TVM_GETNEXTITEM, TVGN_CARET, 0));
				}
			}break;
		}break;

	case WM_COMMAND:
		{
			// process context menu
 			if (pfnContextMenuProcessor(hwndDlg, message, wParam, lParam)){
 				break;
 			}
		}break;

	case WM_USER_REMOVEOBJECT:
		{
			InvokeActorCS.enter();

			::LockWindowUpdate(::GetDlgItem(hwndDialog, IDC_TREE_CAMERAS));

			for (std::vector<HTREEITEM>::iterator Iter = HTreeClearList.begin(); Iter != HTreeClearList.end(); ++Iter)
			{
				TreeView_DeleteItem(::GetDlgItem(hwndDialog, IDC_TREE_CAMERAS), *Iter);
			}

			HTreeClearList.clear();

			::LockWindowUpdate(NULL);

			InvokeActorCS.leave();
		}break;

	case WM_USER_RENAMEOBJECT:
		{
			InvokeActorCS.enter();

			for (TTreeMapActorIterator Iter = TreeMap.begin(); Iter != TreeMap.end(); ++Iter)
			{
				if (Iter->second == HTreeRename)
				{
					TVITEMEX NodeItem;
					NodeItem.hItem = Iter->second;
					NodeItem.cchTextMax = 100;
					NodeItem.pszText = 0;
					NodeItem.mask = TVIF_TEXT;

					if (TreeView_GetItem(::GetDlgItem(hwndDialog, IDC_TREE_CAMERAS), &NodeItem))
					{
						std::wstring Name = ConvertStringToWideString(Iter->first->GetName());
						NodeItem.pszText =(LPWSTR)Name.c_str();
						TreeView_SetItem(::GetDlgItem(hwndDialog, IDC_TREE_CAMERAS), &NodeItem);
					}
					break;
				}
			}
			InvokeActorCS.leave();
		}break;

	case WM_USER_SELECTOBJECT:
		{
			InvokeActorCS.enter();

			TreeView_Select(::GetDlgItem(hwndDialog, IDC_TREE_CAMERAS), HTreeSelected, TVGN_CARET);
			TreeView_EnsureVisible(::GetDlgItem(hwndDialog, IDC_TREE_CAMERAS), HTreeSelected);

			HTreeSelected = NULL;

			InvokeActorCS.leave();
		}break;

	case WM_CLOSE:
		{
			ShowDialogWindow(!IsVisible());
			pWindowManager->UpdateFlagsState();
		}break;
	} 
	return FALSE; 
}

//------------------------------------------------------------------------
void CWin32CameraManager::Update(const class CActor * Sender, ESystemEventID EventId)
{
	switch (EventId)
	{
	case Event_ObjectGenerated:
		{
			if (dynamic_cast<const CCamera*>(Sender))
			{
				CALLBACK_InvokeActor(Sender);
			}
		}break;

	case Event_PreRelease:
		{
			if (dynamic_cast<const CCamera*>(Sender))
			{
				CALLBACK_ClearActor(Sender);
			}
		}break;


	case Event_OnSelected:
		{
			InvokeActorCS.enter();

			TTreeMapActorIterator Iter = TreeMap.find(Sender);
			HTreeSelected = (Iter != TreeMap.end()) ? Iter->second : NULL;

			InvokeActorCS.leave();

			::PostMessage(hwndDialog, WM_USER_SELECTOBJECT, 0, 0);
		}break;

	case Event_ObjectRename:
		{
			InvokeActorCS.enter();

			TTreeMapActorIterator Iter = TreeMap.find(Sender);
			HTreeRename = (Iter != TreeMap.end()) ? Iter->second : NULL;

			InvokeActorCS.leave();

			::PostMessage(hwndDialog, WM_USER_RENAMEOBJECT, 0, 0);
		}break;

	}
}

//------------------------------------------------------------------------
int CWin32CameraManager::CALLBACK_InvokeActor(const class CActor * Sender)
{
	TV_INSERTSTRUCT tvinsert;			// struct to config out tree control
	tvinsert.hParent		= NULL;		// top most level no need handle
	tvinsert.hInsertAfter	= TVI_ROOT; // work as root level
	tvinsert.item.mask		= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	int IndexBitmap = pfnGetResourceIconIndex(Sender->GetType());

	std::wstring Name		= ConvertStringToWideString(Sender->GetName());
	tvinsert.item.pszText	= (LPWSTR)Name.c_str();
	tvinsert.item.iImage	= IndexBitmap;
	tvinsert.item.iSelectedImage = IndexBitmap;

	TreeMap.insert(std::make_pair(Sender, (HTREEITEM)::SendDlgItemMessage(hwndDialog, IDC_TREE_CAMERAS, TVM_INSERTITEM, 0, (LPARAM)&tvinsert)));

	//pCameraManager->RegisterCamera(dynamic_cast<const CCamera*>(Sender));

	return 0;
}

//------------------------------------------------------------------------
int CWin32CameraManager::CALLBACK_ClearActor(const CActor * Sender)
{	
	InvokeActorCS.enter();

	TTreeMapActorIterator IterFind = TreeMap.find(Sender);

	if (IterFind != TreeMap.end())
	{
		HTreeClearList.push_back(IterFind->second);
		TreeMap.erase(IterFind);
	}

	//pCameraManager->UnregisterCamera(dynamic_cast<const CCamera*>(Sender));
	
	::PostMessage(hwndDialog, WM_USER_REMOVEOBJECT, 0, 0);
	
	InvokeActorCS.leave();

	return 0;
}

//------------------------------------------------------------------------
CActor* CWin32CameraManager::GetActorByData(const HTREEITEM hTreeItem)
{
	if (hTreeItem)
	{
		TTreeMapActorIterator Iter = TreeMap.begin();
		while (Iter != TreeMap.end())
		{
			if (Iter->second == hTreeItem)
			{
				return const_cast<CActor*>(Iter->first);
			}
			++Iter;
		}
	}
	return NULL;
}

//------------------------------------------------------------------------
bool CWin32CameraManager::RenameTreeObject(const HTREEITEM hTreeItem)
{
	bool bResult = false;

	CActor * Actor = GetActorByData(hTreeItem);

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

//------------------------------------------------------------------------
bool CWin32CameraManager::SelChangedTreeObject(const HTREEITEM hTreeItem)
{
	bool bResult = false;

	CCoreSDK *pCoreSDK = pWindowManager->GetCoreSDK();

	CActor *pActor = GetActorByData(hTreeItem);

	if (pActor) // find corresponding
	{
		//CWin32PropertyWindow::Update(Actor, Event_OnSelected); // update property window
		
		IDrawInterface *pIFocused = pCoreSDK->GetViewportManager()->GetByActor(pActor);
		if (pIFocused && !pIFocused->IsFocused())
		{
			pCoreSDK->GetViewportManager()->SetFocus(pIFocused);// set focused state
		}

		bResult = true;
	}
	return bResult;
}*/