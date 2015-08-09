#ifndef __listvieweditresource_h__
#define __listvieweditresource_h__

#pragma once

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T>
class CListViewEditResource : public CWindowImpl<CListViewEditResource<T>, CEdit>
{
public:
	BEGIN_MSG_MAP(CListViewEdit)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		//CHAIN_MSG_MAP_ALT(CEditCommands<CListViewEdit>, 1)
	END_MSG_MAP()

	//----------------------------------------------------------------------------------------------
	CListViewEditResource(T *pImpl)
		: m_pImpl(pImpl)
	{
		m_pSiblinClearBtn = new CButton();
		m_pSiblinBrowseBtn = new CButton();
	}

	~CListViewEditResource()
	{
		SAFE_DELETE(m_pSiblinClearBtn);
		SAFE_DELETE(m_pSiblinBrowseBtn);
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		m_pSiblinClearBtn->Create(GetParent(), rcDefault, _T("Button"), WS_CHILD | WS_VISIBLE);
		m_pSiblinClearBtn->SetFont(GetFont());
		m_pSiblinClearBtn->SetWindowTextW(_T("X"));

		m_pSiblinBrowseBtn->Create(GetParent(), rcDefault, _T("Button"), WS_CHILD | WS_VISIBLE);
		m_pSiblinBrowseBtn->SetFont(GetFont());
		m_pSiblinBrowseBtn->SetWindowTextW(_T("..."));

		bHandled = FALSE;
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if (wParam == VK_TAB || wParam == VK_RETURN || wParam == VK_ESCAPE)
		{
			EndLabelEdit(wParam);
		}

		bHandled = FALSE;
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return EndLabelEdit(NULL);
	}

	//----------------------------------------------------------------------------------------------
	LRESULT EndLabelEdit(WPARAM wParam)
	{
		if (wParam == VK_RETURN)
		{
			m_pImpl->OnLVEndLabelEdit(wParam);
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		SetRedraw(TRUE);

		m_pSiblinClearBtn->ShowWindow((BOOL)wParam);
		m_pSiblinBrowseBtn->ShowWindow((BOOL)wParam);

		return 0;
	}

	CButton* GetButtonClear() { return m_pSiblinClearBtn; }
	CButton* GetButtonBrowse() { return m_pSiblinBrowseBtn; }

protected:
private:
	CButton *m_pSiblinClearBtn;
	CButton *m_pSiblinBrowseBtn;

	T *m_pImpl;
};


#endif//__listvieweditresource_h__