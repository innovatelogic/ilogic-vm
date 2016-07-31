#include "PaneTreeContainer.h"

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
CTreePaneContainer<T_CLASS>::CTreePaneContainer(
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
    , m_PointSize(90)
    , m_TypeFace(_T("MS Shell Dlg 2"))
{

}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
CTreePaneContainer<T_CLASS>::~CTreePaneContainer()
{
    for each (auto pair in m_aspectsView)
    {
        delete pair.second;
    }
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void CTreePaneContainer<T_CLASS>::SetEditor(std::shared_ptr<editors::IEditor> &editor)
{
    m_editor = editor;

    AddAspectTab("Main", m_editor->GetApp()->GetExplorerInstance()->GetExplorer3D());
    AddAspectTab("Flat", m_editor->GetApp()->GetExplorerInstance()->GetExplorer2D());

    ToggleAspectView(0);
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void CTreePaneContainer<T_CLASS>::SetRenderContext(SRenderContext *pRenderContext)
{
    m_pRenderContext = pRenderContext;

    for each (auto iter in m_aspectsView)
    {
        iter.second->SetRenderContext(pRenderContext);
    }
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
LRESULT CTreePaneContainer<T_CLASS>::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
    RECT rcDefault = { 0, 25, 200, 200 };
    m_pTabCtrl.Create(m_hWnd, rcDefault, 0, WS_CHILD | WS_VISIBLE);

    // Create the listview and edit control font
    m_Font.CreatePointFont(m_PointSize, m_TypeFace);

    m_pTabCtrl.SetFont(m_Font);

    // assign the edit to the bottom container
//    SetClient(m_pTreeBrowser->GetHWndTree());

    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
LRESULT CTreePaneContainer<T_CLASS>::OnAppInsertObject(UINT, WPARAM, LPARAM, BOOL&)
{
    for each (auto iter in m_aspectsView)
    {
        iter.second->WndProcessInsertObject();
    }
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
LRESULT CTreePaneContainer<T_CLASS>::OnAppRemoveObject(UINT, WPARAM, LPARAM, BOOL&)
{
    for each (auto iter in m_aspectsView)
    {
        iter.second->WndRemoveObject();
    }
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
LRESULT CTreePaneContainer<T_CLASS>::OnAppRenameObject(UINT, WPARAM, LPARAM, BOOL&)
{
    for each (auto iter in m_aspectsView)
    {
        iter.second->WndRenameObject();
    }
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
LRESULT CTreePaneContainer<T_CLASS>::OnAppOnNotify(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&pResult)
{
    // select item handler
    if (((LPNMHDR)lParam)->code == TVN_ITEMSELECTING)
    {
        return 0;
    }
    else if (((LPNMHDR)lParam)->code == TVN_ITEMSELECTFINISHED)
    {
        HWND hwndFrom = ((LPNMHDR)lParam)->hwndFrom;

        Win32ObjectBrowserWidget<T_CLASS> *presenter = nullptr;
        for each(auto item in m_aspectsView)
        {
            if (hwndFrom == item.second->GetHWndTree())
            {
                presenter = item.second;
                break;
            }
        }

        assert(presenter);

        const NMTREEVIEW *nmt = (NMTREEVIEW*)(lParam);
//        if (nmt->itemNew.state == TVIS_SELECTED)
        {
            presenter->SelChangedTreeObject(); // TODO: inc-decremental selection
        }
        return 0;

    }
    else if (((LPNMHDR)lParam)->code == NM_CLICK)
    {
        if (((LPNMHDR)lParam)->hwndFrom == m_pTabCtrl.m_hWnd)
        {
            int selectedGroup = m_pTabCtrl.GetCurSel();

            ToggleAspectView(selectedGroup);
        }
    }

    /*else if (((LPNMHDR)lParam)->code == NM_RCLICK) // context menu
    {
    m_pTreeBrowser->ProcessRightClick();
    }*/
    return CPaneContainer::OnNotify(iunt_, wParam, lParam, pResult);
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
LRESULT CTreePaneContainer<T_CLASS>::OnTreeViewSelected(WORD, WORD, HWND, BOOL&)
{
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void CTreePaneContainer<T_CLASS>::OnObjectSelected()
{
    for each (auto iter in m_aspectsView)
    {
        iter.second->OnNotifySelected();
    }
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
bool CTreePaneContainer<T_CLASS>::AddAspectTab(const char *name, T_CLASS *aspect)
{
    bool bResult = false;

    if (name)
    {
        wchar_t wbuf[256] = { 0 }; // temp buffers
        char ascii[256] = { 0 };

        MultiByteToWideChar(CP_ACP, 0, name, INDEX_NONE, wbuf, 256);

        TCITEM tc = { 0 };
        tc.mask = TCIF_TEXT;
        tc.pszText = wbuf;
        m_pTabCtrl.AddItem(&tc);

        Win32ObjectBrowserWidget<T_CLASS> *presenter = new Win32ObjectBrowserWidget<T_CLASS>(
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

        presenter->SetEditor(m_editor);
        presenter->SetRoot(aspect);

        m_aspectsView.insert(std::make_pair(aspect, presenter));
        bResult = true;
    }
    return true;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
LRESULT CTreePaneContainer<T_CLASS>::OnSize(UINT, WPARAM, LPARAM, BOOL& bHendled)
{
    const int tabHeight = 40;

    RECT rc;
    GetClientRect(&rc);

    const int width = rc.right - rc.top;
    const int height = rc.bottom - rc.top;

    m_pTabCtrl.MoveWindow(0, 0, width, tabHeight);

    for each (auto iter in m_aspectsView)
    {
        iter.second->MoveWindow(0, tabHeight, width, height - tabHeight);
    }

    bHendled = TRUE;
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void CTreePaneContainer<T_CLASS>::LockModel(bool flag)
{
    for each (auto iter in m_aspectsView)
    {
        iter.second->LockModel(flag);
    }
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void CTreePaneContainer<T_CLASS>::FillModel()
{
    for each (auto iter in m_aspectsView)
    {
        iter.second->FillModel();
    }
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
bool CTreePaneContainer<T_CLASS>::ToggleAspectView(size_t index)
{
    if (index >= m_aspectsView.size()) {
        return false;
    }

    m_pTabCtrl.SetCurSel(index);

    size_t counter = 0;

    for each (auto iter in m_aspectsView)
    {
        ::ShowWindow(iter.second->GetHWndTree(), counter == index);
        ++counter;
    }
}