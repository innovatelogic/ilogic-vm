#ifndef __win32threadmonitor_h__
#define __win32threadmonitor_h__

#ifdef _WIN32
#pragma once
#endif

#include "..\..\Foundation\StdafxFoundation.h"

class CWin32ThreadMonitor
{
public:
	static void Initialize(HWND hWndParent, const class CWindowManager *WMgr, int UpdateID, int RenderID, int TimerID, int StreamID);
	static void RegisterUpdateboxClass() ;
	static void ShowDialogWindow(bool bShow = true);
	static bool IsVisible();
	static BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static bool IsThreadsSerialized();

	static void SetCPS_Main(unsigned int cps);
	static void SetCPS_Render(unsigned int cps);
	static void SetCPS_Update(unsigned int cps);
	static void SetCPS_Timer(unsigned int cps);

	static LRESULT CALLBACK UpdateBoxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

//------------------------------------------------------------------------
class UpdateBox 
{
public:
	UpdateBox();
private:
	POINT pos;     
};

#endif//__win32threadmonitor_h__