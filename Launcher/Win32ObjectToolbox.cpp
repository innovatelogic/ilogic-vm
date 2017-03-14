#include "LauncherStdAfx.h"

#define WM_USER_UPDATE						WM_APP+1
#define WM_USER_RELEASE						WM_APP+2
#define WM_USER_DESELECT					WM_APP+3

namespace dialog_toolbox
{	
	/*const CWindowManager *pWindowManager = NULL;
	
	typedef std::map<const CActor*, HTREEITEM>  TTreeMapActor;
	typedef TTreeMapActor::iterator				TTreeMapActorIterator;

	bool bDraggingTemplate	= false;
	HWND hwndDlgToolbox		= NULL;
	TTreeMapActor	TreeMapToolbox;

	CWin32ObjectToolbox::GetResourceIconIndex	pfnGetResourceIconIndex;

	CriticalSection DlgToolboxCS;*/
};

using namespace dialog_toolbox;

//------------------------------------------------------------------------
void CWin32ObjectToolbox::Initialize(HWND hWndParent, GetResourceIconIndex pfnGetResourceIndex, HIMAGELIST hImageList, const CWindowManager *WMgr)
{
	/*HWND hWnd = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG_TOOLBOX), hWndParent, (DLGPROC)CWin32ObjectToolbox::DlgProc);

	if (hWnd != NULL)
	{
		pWindowManager = WMgr;
		hwndDlgToolbox = hWnd;
		pfnGetResourceIconIndex = pfnGetResourceIndex;

		SendDlgItemMessage(hwndDlgToolbox, IDC_TREE_TOOLBOX, TVM_SETIMAGELIST, 0, (LPARAM)hImageList);
	}*/
}

//------------------------------------------------------------------------
void CWin32ObjectToolbox::ShowDialogWindow(bool bShow /*= true*/)
{
	/*if (hwndDlgToolbox != NULL)
	{
		ShowWindow(hwndDlgToolbox, bShow ? SW_SHOW : SW_HIDE);
	}*/
}
//------------------------------------------------------------------------
bool CWin32ObjectToolbox::IsVisible()
{
	/*if (hwndDlgToolbox)
	{
		return IsWindowVisible(hwndDlgToolbox) ? true : false;
	}*/
	return false;
}
//------------------------------------------------------------------------
BOOL CWin32ObjectToolbox::DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
/*	switch (message) 
	{
	case WM_INITDIALOG:
		{
		}break;

	case WM_USER_UPDATE:
		{
			//DlgToolboxCS.enter();

			TreeView_DeleteAllItems(::GetDlgItem(hwndDlgToolbox, IDC_TREE_TOOLBOX));
			TTreeMapActorIterator Iter = TreeMapToolbox.begin();

			while (Iter != TreeMapToolbox.end())
			{
				TV_INSERTSTRUCT tvinsert;			// struct to config out tree control
				tvinsert.hParent		= NULL;		// top most level no need handle
				tvinsert.hInsertAfter	= TVI_ROOT; // work as root level
				tvinsert.item.mask		= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

				int IndexBitmap = pfnGetResourceIconIndex(Iter->first->GetType());

				std::wstring Name		= ConvertStringToWideString(Iter->first->GetName());
				tvinsert.item.pszText	= (LPWSTR)Name.c_str();
				tvinsert.item.iImage	= IndexBitmap;
				tvinsert.item.iSelectedImage = IndexBitmap;

				Iter->second = (HTREEITEM)::SendDlgItemMessage(hwndDlgToolbox, IDC_TREE_TOOLBOX, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
				++Iter;
			}

			//DlgToolboxCS.leave();
		}break;

	case WM_USER_DESELECT:
		{
			TreeView_SelectItem(::GetDlgItem(hwndDlgToolbox, IDC_TREE_TOOLBOX), NULL);
		}break;

	case WM_NOTIFY:
	case IDC_TREE_TOOLBOX:
		{
			if (((LPNMHDR)lParam)->code == NM_CLICK) // rename
			{	
				//DlgToolboxCS.enter();

				POINT pt;
				if (GetCursorPos(&pt)) // address of structure for cursor position
				{
					TVHITTESTINFO tvht;
					tvht.pt = pt;
					ScreenToClient(::GetDlgItem(hwndDlg, IDC_TREE_TOOLBOX), &tvht.pt);
					SelChangedTreeObject((HTREEITEM)SendMessage(::GetDlgItem(hwndDlg, IDC_TREE_TOOLBOX), TVM_HITTEST, NULL, (LPARAM)&tvht));
				}

				//DlgToolboxCS.leave();
			}
		}break;

	case WM_CLOSE:
		{
			ShowDialogWindow(!IsVisible());
			pWindowManager->UpdateFlagsState();
		}break;
	};*/
	return FALSE;
}

//------------------------------------------------------------------------
void CWin32ObjectToolbox::Update(const CActor * Sender, ESystemEventID EventId)
{	

}

//------------------------------------------------------------------------
int CWin32ObjectToolbox::CALLBACK_InvokeActor(const CActor * Sender)
{
/*	TV_INSERTSTRUCT tvinsert;			// struct to config out tree control
	tvinsert.hParent		= NULL;		// top most level no need handle
	tvinsert.hInsertAfter	= TVI_ROOT; // work as root level
	tvinsert.item.mask		= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	int IndexBitmap = pfnGetResourceIconIndex(Sender->GetType());

	std::wstring Name		= ConvertStringToWideString(Sender->GetName());
	tvinsert.item.pszText	= (LPWSTR)Name.c_str();
	tvinsert.item.iImage	= IndexBitmap;
	tvinsert.item.iSelectedImage = IndexBitmap;
	
	TreeMapToolbox.insert(std::make_pair(Sender, (HTREEITEM)::SendDlgItemMessage(hwndDlgToolbox, IDC_TREE_TOOLBOX, TVM_INSERTITEM, 0, (LPARAM)&tvinsert)));
	*/
	return 0;
}

//------------------------------------------------------------------------
bool CWin32ObjectToolbox::SelChangedTreeObject(const HTREEITEM hTreeItem)
{
	bool bResult = false;
/*
	//DlgToolboxCS.enter();

	//pWindowManager->GetCoreSDK()->GetRenderSDK()->EnterCS();

	TTreeMapActorIterator Iter = TreeMapToolbox.begin();
	while (Iter != TreeMapToolbox.end())
	{
		if (Iter->second == hTreeItem) // find corresponding
		{
			//const_cast<CActor*>(Iter->first)->GainFocus();
			break;
		}
		Iter++;
	}

	//pWindowManager->GetCoreSDK()->GetRenderSDK()->LeaveCS();
	//DlgToolboxCS.leave();
	*/
	return bResult;
}
