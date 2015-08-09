#include "LauncherStdAfx.h"

#define ID_EDITCHILD_PROGRESS					12101

#define PROGRESS_WIDTH							900
#define PROGRESS_HEIGHT							150

namespace dialog_threadmonitor
{	
	const CWindowManager *pWindowManager = NULL;
	HWND hwndDialogThreadMonitor   = NULL;

	int threadUpdateID = 0;
	int threadRenderID = 0;
	int threadTimerID  = 0;
	int threadStreamID = 0;

	HINSTANCE gInstance;
}
using namespace dialog_threadmonitor;

//------------------------------------------------------------------------
void CWin32ThreadMonitor::Initialize(HWND hWndParent, const CWindowManager *WMgr, int UpdateID, int RenderID, int TimerID, int StreamID)
{
	HWND hWnd = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG_THREADMONITOR), hWndParent, (DLGPROC)CWin32ThreadMonitor::DlgProc);

	if (hWnd != NULL)
	{
		//ShowWindow(updateB, SW_SHOW);

		hwndDialogThreadMonitor = hWnd;
		pWindowManager = WMgr;
		
		threadUpdateID = UpdateID;
		threadRenderID = RenderID;
		threadTimerID  = TimerID;
		threadStreamID = StreamID;
	}
}

//------------------------------------------------------------------------
void CWin32ThreadMonitor::RegisterUpdateboxClass() 
{
	// Post: Register custom class
	WNDCLASSEX wc;

	wc.cbSize        = sizeof(wc);  
	wc.lpszClassName = L"UpdateBox";
	wc.hInstance     = GetModuleHandle(NULL);
	wc.lpfnWndProc   = UpdateBoxWndProc;
	wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
	wc.hIcon         = NULL;
	wc.lpszMenuName  = NULL;
	wc.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);
	wc.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = sizeof(UpdateBox*);
	wc.hIconSm       = NULL;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Update Box Class Registration Failed", L"Error", MB_ICONEXCLAMATION | MB_OK);
	}  
}
//------------------------------------------------------------------------
LRESULT CWin32ThreadMonitor::UpdateBoxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//UpdateBox *udb = (UpdateBox*)GetWindowLong(hWnd, 0);

	switch (message) 
	{
	case WM_NCCREATE:
		{
			//udb = (UpdateBox*)malloc(sizeof(UpdateBox));

// 			if (udb == false) 
// 			{
// 				MessageBox(NULL, L"Failed to create Update Box window", L"Error",MB_OK|MB_ICONERROR);
// 				return 0;
// 			}
// 			SetWindowLong(hWnd, 0, (LONG)udb); // Attach custom class to this window.
		}
		break;

	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);

			// fill background
			HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
			RECT back_rct;
			back_rct.left = 0;
			back_rct.right = PROGRESS_WIDTH;
			back_rct.top = 0;
			back_rct.bottom = PROGRESS_HEIGHT;
			::FillRect(hdc, &back_rct, whiteBrush);

			// top rule
			HBRUSH blueBrush = CreateSolidBrush(RGB(200,200,200));
			RECT rct;

			rct.left = 0;
			rct.right = PROGRESS_WIDTH;
			rct.top = 0;
			rct.bottom = 15;

			::FillRect(hdc, &rct, blueBrush);

			//TextOut(hdc,10,10, L"Hello World", 11);

			EndPaint(hWnd, &ps);
		}    
		break;

	case WM_NCDESTROY:
		//free(udb);
		break;

	default:
		break;
	}
	return DefWindowProc (hWnd, message, wParam, lParam);
}

//------------------------------------------------------------------------
void CWin32ThreadMonitor::ShowDialogWindow(bool bShow /*= true*/)
{
	if (hwndDialogThreadMonitor)
	{
		ShowWindow(hwndDialogThreadMonitor, bShow ? SW_SHOW : SW_HIDE);
	}
}

//------------------------------------------------------------------------
bool CWin32ThreadMonitor::IsVisible()
{
	if (hwndDialogThreadMonitor)
	{
		return IsWindowVisible(hwndDialogThreadMonitor) ? true : false;
	}
	return false;
}

