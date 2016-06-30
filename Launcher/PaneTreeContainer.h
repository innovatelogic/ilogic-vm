#ifndef __panetreecontainer_h__
#define __panetreecontainer_h__

#pragma once

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template<class T_CLASS>
class CTreePaneContainer : public CPaneContainer
{
	typedef typename void (*pContextMenuFunction)(HWND hwnd, POINT pt, T_CLASS *pActor);
	typedef typename bool (*pContextMenuProcessor)(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	typedef typename int  (*pGetResourceIconIndex)(const std::string &TypeName);
	typedef typename int  (*CALLBACK_FN) (const T_CLASS *pSender);
	typedef typename void (*CALLBACK_EV)(const T_CLASS *pSender, ESystemEventID EventId);

public:
	BEGIN_MSG_MAP(CTreePaneContainer)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_USER_INSERTOBJECT, OnAppInsertObject)
		MESSAGE_HANDLER(WM_USER_REMOVEOBJECT_BRWSR, OnAppRemoveObject)
		MESSAGE_HANDLER(WM_USER_RENAMEOBJECT_BRWSR, OnAppRenameObject)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_NOTIFY, OnAppOnNotify)
		CHAIN_MSG_MAP(CPaneContainer)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	//----------------------------------------------------------------------------------------------
	CTreePaneContainer(
		pContextMenuFunction pfMenu,
		pContextMenuProcessor pfnMenuProcessor,
		pGetResourceIconIndex pfnGetResourceIconIndex,
		CALLBACK_FN pfnInvokeObject,
		CALLBACK_FN pfnDirectInvokeObject,
		CALLBACK_FN pfnClearObject,
		CALLBACK_FN pfnDirectClearObject,
		CALLBACK_EV pfnOnEventUpdate,
		HIMAGELIST hImageList,
		SRenderContext *pRenderContext = 0)
		: m_pfnContextMenu(pfMenu)
		, m_pfnContextMenuProcessor(pfnMenuProcessor)
		, m_pfnGetResourceIconIndex(pfnGetResourceIconIndex)
		, m_pfnInvokeObject(pfnInvokeObject)
		, m_pfnDirectInvokeObject(pfnDirectInvokeObject)
		, m_pfnClearObject(pfnClearObject)
		, m_pfnDirectClearObject(pfnDirectClearObject)
		, m_pfnOnEventUpdate(pfnOnEventUpdate)
		, m_hImageList(hImageList)
		, m_pRenderContext(pRenderContext)
        , m_editor(nullptr)
        , m_pTreeBrowser(nullptr)
	{

	}

	//----------------------------------------------------------------------------------------------
	~CTreePaneContainer()
	{
		delete m_pTreeBrowser;
	}

    //----------------------------------------------------------------------------------------------
    void SetEditor(std::shared_ptr<editors::IEditor> &editor) 
    { 
        m_editor = editor; 
        m_pTreeBrowser->SetEditor(m_editor);
    }

	//----------------------------------------------------------------------------------------------
	void SetRenderContext(SRenderContext *pRenderContext)
	{
		m_pRenderContext = pRenderContext;

		assert(m_pTreeBrowser);

		m_pTreeBrowser->SetRenderContext(pRenderContext);
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&)
	{
        RECT rcDefault = { 0, 25, 200, 200 };
        m_pTabCtrl.Create(m_hWnd, rcDefault, 0, WS_CHILD | WS_VISIBLE);

        m_pTreeBrowser = new Win32ObjectBrowserWidget<T_CLASS>(
            m_hWnd,
            m_pfnContextMenu,
            m_pfnContextMenuProcessor,
            m_pfnGetResourceIconIndex,
            m_pfnInvokeObject,
            m_pfnDirectInvokeObject,
            m_pfnClearObject,
            m_pfnDirectClearObject,
            m_hImageList,
            m_pRenderContext);

        // assign the edit to the bottom container
        SetClient(m_pTreeBrowser->GetHWndTree());

        AddTabPage("Aspect", 0); // add by default

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnAppInsertObject(UINT, WPARAM, LPARAM, BOOL&)
	{
		m_pTreeBrowser->WndProcessInsertObject();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnAppRemoveObject(UINT, WPARAM, LPARAM, BOOL&)
	{
		m_pTreeBrowser->WndRemoveObject();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnAppRenameObject(UINT, WPARAM, LPARAM, BOOL&)
	{
		m_pTreeBrowser->WndRenameObject();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnAppOnNotify(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&pResult)
	{
		// select item handler
        if (((LPNMHDR)lParam)->code == TVN_ITEMSELECTING)
        {
            return 0;
        }
        else if (((LPNMHDR)lParam)->code == TVN_ITEMSELECTED)
		{
            const NMTREEVIEW *nmt = (NMTREEVIEW*)(lParam);
            if (nmt->itemNew.state == TVIS_SELECTED)
            {
                bool bSelChanged = m_pTreeBrowser->SelChangedTreeObject();

                //if (!bSelChanged){
                //	m_pTreeBrowser->Update(0, Event_OnSelected);
                //}
            }
            return 0;
		}
		/*else if (((LPNMHDR)lParam)->code == NM_RCLICK) // context menu
		{	
			m_pTreeBrowser->ProcessRightClick();
		}*/
		return CPaneContainer::OnNotify(iunt_, wParam, lParam, pResult);
	}

	//----------------------------------------------------------------------------------------------
	void Update(const T_CLASS *pSender, ESystemEventID EventId)
	{
        if (m_pTreeBrowser)
        {
            m_pTreeBrowser->Update(pSender, EventId);
        }
	}

    //----------------------------------------------------------------------------------------------
    LRESULT OnTreeViewSelected(WORD, WORD, HWND, BOOL&)
    {
        return 0;
    }

    //----------------------------------------------------------------------------------------------
    void OnObjectSelected()
    {
        m_pTreeBrowser->OnNotifySelected();
    }

    //----------------------------------------------------------------------------------------------
    bool AddTabPage(const char *name, int index /*= 0*/)
    {
        wchar_t wbuf[256] = { 0 }; // temp buffers
        char ascii[256] = { 0 };

        MultiByteToWideChar(CP_ACP, 0, name, INDEX_NONE, wbuf, 256);

        TCITEM tc = { 0 };
        tc.mask = TCIF_TEXT;
        tc.pszText = wbuf;
        m_pTabCtrl.InsertItem(index, &tc);

        return true;
    }

    //----------------------------------------------------------------------------------------------
    LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHendled)
    {
        const int tabHeight = 40;

        RECT rc;
        GetClientRect(&rc);

        const int width = rc.right - rc.top;
        const int height = rc.bottom - rc.top;

        m_pTabCtrl.MoveWindow(0, 0, width, tabHeight);
        m_pTreeBrowser->MoveWindow(0, tabHeight, width, height - tabHeight);

        bHendled = TRUE;
        return 0;
    }

private:
    CTabCtrl    m_pTabCtrl;
	Win32ObjectBrowserWidget<T_CLASS> *m_pTreeBrowser;

	pContextMenuFunction	m_pfnContextMenu;
	pContextMenuProcessor	m_pfnContextMenuProcessor;
	pGetResourceIconIndex	m_pfnGetResourceIconIndex;

	CALLBACK_FN				m_pfnInvokeObject;
	CALLBACK_FN				m_pfnDirectInvokeObject;
	CALLBACK_FN				m_pfnClearObject;
	CALLBACK_FN				m_pfnDirectClearObject;
	CALLBACK_EV				m_pfnOnEventUpdate;

	HIMAGELIST m_hImageList;

	class SRenderContext *m_pRenderContext;

    std::shared_ptr<editors::IEditor> m_editor;
};

#endif//__panetreecontainer_h__