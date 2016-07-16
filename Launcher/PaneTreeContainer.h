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
        SRenderContext *pRenderContext = 0);

    ~CTreePaneContainer();

    void SetEditor(std::shared_ptr<editors::IEditor> &editor);

    void SetRenderContext(SRenderContext *pRenderContext);

    LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnAppInsertObject(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnAppRemoveObject(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnAppRenameObject(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnAppOnNotify(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&pResult);

    void Update(const T_CLASS *pSender, ESystemEventID EventId);

    LRESULT OnTreeViewSelected(WORD, WORD, HWND, BOOL&);

    void OnObjectSelected();

    bool AddAspectTab(const char *name, T_CLASS *aspect, Win32ObjectBrowserWidget<T_CLASS> *presenter);

    LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHendled);

private:
    CTabCtrl    m_pTabCtrl;

public:
	Win32ObjectBrowserWidget<T_CLASS> *m_pTreeBrowser;

private:
	pContextMenuFunction	m_pfnContextMenu;
	pContextMenuProcessor	m_pfnContextMenuProcessor;
	pGetResourceIconIndex	m_pfnGetResourceIconIndex;

	CALLBACK_FN				m_pfnInvokeObject;
	CALLBACK_FN				m_pfnDirectInvokeObject;
	CALLBACK_FN				m_pfnClearObject;
	CALLBACK_FN				m_pfnDirectClearObject;
	CALLBACK_EV				m_pfnOnEventUpdate;

	HIMAGELIST m_hImageList;

public:
	class SRenderContext *m_pRenderContext;
private:
    std::shared_ptr<editors::IEditor> m_editor;

    std::map<T_CLASS*, Win32ObjectBrowserWidget<T_CLASS>*> m_aspectsView;
};

#include "PaneTreeContainer.ipp"

#endif//__panetreecontainer_h__