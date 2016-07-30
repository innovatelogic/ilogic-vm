#ifndef __panepropertycontainer_h__
#define __panepropertycontainer_h__

#pragma once

#include "ToolBarHelper.h"
#include "WTLPropertyGrid.h"

#define ID_BUTTON_CATEGORIZED_A				983
#define ID_BUTTON_SORTED					984

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template<class T_CLASS>
class CPanePropertyContainer : public CPaneContainer
{
	template<typename U>
	class CToolbarContainer : public CWindowImpl<CToolbarContainer<U>>
	{
	public:
		BEGIN_MSG_MAP(CToolbarContainer)
			MESSAGE_HANDLER(WM_CREATE, OnCreate)
			MESSAGE_HANDLER(WM_COMMAND, OnLBClick)
			REFLECT_NOTIFICATIONS()
		END_MSG_MAP()

        CToolbarContainer(CPanePropertyContainer<T_CLASS> *pOwner);
        LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
        LRESULT OnLBClick(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	private:
		CPanePropertyContainer<T_CLASS>  *m_pOwner;
	};

public:
	BEGIN_MSG_MAP(CPanePropertyContainer)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		//MESSAGE_HANDLER(WM_USER_DELETE_ALL, OnDeleteAll)
		MESSAGE_HANDLER(WM_USER_PROPS_FILLED, OnFilledProps)
		MESSAGE_HANDLER(WM_USER_UPDATE_PROPS, OnUpdateProps)
		CHAIN_MSG_MAP(CPaneContainer)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	//----------------------------------------------------------------------------------------------
	CPanePropertyContainer()
	{
		m_pPropGrid = new CWTLPropertyGrid<T_CLASS>();
		m_pMenuCtrl = new CToolbarContainer<T_CLASS>(this);
	}

	//----------------------------------------------------------------------------------------------
	~CPanePropertyContainer()
	{
		SAFE_DELETE(m_pPropGrid);
		SAFE_DELETE(m_pMenuCtrl);
	}

    //----------------------------------------------------------------------------------------------
    void SetAppMain(CCoreSDK *app) 
    { 
        m_pPropGrid->SetAppMain(app);
    }

	//----------------------------------------------------------------------------------------------
	void SetRenderContext(SRenderContext *pContext) 
	{
		assert(m_pPropGrid);

		m_pPropGrid->SetRenderContext(pContext);
	}

	//----------------------------------------------------------------------------------------------
	void Update(const T_CLASS *pSender, ESystemEventID EventId)
	{
		assert(m_pPropGrid);

		m_pPropGrid->_Update(pSender, EventId);
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&)
	{
		RECT rcDefault = {0, 25, 200, 200};

		m_pMenuCtrl->Create(m_hWnd, rcDefault, 0, WS_CHILD | WS_VISIBLE);
		m_pTabCtrl.Create(m_hWnd, rcDefault, 0, WS_CHILD | WS_VISIBLE);
		m_pPropGrid->Create(m_hWnd, rcDefault);
		
		//SetClient(m_pPropGrid->m_hWnd);
	
	//	m_pMenuCtrl.SetFont(m_pPropGrid->GetFont());
		m_pTabCtrl.SetFont(m_pPropGrid->GetFont());

		AddTabPage("Value", 0); // add by default

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnNotify(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
	{
		switch (((LPNMHDR)lParam)->code)
		{
		// process fill list
		case LVN_GETDISPINFO:
			{
				if (((LPNMHDR)lParam)->hwndFrom == m_pPropGrid->m_hWnd)
				{
					LPNMLVDISPINFOA plvdi = (LPNMLVDISPINFOA)lParam;
					LVITEMA *pItem = &(((LPNMLVDISPINFOA)lParam)->item);

					m_pPropGrid->GETDISPINFO_FillList(pItem);
					bHandled = true;
				}
			}break;

		case NM_CUSTOMDRAW:
			{
				if (((LPNMHDR)lParam)->hwndFrom == m_pPropGrid->m_hWnd)
				{
					LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;
					return m_pPropGrid->HandleCustomDraw(lplvcd);
				}
			}break;

		case NM_CLICK:
			{
				if (((LPNMHDR)lParam)->hwndFrom == m_pPropGrid->m_hWnd){
					m_pPropGrid->ClickListProperties((LPNMLISTVIEW)lParam);
				} 
				else if (((LPNMHDR)lParam)->hwndFrom == m_pTabCtrl.m_hWnd)
				{
					int selectedGroup = m_pTabCtrl.GetCurSel();

					m_pPropGrid->SetSelectedGroup(selectedGroup);

					m_pPropGrid->DeleteAllItems();

					m_pPropGrid->FillListProperties();
				}
			}break;
		};
		bHandled = FALSE;
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHendled)
	{
		const int tabHeight = 40;

		RECT rc;
		GetClientRect(&rc);

		int width = rc.right - rc.top;
		int height = rc.bottom - rc.top;

		m_pMenuCtrl->MoveWindow(0, 0, width, tabHeight);
		m_pTabCtrl.MoveWindow(0, tabHeight, width, tabHeight);
		m_pPropGrid->MoveWindow(0, tabHeight * 2, width, height - (tabHeight * 2));

		m_pPropGrid->AdjustColumnsSize();

		bHendled = FALSE;
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnDeleteAll(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		ClearTabPages();

		m_pPropGrid->ClearListProperties();
		m_pPropGrid->HideChildControls();

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnFilledProps(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		ClearTabPages();

		const TVecPropertyGroup &groups = m_pPropGrid->GetPropertyGroups();

		int IndexGroup = 0;
		TVecPropertyGroupConstIter IterGroup = groups.begin();
		while (IterGroup != groups.end())
		{
			AddTabPage((*IterGroup)->GroupName.c_str(), IndexGroup++);
			++IterGroup;
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnUpdateProps(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		m_pPropGrid->UpdatePreview();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	bool AddTabPage(const char *name, int Index = 0)
	{
		wchar_t wbuf[256] = {0}; // temp buffers
		char ascii[256] = {0};

		MultiByteToWideChar(CP_ACP, 0, name, INDEX_NONE, wbuf, 256);

		TCITEM tc = {0};
		tc.mask = TCIF_TEXT;
		tc.pszText = wbuf;
		m_pTabCtrl.InsertItem(Index, &tc);

		return true;
	}

	//----------------------------------------------------------------------------------------------
	bool ClearTabPages()
	{
		m_pTabCtrl.DeleteAllItems();
		return true;
	}

	//----------------------------------------------------------------------------------------------
	void SetGridViewStyle(EGridView Value)
	{
		m_pPropGrid->SetGridViewStyle(Value);
	}

protected:
public:
	CToolbarContainer<T_CLASS> *m_pMenuCtrl;
	CTabCtrl m_pTabCtrl;

	CToolBarCtrl m_ToolBar;
	class CWTLPropertyGrid<T_CLASS> *m_pPropGrid;
};

#include "pane_property_container.ipp"
#endif//__panepropertycontainer_h__