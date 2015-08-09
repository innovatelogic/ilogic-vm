#ifndef __listveiwcombobox_h__
#define __listviewcombobox_h__

#pragma once

typedef CWinTraitsOR<WS_CHILD | WS_VISIBLE, WS_EX_CLIENTEDGE>	CListViewComboBoxTraits;

template<class CLASS_T>
class CListViewComboBox : public CWindowImpl<CListViewComboBox<CLASS_T>, CListViewCtrl, CListViewComboBoxTraits>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, WC_LISTVIEW)

	BEGIN_MSG_MAP(CListViewComboBox)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_MOVE, OnMove)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	//----------------------------------------------------------------------------------------------
	CListViewComboBox(CLASS_T *pImpl)
		: m_pImpl(pImpl)
		, m_FirstShow(TRUE)
	{
		m_pButton = new CButton();
	}

	//----------------------------------------------------------------------------------------------
	~CListViewComboBox()
	{
		SAFE_DELETE(m_pButton);
	}

	CButton* GetButton() { return m_pButton; }

	//----------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------
	// Used with Views: Overridden create function sets required listview styles
	HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
		DWORD dwStyle = 0, DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, 
		LPVOID lpCreateParam = NULL)
	{
		dwStyle |= LVS_REPORT; // "Details" mode
		dwStyle |= LVS_SHOWSELALWAYS;
		dwStyle |= LVS_SINGLESEL;
		dwStyle |= LVS_NOSCROLL;

		return CWindowImpl<CListViewComboBox<CLASS_T>, CListViewCtrl, CListViewComboBoxTraits>::Create(hWndParent,
			rect.m_lpRect, szWindowName, dwStyle, dwExStyle, MenuOrID.m_hMenu,
			lpCreateParam);
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		DWORD dwExStyle = 0;
		dwExStyle |= LVS_EX_GRIDLINES;
		dwExStyle |= LVS_EX_FULLROWSELECT;
		dwExStyle |= LVS_EX_DOUBLEBUFFER; // reduces flicker
		SetExtendedListViewStyle(dwExStyle);
		
		DWORD dwStyle = GetWindowLong(GWL_STYLE);
		dwStyle |= LVS_NOCOLUMNHEADER;
		//dwStyle |= ;~(WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
		SetWindowLong(GWL_STYLE, dwStyle);

		AddColumn(TEXT("Value"), 0);

		m_pButton->Create(GetParent(), rcDefault, _T("Button"), WS_CHILD | WS_VISIBLE);
		m_pButton->SetFont(GetFont());
		m_pButton->SetWindowTextW(_T("..."));

		bHandled = FALSE;
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnMove(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		RECT wrect;
		GetWindowRect(&wrect);
		
		RECT rect;
		GetClientRect(&rect);
		
		int width = rect.right - rect.left;
		int Height = rect.bottom - rect.top;
		
		SetColumnWidth(0, width);

		RECT irc;
		GetItemRect(0, &irc, 0);
		int iheight = irc.bottom - irc.top;

	/*	RECT btn_rect;
		btn_rect.left = (rect.left + width) - iheight;
		btn_rect.top = rect.top - iheight;
		btn_rect.right = btn_rect.left + iheight;
		btn_rect.bottom = btn_rect.top + iheight;
		
		m_pButton->MoveWindow(&btn_rect);*/

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		SetRedraw(TRUE);

		EnsureVisible(0, TRUE); // Make sure top row shows

		m_pButton->ShowWindow((BOOL)wParam);

		return 0;
	}

private:
	CButton *m_pButton;

	BOOL m_FirstShow;	
	
	CLASS_T *m_pImpl;
};


#endif//__listviewcombobox_h__