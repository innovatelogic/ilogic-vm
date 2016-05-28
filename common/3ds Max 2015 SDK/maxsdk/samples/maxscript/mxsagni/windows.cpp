/**********************************************************************
*<
FILE: windows.cpp

DESCRIPTION: 

CREATED BY: Larry Minton

HISTORY: Created 15 April 2007

*>	Copyright (c) 2007, All Rights Reserved.
**********************************************************************/

#include <maxscript/maxscript.h>
#include <maxscript/foundation/numbers.h>
#include <maxscript/foundation/3dmath.h>		// Point2Value
#include <maxscript/foundation/strings.h>
#include <maxscript/maxwrapper/bitmaps.h>
#include "MXSAgni.h"

#ifdef ScripterExport
	#undef ScripterExport
#endif
#define ScripterExport __declspec( dllexport )

#include <maxscript\macros\define_instantiation_functions.h>
#include "extclass.h"

extern Bitmap* CreateBitmapFromBInfo(void **ptr, const int cx, const int cy, bool captureAlpha);
extern void DeGammaCorrectBitmap(Bitmap* map);

// ============================================================================

#include <maxscript\macros\define_external_functions.h>
#	include "namedefs.h"

#include <maxscript\macros\define_instantiation_functions.h>
#	include "windows_wraps.h"
// -----------------------------------------------------------------------------------------

struct EnumProcArg
{
	Array* returnArray; // will contain the array to store results it
	bool checkParent; // if true, check window parent against parent member value below
	HWND parent; // parent window value to check against
	const TCHAR* matchString; // string to test window text against
	EnumProcArg(Array* returnArray, bool checkParent, HWND parent, const TCHAR* matchString = 0) 
		: returnArray(returnArray), checkParent(checkParent), parent(parent), matchString(matchString) {}
};

static const int CollectWindowData_DataSize = 8;

// function for collecting data for a specified window
static void CollectWindowData (HWND hWnd, Array *winData)
{
	TCHAR className[256] = {};
	TCHAR windowText[256] = {};
	int bufSize = _countof(windowText);

	// 1: HWND
	winData->append(IntegerPtr::intern((INT_PTR)hWnd));  // 1
	HWND ancestor = GetAncestor(hWnd,GA_PARENT);
	// 2: the parent HWND. This does not include the owner
	winData->append(IntegerPtr::intern((INT_PTR)ancestor)); // 2
	HWND par = GetParent(hWnd);
	// 3: the parent or owner HWND. If the window is a child window, the value is a handle to the parent window. 
	// If the window is a top-level window, the value is a handle to the owner window.
	winData->append(IntegerPtr::intern((INT_PTR)par)); // 3
	GetClassName(hWnd, className, bufSize);
	// 4: class name as a string � limit of 255 characters
	winData->append(new String(className)); // 4
	GetWindowText(hWnd, windowText, bufSize);
	// 5: window text as a string � limit of 255 characters
	winData->append(new String(windowText)); // 5
	HWND owner = GetWindow(hWnd,GW_OWNER);
	// 6: the owner's HWND 
	winData->append(IntegerPtr::intern((INT_PTR)owner)); // 6
	HWND root = GetAncestor(hWnd,GA_ROOT);
	// 7: root window HWND. The root window as determined by walking the chain of parent windows
	winData->append(IntegerPtr::intern((INT_PTR)root)); // 7
	HWND rootowner = GetAncestor(hWnd,GA_ROOTOWNER);
	// 8: owned root window HWND. The owned root window as determined by walking the chain of parent and owner 
	// windows returned by GetParent
	winData->append(IntegerPtr::intern((INT_PTR)rootowner)); // 8
}

Value*
getData_HWND_cf(Value** arg_list, int count)
{
	// windows.getHWNDData {<int_HWND>|#max} 
	check_arg_count(windows.getHWNDData, 1, count);
	HWND hWnd = NULL;
	if (arg_list[0] == n_max)  
		hWnd = MAXScript_interface->GetMAXHWnd();
	else
		hWnd = (HWND)arg_list[0]->to_intptr();
	if (hWnd == 0)
		hWnd = GetDesktopWindow();

	if (IsWindow(hWnd)) 
	{
		one_typed_value_local(Array* result);
		vl.result = new Array (CollectWindowData_DataSize);
		CollectWindowData(hWnd, vl.result);
		return_value(vl.result);
	}
	else
		return &undefined;
}

