#ifndef __03022006__WIN32INPUTBOX__
#define __03022006__WIN32INPUTBOX__

#include <windows.h>
#include <tchar.h>

class CWin32InputBox;

// Structure used to orient the inputbox behavior
struct WIN32INPUTBOX_PARAM
{
	friend class CWin32InputBox;

	//
	IN OPTIONAL bool bMultiline;

	// Pass this as none zero so to use this memory dlg template
	IN OPTIONAL LPVOID DlgTemplateData;

	// Pass this as none ZERO so to load DLGTEMPLATE from resources
	IN OPTIONAL LPCTSTR DlgTemplateName;

	// passing both "DlgTemplateName" and "DlgTemplateData" ZERO will cause
	// the dialog to use his default embedded resource

	// Center on monitor or owner window?
	IN OPTIONAL bool bCenter;

	// Want to add more styles to the dialog?
	IN OPTIONAL DWORD dwStylesPlus, dwStylesMinus;
	IN OPTIONAL DWORD dwExStylesPlus, dwExStylesMinus;

	IN LPCTSTR szTitle, szPrompt;

	// Return buffer
	OUT LPTSTR szResult;
	IN DWORD nResultSize;

	// Owner window
	HWND hwndOwner;
	HINSTANCE hInstance;

	short xPos, yPos;

	WIN32INPUTBOX_PARAM();
private:
	HWND hDlg;
};

class CWin32InputBox
{
private:
	WIN32INPUTBOX_PARAM *_param;
	static LRESULT CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
	HWND _hwndEditCtrl;

	void InitDialog();
	void SetParam(WIN32INPUTBOX_PARAM *);
	WIN32INPUTBOX_PARAM * GetParam();

public:

	CWin32InputBox(WIN32INPUTBOX_PARAM *);
	~CWin32InputBox();

	static INT_PTR InputBoxEx(WIN32INPUTBOX_PARAM *);
	static INT_PTR InputBox(
		LPCTSTR szTitle, 
		LPCTSTR szPrompt, 
		LPTSTR szResult, 
		DWORD nResultSize,
		bool bMultiLine = false,
		HWND hwndParent = 0);
};

#endif