//------------------------------------------------------------------------
BOOL CWin32ThreadMonitor::DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_INITDIALOG:
		{
			RegisterUpdateboxClass();

			HWND updateB = CreateWindow(L"UpdateBox",
				NULL,
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				10, 170, 
				PROGRESS_WIDTH, PROGRESS_HEIGHT,
				hwndDlg,
				(HMENU)ID_EDITCHILD_PROGRESS,
				NULL,
				NULL);

			if (!updateB) 
			{
				MessageBox(hwndDlg, L"Failed to create Update Box Control.", L"Error", MB_OK | MB_ICONERROR);
			}

			bool IsOneThread = IsThreadsSerialized();
			SendDlgItemMessage(hwndDialogThreadMonitor, IDC_CHECK_THREADMODE, IsOneThread ? BM_SETCHECK : BST_UNCHECKED, 0, 0);
		}break;

	case WM_NOTIFY:
		{
		}break;
	
	case WM_COMMAND:
	case BN_CLICKED:
		{
			if (LOWORD(wParam) == IDC_CHECK_THREADMODE)
			{
#ifdef USE_THREADS
				bool isSerial = IsThreadsSerialized();

				// change all threads to opposite status
				ThreadManagerSerial &tm = ThreadManagerSerial::getThreadManagerSerial();
				tm.setThreadSerialized(threadUpdateID, !isSerial);
				tm.setThreadSerialized(threadRenderID, !isSerial);
				tm.setThreadSerialized(threadTimerID, !isSerial);
				tm.setThreadSerialized(threadStreamID, !isSerial);
#endif
			}
		}break;

	case WM_CLOSE:
		{
			ShowDialogWindow(!IsVisible());
		//	pWindowManager->UpdateFlagsState();
		}break;
	} 
	return FALSE; 
}

//------------------------------------------------------------------------
bool CWin32ThreadMonitor::IsThreadsSerialized()
{
	// just check the update thread
	ThreadManagerSerial &tm = ThreadManagerSerial::getThreadManagerSerial();
	return tm.isThreadSerialized(0);
}

//------------------------------------------------------------------------
void CWin32ThreadMonitor::SetCPS_Main(unsigned int cps)
{
	HWND _hwndEditCtrl = ::GetDlgItem(hwndDialogThreadMonitor, IDC_STATIC_CPS_MAIN);

	if (IsVisible() && _hwndEditCtrl)
	{
		char ascii[256] = {0};
		itoa(cps, ascii, 10);
		::SetWindowText(_hwndEditCtrl, ConvertStringToWideString(ascii).c_str());
	}
}

//------------------------------------------------------------------------
void CWin32ThreadMonitor::SetCPS_Render(unsigned int cps)
{
	HWND _hwndEditCtrl = ::GetDlgItem(hwndDialogThreadMonitor, IDC_STATIC_CPS_RENDER);

	if (IsVisible() && _hwndEditCtrl)
	{
		char ascii[256] = {0};
		itoa (cps, ascii, 10);
		::SetWindowText(_hwndEditCtrl, ConvertStringToWideString(ascii).c_str());
	}
}

//------------------------------------------------------------------------
void CWin32ThreadMonitor::SetCPS_Update(unsigned int cps)
{
	HWND _hwndEditCtrl = ::GetDlgItem(hwndDialogThreadMonitor, IDC_STATIC_CPS_UPDATE);

	if (IsVisible() && _hwndEditCtrl)
	{
		char ascii[256] = {0};
		itoa (cps, ascii, 10);
		::SetWindowText(_hwndEditCtrl, ConvertStringToWideString(ascii).c_str());
	}
}

//------------------------------------------------------------------------
void CWin32ThreadMonitor::SetCPS_Timer(unsigned int cps)
{
	HWND _hwndEditCtrl = ::GetDlgItem(hwndDialogThreadMonitor, IDC_STATIC_CPS_TIMER);

	if (IsVisible() && _hwndEditCtrl)
	{
		char ascii[256] = {0};
		itoa (cps, ascii, 10);
		::SetWindowText(_hwndEditCtrl, ConvertStringToWideString(ascii).c_str());
	}
}

//------------------------------------------------------------------------
//	UpdateBox implementation
//------------------------------------------------------------------------
UpdateBox::UpdateBox()
{

}