static BOOL CALLBACK CollectorEnumChildProc(HWND hWnd, LPARAM lParam)
{
	EnumProcArg *enumProcArg = (EnumProcArg*)lParam;

	HWND par = GetParent(hWnd);
	if (enumProcArg->checkParent && (par != enumProcArg->parent)) 
		return TRUE;

	Array *winData = new Array(CollectWindowData_DataSize);
	enumProcArg->returnArray->append(winData);
	CollectWindowData(hWnd, winData);

	return TRUE;
}

Value*
getChildren_HWND_cf(Value** arg_list, int count)
{
	// windows.getChildrenHWND {<int_HWND>|#max} parent:{<int_HWND>|#max}
	check_arg_count_with_keys(windows.getChildrenHWND, 1, count);
	HWND hWnd = NULL;
	if (arg_list[0] == n_max)  
		hWnd = MAXScript_interface->GetMAXHWnd();
	else
		hWnd = (HWND)arg_list[0]->to_intptr();

	Value *tmp;
	tmp = key_arg(parent);
	bool checkParent = false;
	HWND parent_hwnd = NULL;
	if (tmp == n_max)
	{
		parent_hwnd = MAXScript_interface->GetMAXHWnd();
		checkParent = true;
	}
	else if (tmp != &unsupplied)
	{
		parent_hwnd = (HWND)intptr_key_arg(parent,tmp,0);
		checkParent = true;
	}
	if (checkParent && parent_hwnd == 0)
		parent_hwnd = GetDesktopWindow();

	if (hWnd == 0 || IsWindow(hWnd)) 
	{
		one_typed_value_local(Array* result);
		vl.result = new Array (0);
		EnumProcArg enumProcArg(vl.result, checkParent, parent_hwnd);
		EnumChildWindows(hWnd, CollectorEnumChildProc, (LPARAM)&enumProcArg);
		return_value(vl.result);
	}
	else
		return &undefined;
}

static BOOL CALLBACK MatchEnumChildProc(HWND hWnd, LPARAM lParam)
{
	EnumProcArg *enumProcArg = (EnumProcArg*)lParam;

	TCHAR windowText[256];
	int bufSize = _countof(windowText);

	HWND par = GetParent(hWnd);
	if (enumProcArg->checkParent && (par != enumProcArg->parent)) 
		return TRUE;

	int res2 = GetWindowText(hWnd, windowText, bufSize);

	if (res2)
	{
		if (_tcsnicmp(windowText, enumProcArg->matchString, bufSize) == 0)
		{
			CollectWindowData(hWnd, enumProcArg->returnArray);
			return FALSE;
		}
	}

	return TRUE;
}

Value*
getChild_HWND_cf(Value** arg_list, int count)
{
	// windows.getChildHWND {<int_HWND>|#max} <string> parent:{<int_HWND>|#max}
	check_arg_count_with_keys(windows.getChildHWND, 2, count);
	HWND hWnd = NULL;
	if (arg_list[0] == n_max)  
		hWnd = MAXScript_interface->GetMAXHWnd();
	else
		hWnd = (HWND)arg_list[0]->to_intptr();

	Value *tmp;
	tmp = key_arg(parent);
	bool checkParent = false;
	HWND parent_hwnd = NULL;
	if (tmp == n_max)
	{
		parent_hwnd = MAXScript_interface->GetMAXHWnd();
		checkParent = true;
	}
	else if (tmp != &unsupplied)
	{
		parent_hwnd = (HWND)intptr_key_arg(parent,tmp,0);
		checkParent = true;
	}
	if (checkParent && parent_hwnd == 0)
		parent_hwnd = GetDesktopWindow();

	if (hWnd == 0 || IsWindow(hWnd)) 
	{
		one_typed_value_local(Array* result);
		vl.result = new Array (0);
		EnumProcArg enumProcArg(vl.result, checkParent, parent_hwnd, arg_list[1]->to_string());
		BOOL notFound = EnumChildWindows(hWnd, MatchEnumChildProc, (LPARAM)&enumProcArg);
		if (notFound)
			return_value(&undefined)
		else
			return_value(vl.result);
	}
	else
		return &undefined;
}

