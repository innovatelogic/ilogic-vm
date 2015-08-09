#ifndef __win32objecttoolbox_h__
#define __win32objecttoolbox_h__

#ifdef _WIN32
#pragma once
#endif

#include "..\..\Foundation\StdafxFoundation.h"

class CWin32ObjectToolbox
{
public:
	typedef int  (*GetResourceIconIndex)(const std::string& TypeName);

	static void Initialize(HWND hWndParent, GetResourceIconIndex pfnGetResourceIconIndex, HIMAGELIST hImageList, const class CWindowManager *WMgr);
	static void ShowDialogWindow(bool bShow = true);
	static bool IsVisible();
	static void Update(const class CActor * Sender, ESystemEventID EventId);
	static BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
	
	/** callback invoke for each actor*/
	static int CALLBACK_InvokeActor(const class CActor * Sender);
	static bool SelChangedTreeObject(const HTREEITEM hTreeItem);
};
#endif//__win32objecttoolbox_h__