#include "ToolBarHelper.h"
#include "PaneTreeContainer.h"

//----------------------------------------------------------------------------------------------
template<class T>
template<typename U>
CPanePropertyContainer<T>::CToolbarContainer<U>::CToolbarContainer(CPanePropertyContainer<T> *pOwner)
    : m_pOwner(pOwner)
{
}

//----------------------------------------------------------------------------------------------
template<class T>
template<typename U>
LRESULT CPanePropertyContainer<T>::CToolbarContainer<U>::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
    CRect rc(0, 0, 100, 100);

    CImageList img;
    CToolBarCtrl m_navigationBar; //CCS_NODIVIDER|CCS_NOPARENTALIGN|CCS_NORESIZE|WS_CHILD|WS_VISIBLE|TBSTYLE_FLAT|TBSTYLE_TRANSPARENT|TBSTYLE_TOOLTIPS
    m_navigationBar.Create(m_hWnd, rc, NULL,
        CCS_NODIVIDER | CCS_NOPARENTALIGN | CCS_NORESIZE | WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT  | TBSTYLE_TOOLTIPS); //| TBSTYLE_TRANSPARENT 

    HIMAGELIST hBitmapProp = ::ImageList_LoadImage(::GetModuleHandle(NULL), L"resources/properties.bmp", 16, 4, CLR_NONE, IMAGE_BITMAP, LR_SHARED | LR_DEFAULTSIZE | LR_LOADFROMFILE);
    img.Attach(hBitmapProp);

    m_navigationBar.SetBitmapSize(16, 16);
    m_navigationBar.SetButtonSize(20, 20);
    m_navigationBar.SetImageList(img);

    TBBUTTON CustomButtons[] =
    {
        { 0, ID_BUTTON_CATEGORIZED_A, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0, 0, 0 },
        { 1, ID_BUTTON_SORTED, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0, 0, 0 },
        { -1, 0, 0, TBSTYLE_SEP, 0L, 0, 0, 0 },
    };

    m_navigationBar.AddButtons(2, CustomButtons);

    img.Detach();
    
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
template<typename U>
LRESULT CPanePropertyContainer<T>::CToolbarContainer<U>::OnLBClick(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    INT_PTR buttonId = LOWORD(wParam);
    if (buttonId == ID_BUTTON_CATEGORIZED_A)
    {
        m_pOwner->SetGridViewStyle(EGV_Categorized);
    }
    else if (buttonId == ID_BUTTON_SORTED)
    {
        m_pOwner->SetGridViewStyle(EGV_SortByName);
    }
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
CPanePropertyContainer<T>::CPanePropertyContainer(editors::TIEditor &editor)
{
    m_editor = editor;
    m_pPropGrid = new CWTLPropertyGrid<T>(editor);
    m_pMenuCtrl = new CToolbarContainer<T>(this);

    oes::foundation::IEventManager *mgr = editor->GetApp()->GetEventManager();

    if (mgr)
    {
        mgr->RegisterFunc([&](ESystemEventID id, const CObjectAbstract *param)
        {
            Update((T*)(param), id);
        });
    }
}

//----------------------------------------------------------------------------------------------
template<class T>
CPanePropertyContainer<T>::~CPanePropertyContainer()
{
    SAFE_DELETE(m_pPropGrid);
    SAFE_DELETE(m_pMenuCtrl);
}

//----------------------------------------------------------------------------------------------
template<class T>
void CPanePropertyContainer<T>::SetRenderContext(oes::d3d::SRenderContext *pContext)
{
    assert(m_pPropGrid);

    m_pPropGrid->SetRenderContext(pContext);
}

//----------------------------------------------------------------------------------------------
template<class T>
void CPanePropertyContainer<T>::Update(const T *pSender, ESystemEventID EventId)
{
    assert(m_pPropGrid);

    m_pPropGrid->_Update(pSender, EventId);
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CPanePropertyContainer<T>::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
    RECT rcDefault = { 0, 25, 200, 200 };

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
template<class T>
LRESULT CPanePropertyContainer<T>::OnNotify(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
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
        if (((LPNMHDR)lParam)->hwndFrom == m_pPropGrid->m_hWnd) {
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
template<class T>
LRESULT CPanePropertyContainer<T>::OnSize(UINT, WPARAM, LPARAM, BOOL& bHendled)
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
template<class T>
LRESULT CPanePropertyContainer<T>::OnDeleteAll(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&)
{
    ClearTabPages();

    m_pPropGrid->ClearListProperties();
    m_pPropGrid->HideChildControls();

    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CPanePropertyContainer<T>::OnFilledProps(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&)
{
    ClearTabPages();

    std::vector<std::string> groups;
    m_pPropGrid->GetPropertyGroups(groups);

    int idx = 0;
    for each (const auto &str in groups)
    {
        AddTabPage(str.c_str(), idx++);
    }
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CPanePropertyContainer<T>::OnUpdateProps(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&)
{
    m_pPropGrid->UpdatePreview();
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
bool CPanePropertyContainer<T>::AddTabPage(const char *name, int Index = 0)
{
    wchar_t wbuf[256] = { 0 }; // temp buffers
    char ascii[256] = { 0 };

    MultiByteToWideChar(CP_ACP, 0, name, INDEX_NONE, wbuf, 256);

    TCITEM tc = { 0 };
    tc.mask = TCIF_TEXT;
    tc.pszText = wbuf;
    m_pTabCtrl.InsertItem(Index, &tc);

    return true;
}

//----------------------------------------------------------------------------------------------
template<class T>
bool CPanePropertyContainer<T>::ClearTabPages()
{
    m_pTabCtrl.DeleteAllItems();
    return true;
}

//----------------------------------------------------------------------------------------------
template<class T>
void CPanePropertyContainer<T>::SetGridViewStyle(EGridView Value)
{
    m_pPropGrid->SetGridViewStyle(Value);
}

//----------------------------------------------------------------------------------------------
template<class T>
void CPanePropertyContainer<T>::OnObjectSelected()
{
    m_pPropGrid->FillPropertyGrid(m_editor->GetSelected());
}