Value*
getMAX_HWND_cf(Value** arg_list, int count)
{
	// windows.getMAXHWND() 
	check_arg_count(windows.getMAXHWND, 0, count);
	return IntegerPtr::intern((INT_PTR)MAXScript_interface->GetMAXHWnd());
}

Value*
getDesktop_HWND_cf(Value** arg_list, int count)
{
	// windows.getDesktopHWND() 
	check_arg_count(windows.getDesktopHWND, 0, count);
	return IntegerPtr::intern((INT_PTR)GetDesktopWindow());
}

Value*
send_message_cf(Value** arg_list, int count)
{
	// windows.sendMessage <int HWND> <int message> <int messageParameter1> <int messageParameter2>
	check_arg_count(windows.sendMessage, 4, count);

#ifdef USE_GMAX_SECURITY
	NOT_SUPPORTED_BY_PRODUCT("windows.sendMessage");
#else

	HRESULT hRes = SendMessage(
		(HWND)arg_list[0]->to_intptr(),
		(UINT)arg_list[1]->to_int(),
		(WPARAM)arg_list[2]->to_intptr(),
		(LPARAM)arg_list[3]->to_intptr());
	return Integer::intern(hRes);
#endif // USE_GMAX_SECURITY
}

Value*
post_message_cf(Value** arg_list, int count)
{
	// windows.postMessage <int HWND> <int message> <int messageParameter1> <int messageParameter2>
	check_arg_count(windows.postMessage, 4, count);

#ifdef USE_GMAX_SECURITY
	NOT_SUPPORTED_BY_PRODUCT("windows.postMessage");
#else

	BOOL res = PostMessage(
		(HWND)arg_list[0]->to_intptr(),
		(UINT)arg_list[1]->to_int(),
		(WPARAM)arg_list[2]->to_intptr(),
		(LPARAM)arg_list[3]->to_intptr());
	return bool_result(res);
#endif // USE_GMAX_SECURITY
}

Value*
process_messages_cf(Value** arg_list, int count)
{
	// windows.processPostedMessages() 
	check_arg_count(windows.processPostedMessages, 0, count);
	MSG wmsg;
	while (PeekMessage(&wmsg, NULL, 0 , 0, PM_REMOVE)) 
	{
		MAXScript_interface->TranslateAndDispatchMAXMessage(wmsg); 
		if (GetCOREInterface7()->QuitingApp())
			throw SignalException(); 
	}
	return &ok;
}

Value*
getParent_HWND_cf(Value** arg_list, int count)
{
	// windows.getParentHWND() <int HWND>
	check_arg_count(windows.getParentHWND, 1, count);
	return IntegerPtr::intern((INT_PTR)::GetParent((HWND)arg_list[0]->to_intptr()));
}

Value*
set_window_pos_cf(Value** arg_list, int count)
{
	// windows.setWindowPos <int HWND> <int x> <int y> <int width> <int height> <bool repaint>
	//		** OR **
	// windows.setWindowPos <int HWND> <Box2 rect> <bool repaint>
	if (count != 6 && count != 3) {
		throw ArgCountError (_M("windows.setWindowPos"), 6, count);
	}

	Box2 box;
	BOOL repaint;
	if (count == 6) {
		box.SetXY(arg_list[1]->to_int(), arg_list[2]->to_int());
		box.SetWH(arg_list[3]->to_int(), arg_list[4]->to_int());
		repaint = arg_list[5]->to_bool();
	}
	else {
		box = arg_list[1]->to_box2();
		repaint = arg_list[2]->to_bool();
	}

	// For a top-level window, the position and dimensions are relative to the upper-left corner of the screen. 
	// For a child window, they are relative to the upper-left corner of the parent window's client area
	BOOL res = ::MoveWindow(
		(HWND)arg_list[0]->to_intptr(),
		box.x(), box.y(), box.w(), box.h(), repaint);
	return bool_result(res);
}


Value*
get_window_pos_cf(Value** arg_list, int count)
{
	// <Box2 rect> windows.getWindowPos <int HWND> 
	check_arg_count(windows.getWindowRect, 1, count);

	RECT wRect;
	// The dimensions are given in screen coordinates that are relative to the upper-left corner of the screen.
	BOOL res = ::GetWindowRect(
		(HWND)arg_list[0]->to_intptr(),
		&wRect);

	if (res) {
		one_typed_value_local(Box2Value* result);
		// Per Windows GetWindowRect specs: 'In conformance with conventions for the RECT structure, 
		// the bottom-right coordinates of the returned rectangle are exclusive'.
		// So in order to have compatible values in maxscript setWindowPos&getWindowPos,
		// adjusting the results from GetWindowRect to be inclusive
		wRect.right--; wRect.bottom--;
		vl.result = new Box2Value(wRect);
		return_value(vl.result);
	}
	else
		return &undefined;
}

