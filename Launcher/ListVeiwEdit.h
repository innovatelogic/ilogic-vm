#ifndef __listviewedit_h__
#define __listviewedit_h__

#pragma once

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T>
class CListViewEdit : public CWindowImpl<CListViewEdit<T>, CEdit>
{
public:
	BEGIN_MSG_MAP(CListViewEdit)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	CListViewEdit(T *pImpl)
		: m_pImpl(pImpl)
	{

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
	LRESULT EndLabelEdit(WPARAM wParam)
	{
		m_pImpl->OnLVEndLabelEdit(wParam);
		return 0;
	}

private:
	T *m_pImpl;
};


#endif//__listviewedit_h__