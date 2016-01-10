#ifndef __win32cameramanager_h__
#define __win32cameramanager_h__

#ifdef WIN32
#pragma once
#endif
/*
#include "..\..\Foundation\StdafxFoundation.h"

class CWin32CameraManager
{
public:
	typedef void (*ContextMenuFunction) (HWND hwnd, POINT pt, class CActor * Actor);
	typedef bool (*ContextMenuProcessor)(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	typedef int  (*GetResourceIconIndex)(const std::string& TypeName);

	static void Initialize(HWND hWndParent,
							ContextMenuFunction pfMenu,
							ContextMenuProcessor pfnMenuProcessor,
							GetResourceIconIndex pfnGetResourceIconIndex,
							HIMAGELIST hImageList,
							const class CWindowManager *WMgr);

	static void ShowDialogWindow(bool bShow = true);
	static bool IsVisible();

	static BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
	static void Update(const class CActor * Sender, ESystemEventID EventId);

	static int CALLBACK_InvokeActor(const class CActor * Sender);
	static int CALLBACK_ClearActor(const CActor * Sender);

	static CActor* GetActorByData(const HTREEITEM lpnmtv);
	static bool RenameTreeObject(const HTREEITEM hTreeItem);
	static bool SelChangedTreeObject(const HTREEITEM hTreeItem);

protected:
private:
};
*/
#endif