Value*
client_to_screen_cf(Value** arg_list, int count)
{
	// <Point2> windows.clientToScreen <int HWND> <int x> <int y>
	//		** OR **
	// <Point2> windows.clientToScreen <int HWND> <Point2 clientCoord>
	if (count != 2 && count != 3) {
		throw ArgCountError (_M("windows.clientToScreen"), 3, count);
	}

	POINT coord;
	if (count == 3) {
		coord.x = arg_list[1]->to_int(); coord.y = arg_list[2]->to_int();
	}
	else {
		Point2 clientCoord = arg_list[1]->to_point2();
		coord.x = clientCoord.x; coord.y = clientCoord.y; 
	}
	// ClientToScreen returns the new screen coordinates into 'coord' if the function succeeds.
	BOOL res = ::ClientToScreen(
		(HWND)arg_list[0]->to_intptr(),
		&coord);

	if (res) {
		one_typed_value_local(Point2Value* result);
		vl.result = new Point2Value(coord);
		return_value(vl.result);
	}
	else
		return &undefined;
}


Value*
screen_to_client_cf(Value** arg_list, int count)
{
	// <Point2> windows.screenToClient <int HWND> <int x> <int y>
	//		** OR **
	// <Point2> windows.screenToClient <int HWND> <Point2 screenCoord>
	if (count != 2 && count != 3) {
		throw ArgCountError (_M("windows.screenToClient"), 3, count);
	}

	POINT coord;
	if (count == 3) {
		coord.x = arg_list[1]->to_int(); coord.y = arg_list[2]->to_int();
	}
	else {
		Point2 screenCoord = arg_list[1]->to_point2();
		coord.x = screenCoord.x; coord.y = screenCoord.y; 
	}
	// ScreenToClient returns the new client coordinates into 'coord' if the function succeeds.
	BOOL res = ::ScreenToClient(
		(HWND)arg_list[0]->to_intptr(),
		&coord);

	if (res) {
		one_typed_value_local(Point2Value* result);
		vl.result = new Point2Value(coord);
		return_value(vl.result);
	}
	else
		return &undefined;
}

