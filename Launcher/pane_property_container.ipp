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