Value* snapshot_cf(Value** arg_list, int count)
{
	// windows.snapshot {<int_HWND>|#max}  captureAlpha:<bool> gammaCorrect:<bool> captureScreenPixels:<bool>
	check_arg_count_with_keys(windows.snapshot, 1, count);
	bool captureAlpha = key_arg_or_default(captureAlpha, &false_value)->to_bool() ? true : false;
	bool gammaCorrect = key_arg_or_default(gammaCorrect, &true_value)->to_bool() ? true : false;
	bool captureScreenPixels  = key_arg_or_default(captureScreenPixels, &false_value)->to_bool() ? true : false;
	HWND hWndDesktop = GetDesktopWindow();
	HWND hWnd = NULL;
	if (arg_list[0] == n_max)  
		hWnd = MAXScript_interface->GetMAXHWnd();
	else
		hWnd = (HWND)arg_list[0]->to_intptr();
	if (hWnd == 0)
		hWnd = hWndDesktop;

	if (hWnd == hWndDesktop)
		captureScreenPixels = false;  // capturing desktop, so meaningless

	if (IsWindow(hWnd)) 
	{
		int width = 0;
		int height = 0;
		int capture_offset_x = 0;
		int capture_offset_y = 0;
		if (hWnd == hWndDesktop)
		{
			width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
			height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		}
		else
		{
			WINDOWPLACEMENT wp;
			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wp);

			if (wp.showCmd != SW_SHOWMINIMIZED)
			{
				WINDOWINFO winfo;
				winfo.cbSize = sizeof(WINDOWINFO);
				GetWindowInfo(hWnd, &winfo);

				RECT rect;
				if (captureScreenPixels)
				{
					if (wp.showCmd == SW_SHOWMAXIMIZED) // no border
					{
						winfo.rcWindow.top += winfo.cyWindowBorders;
						winfo.rcWindow.bottom -= winfo.cyWindowBorders;
						winfo.rcWindow.left += winfo.cxWindowBorders;
						winfo.rcWindow.right -= winfo.cxWindowBorders;
						rect = winfo.rcWindow;
					}
					else  // border
					{
						GetWindowRect(hWnd, &rect);
					}
					capture_offset_x = rect.left;
					capture_offset_y = rect.top;
				}
				else // no border
				{
					winfo.rcWindow.top += winfo.cyWindowBorders;
					winfo.rcWindow.bottom -= winfo.cyWindowBorders;
					winfo.rcWindow.left += winfo.cxWindowBorders;
					winfo.rcWindow.right -= winfo.cxWindowBorders;
					rect = winfo.rcWindow;
					capture_offset_x = winfo.rcWindow.left - winfo.rcClient.left;
					capture_offset_y = winfo.rcWindow.top - winfo.rcClient.top;
				}
				width = (rect.right-rect.left);
				height = (rect.bottom-rect.top);
			}
		}
		int bufferSize =  width * height * sizeof(RGBQUAD);
		int dibSize = sizeof(BITMAPINFOHEADER) + bufferSize;

		BITMAPINFO *bmi = (BITMAPINFO *)malloc(dibSize);
		if (bmi == nullptr)
			return &undefined;
		memset(bmi, 0, dibSize);
		BYTE *buf = new BYTE[dibSize];
		if (buf == nullptr)
		{
			free(bmi);
			return &undefined;
		}
		
		HWND hWndToCaptureFrom = hWnd;
		int capture_width = width;
		int capture_height = height;
		if (captureScreenPixels)
		{
			hWndToCaptureFrom = hWndDesktop;
			capture_width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
			capture_height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		}
		HDC hdc = GetDC(hWndToCaptureFrom);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, capture_width, capture_height);
		HDC hdcMem = CreateCompatibleDC(hdc);
		HGDIOBJ hOldB = SelectObject(hdcMem, hBitmap);
		BitBlt(hdcMem, 0, 0, width, height, hdc, capture_offset_x, capture_offset_y, SRCCOPY);

		BITMAPINFOHEADER *bmih = &(bmi->bmiHeader);
		bmih->biSize = sizeof(BITMAPINFOHEADER);
		bmih->biWidth = width;
		bmih->biHeight = height;
		bmih->biPlanes = 1;
		bmih->biBitCount = 32;
		bmih->biCompression = BI_RGB;
		bmih->biSizeImage = bufferSize;
		bmih->biXPelsPerMeter = GetDeviceCaps(hdc, LOGPIXELSX);
		bmih->biYPelsPerMeter = GetDeviceCaps(hdc, LOGPIXELSY);
		bmih->biClrUsed = 0;
		bmih->biClrImportant = 0;

		if (!GetDIBits(hdcMem, hBitmap, 0, height, &buf[sizeof(BITMAPINFOHEADER)], bmi, DIB_RGB_COLORS))
		{
			delete [] buf;
			free(bmi);
			DeleteDC(hdcMem);
			ReleaseDC(hWnd, hdc);
			DeleteObject(hBitmap);
			return &undefined;
		}			
		memcpy(buf, bmih, sizeof(BITMAPINFOHEADER));

		Bitmap *map = CreateBitmapFromBInfo((void**)&buf, width, height, captureAlpha);

		delete [] buf;
		free(bmi);
		DeleteDC(hdcMem);
		ReleaseDC(hWnd, hdc);
		DeleteObject(hBitmap);

		if (gammaCorrect) 
			DeGammaCorrectBitmap(map);

		one_typed_value_local(MAXBitMap* mbm);
		vl.mbm = new MAXBitMap ();
		vl.mbm->bi.CopyImageInfo(&map->Storage()->bi);
		vl.mbm->bi.SetFirstFrame(0);
		vl.mbm->bi.SetLastFrame(0);
		vl.mbm->bi.SetName(_T(""));
		vl.mbm->bi.SetDevice(_T(""));
		if (vl.mbm->bi.Type() > BMM_TRUE_64)
			vl.mbm->bi.SetType(BMM_TRUE_64);
		vl.mbm->bm = map;
		return_value(vl.mbm);
	}
	else
		return &undefined;
}
