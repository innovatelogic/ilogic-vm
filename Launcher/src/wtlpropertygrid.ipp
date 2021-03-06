#include "WTLPropertyGrid.h"

#define LIST_CLASS_ELEMENT 0
#define LIST_PROP_ELEMENT 1

//----------------------------------------------------------------------------------------------
template<class T>
CWTLPropertyGrid<T>::CWTLPropertyGrid(editors::TIEditor &editor)
    : m_bInit(FALSE)
    , m_GridViewStyle(EGV_Categorized)
    , m_EditingPropertyIndex(INDEX_NONE)
    , m_MemoryOffset(0)
    , m_PointSize(90)
    , m_TypeFace(_T("MS Shell Dlg 2"))
    , m_nSelectedGroup(INDEX_NONE)
    , m_pAppMain(nullptr)
    , m_pRenderContext(0)
{
    m_propReactor = std::make_shared<oes::rflex::PropertyReactor<IObjectAbstract>>();

    m_editor = editor;
    m_pAppMain = editor->GetApp();

    m_pEdit = new TEdit(this);
    m_pComboBox = new TComboBox(this);
    m_pResourceEdit = new TResourceEdit(this);
}

//----------------------------------------------------------------------------------------------
template<class T>
CWTLPropertyGrid<T>::~CWTLPropertyGrid()
{
    SAFE_DELETE(m_pEdit);
    SAFE_DELETE(m_pComboBox);
    SAFE_DELETE(m_pResourceEdit);
}

//----------------------------------------------------------------------------------------------
template<class T>
HWND CWTLPropertyGrid<T>::Create(HWND hWndParent,
    ATL::_U_RECT rect = NULL, 
    LPCTSTR szWindowName = NULL,
    DWORD dwStyle = 0, 
    DWORD dwExStyle = 0,
    ATL::_U_MENUorID MenuOrID = 0U,
    LPVOID lpCreateParam = NULL)
{
    dwStyle |= LVS_OWNERDATA; // We provide item data via the GETDISPINFO handler
    dwStyle |= LVS_REPORT; // "Details" mode
    dwStyle |= LVS_SHOWSELALWAYS;
    dwStyle |= LVS_SINGLESEL;

    return CWindowImpl<CWTLPropertyGrid<T>, CListViewCtrl, CListTraits>::Create(hWndParent,
        rect.m_lpRect, szWindowName, dwStyle, dwExStyle, MenuOrID.m_hMenu,
        lpCreateParam);
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CWTLPropertyGrid<T>::OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
    DWORD dwExStyle = 0;
    dwExStyle |= LVS_EX_GRIDLINES;
    dwExStyle |= LVS_EX_SUBITEMIMAGES; // Allows images to be displayed for subitems
    dwExStyle |= LVS_EX_FULLROWSELECT;
    dwExStyle |= LVS_EX_DOUBLEBUFFER; // reduces flicker

    SetExtendedListViewStyle(dwExStyle);

    // Create the listview and edit control font
    m_Font.CreatePointFont(m_PointSize, m_TypeFace);

    m_pEdit->Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    m_pEdit->SetFont(m_Font);

    m_pResourceEdit->Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    m_pResourceEdit->SetFont(m_Font);

    m_pComboBox->Create(m_hWnd, rcDefault);
    m_pComboBox->SetFont(m_Font);

    SetFont(m_Font);

    LoadHelperImageList();

    RECT rect;
    GetClientRect(&rect);

    int Width = rect.right - rect.left;
    int Height = rect.bottom - rect.top;

    AddColumn(TEXT("Property"), 0, 0, LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM, LVCFMT_LEFT);
    AddColumn(TEXT("Value"), 1, 1, LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_IMAGE, LVCFMT_LEFT);

    bHandled = FALSE;
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CWTLPropertyGrid<T>::OnNotify(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (m_bInit == TRUE)
    {
        switch (((LPNMHDR)lParam)->code)
        {
        case NM_CLICK:
        {
            if (((LPNMHDR)lParam)->hwndFrom == m_pComboBox->m_hWnd) {
                ClickListComboProperties((LPNMLISTVIEW)lParam);
            }
        }break;

        case HDN_BEGINTRACKA:
        case HDN_BEGINTRACKW:
        {
            HD_NOTIFY* pHDN = (HD_NOTIFY*)lParam;
            if (pHDN->iItem == 1)
            {
                return TRUE; // skip second row width change
            }
        }break;

        case HDN_ENDTRACK:
        {
            HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;

            if (pHDN->pitem->mask & HDI_WIDTH)
            {
                ::PostMessage(m_hWnd, WM_ADJUST_COLUMN_WIDTH, 0, 0);

                InvalidateRect(NULL, TRUE);
            }
        }
        break;
        };
    }
    bHandled = TRUE;
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CWTLPropertyGrid<T>::OnLBClick(UINT, WPARAM, LPARAM lParam, BOOL& bHandled)
{
    if (((HWND)lParam) == m_pResourceEdit->GetButtonBrowse()->m_hWnd)
    {
        // check cursor hit
        OPENFILENAME ofn;
        WCHAR szFileName[MAX_PATH] = L"";
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = m_hWnd;
        ofn.lpstrFilter = L"Bitmap Files (*.tga)\0*.tga\0DDS Files (*.dds)\0*.dds\0All Files (*.*)\0*.*\0";
        ofn.lpstrFile = szFileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        ofn.lpstrDefExt = L"txt";

        std::string chFileName = ConvertWideStringToString(szFileName);
        if (GetOpenFileName(&ofn))
        {
            m_PropertyCS.enter();

            TCHAR	szText[256];
            int SelectedGroup = m_nSelectedGroup;

            TCITEM tc;
            tc.mask = TCIF_TEXT;
            tc.pszText = szText;
            tc.cchTextMax = 255;

            oes::rflex::SPropertyClass *pOutClass = 0;
            oes::rflex::Property_Base *pOutProperty = 0;
            int OutMemoryOffset = 0;

            if (GetPropertyByIndex(m_EditingPropertyIndex, SelectedGroup, &pOutClass, &pOutProperty, OutMemoryOffset))
            {
                if (pOutProperty && pOutClass)
                {
                    std::string ResourceTag(chFileName);
                    std::string::size_type pos0 = ResourceTag.rfind("\\");
                    ResourceTag = ResourceTag.substr((pos0 + 1));

                    PushContext();

                    pOutProperty->SetProperty((BYTE*)pOutClass->pDataObject + OutMemoryOffset, ResourceTag.c_str()); // set new value

                    PopContext();
                }
            }
        }
        m_EditingPropertyIndex = INDEX_NONE;
        m_MemoryOffset = 0;
        HideChildControls();
    }
    else if (((HWND)lParam) == m_pResourceEdit->GetButtonClear()->m_hWnd)
    {
        m_PropertyCS.enter();

        TCHAR	szText[256];
        int SelectedGroup = m_nSelectedGroup;

        TCITEM tc;
        tc.mask = TCIF_TEXT;
        tc.pszText = szText;
        tc.cchTextMax = 255;

        oes::rflex::SPropertyClass *pOutClass = 0;
        oes::rflex::Property_Base *pOutProperty = 0;
        int OutMemoryOffset = 0;

        if (GetPropertyByIndex(m_EditingPropertyIndex, SelectedGroup, &pOutClass, &pOutProperty, OutMemoryOffset))
        {
            if (pOutProperty && pOutClass)
            {
                PushContext();

                pOutProperty->SetProperty((BYTE*)pOutClass->pDataObject + OutMemoryOffset, "none"); // set new value

                PopContext();
            }
        }

        m_PropertyCS.leave();

        m_EditingPropertyIndex = INDEX_NONE;
        m_MemoryOffset = 0;

        HideChildControls();
    }
    bHandled = FALSE;
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CWTLPropertyGrid<T>::OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    if ((BOOL)wParam == TRUE)
    {
        if (m_bInit == FALSE)
        {
            RECT rect;
            GetClientRect(&rect);

            int Width = rect.right - rect.left;
            int Height = rect.bottom - rect.top;

            SetColumnWidth(0, (int)(Width * 0.7f)); // 1/3
            SetColumnWidth(1, (int)(Width * 0.3f)); // 2/3

            EnsureVisible(0, TRUE); // Make sure top row shows

            m_bInit = TRUE;
        }
    }
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CWTLPropertyGrid<T>::OnScrollWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    ScrollWindow();

    bHandled = FALSE;
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::ScrollWindow()
{
    HideChildControls();
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::AdjustColumnsSize()
{
    if (m_bInit)
    {
        const int minColumnSize = 80;

        RECT rect;
        GetWindowRect(&rect);

        int scrollWidth = GetSystemMetrics(SM_CXVSCROLL);

        int width = (rect.right - rect.left) - scrollWidth;

        if (width > 0)
        {
            if (width > minColumnSize * 2)
            {
                int width_0 = GetColumnWidth(0);
                int width_1 = GetColumnWidth(1);

                if ((width_0 + width_1) < width) // scale up
                {
                    int delta = width - (width_0 + width_1);
                    SetColumnWidth(1, width_1 + delta);
                }
                else // scale down
                {
                    int delta = (width_0 + width_1) - width;

                    if (width_1 - delta > minColumnSize)
                    {
                        SetColumnWidth(1, width_1 - delta);
                        return;
                    }
                    else
                    {
                        SetColumnWidth(1, minColumnSize);
                        delta = minColumnSize - (width_1 - delta);
                    }

                    if (width_0 - delta > minColumnSize)
                    {
                        SetColumnWidth(0, width_0 - delta);
                    }
                    else
                    {
                        SetColumnWidth(0, minColumnSize);
                    }
                }
            }
            else
            {
                SetColumnWidth(0, minColumnSize);
                SetColumnWidth(1, minColumnSize);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CWTLPropertyGrid<T>::OnDeleteAll(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&)
{
    ClearListProperties();
    HideChildControls();

    m_nSelectedGroup = 0;
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CWTLPropertyGrid<T>::OnFillProps(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&)
{
    m_nSelectedGroup = 0;

    FillPropertyTabs();
    FillListProperties();

    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CWTLPropertyGrid<T>::OnUpdateProps(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&)
{
    UpdatePreview();
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CWTLPropertyGrid<T>::OnAdjustColumnWidth(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AdjustColumnsSize();

    bHandled = TRUE;
    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CWTLPropertyGrid<T>::OnBeginTrack(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    return S_OK;
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CWTLPropertyGrid<T>::OnLVEndLabelEdit(WPARAM wParam)
{
    if (wParam == VK_RETURN)
    {
        SFetchData &data = m_cacheFill.at(m_EditingPropertyIndex);

        std::string value;
        if (GetPropertySelectedBatch(data.property, value))
        {
            wchar_t wbuf[256] = { 0 };
            char ascii[256] = { 0 };

            if (data.property->GetCtrl() == CTRL_EDIT_RESOURCE) { // switch property type
                m_pResourceEdit->SendMessage(WM_GETTEXT, 256, (LPARAM)wbuf);
            }
            else {
                m_pEdit->SendMessage(WM_GETTEXT, 256, (LPARAM)wbuf);
            }

            WideCharToMultiByte(CP_ACP, 0, wbuf, 256, ascii, 256, NULL, NULL);

            //m_editor->SetProperty(ascii, value, m_editor->GetSelected(), data.property);
       }
    }

    m_EditingPropertyIndex = INDEX_NONE;
    m_MemoryOffset = 0;

    HideChildControls();
    UpdatePreview();

    return 0;
}

//----------------------------------------------------------------------------------------------
template<class T>
LRESULT CWTLPropertyGrid<T>::OnLVCustomDraw(int, LPNMHDR pNMHDR, BOOL&)
{
    return HandleCustomDraw((LPNMLVCUSTOMDRAW)pNMHDR);
}

//----------------------------------------------------------------------------------------------
template<class T>
long CWTLPropertyGrid<T>::HandleCustomDraw(LPNMLVCUSTOMDRAW pNMLVCD)
{
    if (pNMLVCD == NULL) {
        return INDEX_NONE;
    }

    switch (pNMLVCD->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
    {
        return (CDRF_NOTIFYPOSTPAINT | CDRF_NOTIFYITEMDRAW);
    }break;

    case CDDS_ITEMPREPAINT:
    {
        return CDRF_NOTIFYSUBITEMDRAW;
    }break;

    case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
    {
        CustomDrawProperty(pNMLVCD);
        return (CDRF_NEWFONT); // | CDRF_NOTIFYPOSTPAINT);
    }break;

    case CDDS_ITEMPOSTPAINT:
    {
        return CDRF_DODEFAULT;
    }
    break;

    default:
    break;
    }
    return CDRF_DODEFAULT;
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::CustomDrawProperty(LPNMLVCUSTOMDRAW pNMLVCD)
{
    SFetchData &data = m_cacheFill.at((int)pNMLVCD->nmcd.dwItemSpec);

    switch (data.id)
    {
    case LIST_CLASS_ELEMENT:
    {
        pNMLVCD->clrText = 0x00000011; // 0x00bbggrr
        pNMLVCD->clrTextBk = RGB(220, 220, 220);
        pNMLVCD->nmcd.uItemState &= ~CDIS_SELECTED; // kill sys sel.
    }break;

    case LIST_PROP_ELEMENT:
    {
        std::string value;

        bool editable = GetPropertySelectedBatch(data.property, value);
        
        int policy = data.property->GetPolicy();

        if (policy == READ_ONLY || policy == NO_READ_WRITE || data.property->GetCtrl() == CTRL_ARRAY)
        {
            pNMLVCD->nmcd.uItemState &= ~CDIS_SELECTED; // kill sys sel
            pNMLVCD->clrText = RGB(128, 128, 128);
        }

        if (pNMLVCD->iSubItem == 1)
        {
            switch (data.property->GetCtrl()) // switch property type
            {
            case CTRL_COLOR:
            {
                pNMLVCD->nmcd.uItemState &= ~CDIS_SELECTED; // kill sys sel.

                std::string value;
                if (GetPropertySelectedBatch(data.property, value))
                {
                    int val = atoi(value.c_str());

                    //assert(value.size() == 4);
                    DWORD buffer = MAKELONG(MAKEWORD(value[4], value[3]), MAKEWORD(value[2], value[1]));
                    DWORD color = RGB(GetBValue(val), GetGValue(val), GetRValue(val));
                    
                    byte r = (val & 0x00FF0000) >> 16;
                    byte g = (val & 0x0000FF00) >> 8;
                    byte b = (val & 0x000000FF);

                    pNMLVCD->clrText = RGB(r, g, b);
                    pNMLVCD->clrTextBk = RGB(r, g, b);
                }
            }break;

            case CTRL_COMBO:
            {
                pNMLVCD->nmcd.uItemState &= ~CDIS_SELECTED; // kill sys sel.
            }break;

            default:
                //assert(false);
                break;
            }
        }

    }break;
    default:
        assert(false);
        break;
    }
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::FillModel()
{
    m_cacheDataAll.clear();

    oes::rflex::IPropertyReactor::TMapClassData &classes = m_propReactor->GetClasses();

    // fetch data
    for each (auto &item in classes)
    {
        m_cacheDataAll.push_back(SFetchData(LIST_CLASS_ELEMENT, &item, nullptr));

        for each (auto prop in item.properties)
        {
            m_cacheDataAll.push_back(SFetchData(LIST_PROP_ELEMENT, &item, prop));
        }
    }
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::FillPropertyGrid(std::vector<IObjectAbstract*> &actors)
{
    m_propReactor->Build(m_editor->GetSelected());

    FillModel();

    if (!actors.empty()){
        SetSelected(actors.at(0));
    }

    PostMessage(WM_USER_DELETE_ALL, 0, 0);
    PostMessage(WM_USER_FILL_PROPS, 0, 0);
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::FillPropertyTabs()
{
    if (IObjectAbstract *pSelected = GetSelected())
    {
        FillPropertyData(pSelected);
    }
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::FillPropertyData(IObjectAbstract *pActor)
{
    m_PropertyCS.enter();

    m_pSelectedActor = pActor;
    m_nSelectedGroup = 0; // total reinit clear value

    oes::rflex::AppClassTree &classTree = oes::rflex::GetClassTree();

    if (oes::rflex::ClassNode *pClassNode = classTree.Find(pActor->GetType()))
    {
        while (pClassNode)
        {
            TVecPropertyConstIterator IterProp = pClassNode->propertyMap.begin();

            while (IterProp != pClassNode->propertyMap.end())
            {
                // find or alloc group
                SPropertyGroup *pGroup = GetGroupByName((*IterProp)->GetGroupName());
                if (!pGroup)
                {
                    pGroup = new SPropertyGroup((*IterProp)->GetGroupName());

                    if (pGroup->GroupName == "Value") {
                        m_PropertyGroups.insert(m_PropertyGroups.begin(), pGroup);
                    }
                    else {
                        m_PropertyGroups.push_back(pGroup);
                    }
                }

                // find or alloc class
                oes::rflex::SPropertyClass *pClass = 0;
                oes::rflex::TVecPropertyClassIter IterClass = pGroup->VecPropertyClasses.begin();

                while (IterClass != pGroup->VecPropertyClasses.end())
                {
                    if ((*IterClass)->ClassName == (*IterProp)->ClassName())
                    {
                        pClass = *IterClass;
                        break;
                    }
                    ++IterClass;
                }

                if (!pClass)
                {
                    pClass = new oes::rflex::SPropertyClass((*IterProp)->ClassName(), (void*)pActor);
                    pGroup->VecPropertyClasses.insert(pGroup->VecPropertyClasses.begin(), pClass);
                    pClass->InheritProperties.push_back(NULL); // separator
                }

                pClass->InheritProperties.push_back(*IterProp);

                // add sorted array (for non grouped mode)
                m_VSortedProperties.push_back(new SPropertyWrapper(*IterProp, pClass));

                ++IterProp;
            }

            // add interface properties
            oes::rflex::ClassNode::TVecInterfaceIter IterIntf = pClassNode->interfaces.begin();

            while (IterIntf != pClassNode->interfaces.end())
            {
                oes::rflex::ClassNode *pNodeInterface = classTree.FindInterface((*IterIntf)->strType);

                if (pNodeInterface)
                {
                    TVecPropertyConstIterator IterIntfProp = pNodeInterface->propertyMap.begin();

                    while (IterIntfProp != pNodeInterface->propertyMap.end())
                    {
                        // find or alloc group
                        SPropertyGroup *pGroup = GetGroupByName("Value");
                        assert(pGroup);

                        // find or alloc class
                        oes::rflex::SPropertyClass *pClass = 0;
                        oes::rflex::TVecPropertyClassIter IterClass = pGroup->VecPropertyClasses.begin();

                        while (IterClass != pGroup->VecPropertyClasses.end())
                        {
                            if (!strcmp((*IterClass)->ClassName.c_str(), (*IterIntf)->strType))
                            {
                                pClass = *IterClass;
                                break;
                            }
                            ++IterClass;
                        }

                        if (!pClass)
                        {
                            pClass = new oes::rflex::SPropertyClass((*IterIntf)->strType, (void*)pActor, (*IterIntf)->byteShift);
                            pGroup->VecPropertyClasses.insert(pGroup->VecPropertyClasses.begin(), pClass);
                            pClass->InheritProperties.push_back(NULL);
                        }

                        pClass->InheritProperties.push_back(*IterIntfProp);

                        // add sorted array (for non grouped mode)
                        m_VSortedProperties.push_back(new SPropertyWrapper(*IterIntfProp, pClass));

                        ++IterIntfProp;
                    }
                }
                ++IterIntf;
            }

            pClassNode = pClassNode->GetRootNode();
        }
    }

    const Registry *pReg = m_pAppMain->GetRegistry();

    assert(pReg);

    // transient properties
/*    for (TVecActorChildConstIterator IterActor = pActor->m_ChildNodes.begin(); IterActor != pActor->m_ChildNodes.end(); ++IterActor)
    {
        if ((*IterActor)->IsTransient() &&
            pReg->IsEditorVisible((*IterActor)->GetType()))
        {
            FillPropertyDataTransient(*IterActor);
        }
    }*/

    std::sort(m_VSortedProperties.begin(), m_VSortedProperties.end(), SPropertyWrapper::CompByName);

    ::SendMessage(GetParent().m_hWnd, WM_USER_PROPS_FILLED, 0, 0);

    m_PropertyCS.leave();
}

//----------------------------------------------------------------------------------------------
template<class T>
SPropertyGroup* CWTLPropertyGrid<T>::GetGroupByName(const std::string &name) const
{
    // find or alloc group
    SPropertyGroup *pOutGroup = NULL;

    TVecPropertyGroupConstIter IterGroup = m_PropertyGroups.begin();
    while (IterGroup != m_PropertyGroups.end())
    {
        if ((*IterGroup)->GroupName == name) {
            pOutGroup = *IterGroup;
            break;
        }
        ++IterGroup;
    }
    return pOutGroup;
}

//----------------------------------------------------------------------------------------------
template<class T>
bool CWTLPropertyGrid<T>::IsClassAllowed(const std::string& name) const
{
    std::vector<std::string>::const_iterator IterCollapsedClass = std::find(m_VCollapsedClassNames.begin(), m_VCollapsedClassNames.end(), name);
    return IterCollapsedClass == m_VCollapsedClassNames.end();
}

//----------------------------------------------------------------------------------------------
template<class T>
bool CWTLPropertyGrid<T>::IsDisclosed(const std::string &name) const
{
    return true;
}

//----------------------------------------------------------------------------------------------
template<class T>
BOOL CWTLPropertyGrid<T>::FillListProperties()
{
    BOOL bResult = FALSE;

    HideChildControls();

    wchar_t wGroupBuff[256] = { 0 };
    wchar_t wPropBuff[256] = { 0 };
    wchar_t wPropValBuff[256] = { 0 };

    int SelectedGroup = m_nSelectedGroup;

    if (SelectedGroup != INDEX_NONE)
    {
        std::vector<std::string> groups;
        m_propReactor->FetchGroups(groups);
        std::string strGroupName = groups.at(SelectedGroup); // TODO: catch exp
                                                                            
        // selected object property
        m_cacheFill.clear();

        if (m_GridViewStyle == EGV_Categorized)
        {
            m_cacheFill.clear();

            // arrange data by indexes
            std::vector<SFetchData>::iterator iter = m_cacheDataAll.begin();
            while (iter != m_cacheDataAll.end())
            {
                assert(iter->id == LIST_CLASS_ELEMENT);

                std::vector<SFetchData> propsX;

                SFetchData classX(LIST_CLASS_ELEMENT, iter->pclass, nullptr);
                
                std::vector<SFetchData>::iterator iterProp = ++iter;
                while (iterProp != m_cacheDataAll.end() && iterProp->id != LIST_CLASS_ELEMENT)
                {
                    if (iterProp->property->GetGroupName() == strGroupName){
                        propsX.push_back(SFetchData(LIST_PROP_ELEMENT, iter->pclass, iterProp->property));
                    }
                    ++iterProp;
                }

                if (!propsX.empty())
                {
                    m_cacheFill.push_back(classX);
                    m_cacheFill.insert(m_cacheFill.end(), propsX.begin(), propsX.end());
                }
                iter = iterProp;
            }

            int listIndex = 0;

            // populate
            for each (auto &item in m_cacheFill)
            {
                LVITEM lvI;
                memset(&lvI, 0, sizeof(LVITEM));

                lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
                lvI.iSubItem = 0;
                lvI.pszText = LPSTR_TEXTCALLBACK;
                lvI.iImage = I_IMAGECALLBACK;
                lvI.iItem = listIndex++;
                lvI.lParam = reinterpret_cast<LPARAM>(&item);
                lvI.iIndent = 0;

                InsertItem(&lvI);
            }


           /* for each (auto &item in fetchClasses)
            {          
                // selection query
                bool bAllowClass = IsClassAllowed(item.name);

                {
                    LVITEM lvI;
                    memset(&lvI, 0, sizeof(LVITEM));

                    lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
                    lvI.iSubItem = 0;
                    lvI.pszText = LPSTR_TEXTCALLBACK;
                    lvI.iImage = I_IMAGECALLBACK;
                    lvI.iItem = LIST_CLASS_ELEMENT;
                    lvI.lParam = &item;
                    lvI.iIndent = 0;
                    InsertItem(&lvI);
                }

                for each (const auto &prop in item.properties)
                {
                    LVITEM lvI;
                    memset(&lvI, 0, sizeof(LVITEM));

                    lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
                    lvI.iSubItem = 0;
                    lvI.pszText = LPSTR_TEXTCALLBACK;
                    lvI.iImage = I_IMAGECALLBACK;
                    lvI.iItem = LIST_PROP_ELEMENT;
                    lvI.iIndent = 0;

                    InsertItem(&lvI);
                }
            }*/

            /*for (TVecPropertyClassIter IterClass = pGroup->VecPropertyClasses.begin(); IterClass != pGroup->VecPropertyClasses.end(); ++IterClass)
            {
                // selection query
                bool bAllowClass = IsClassAllowed((*IterClass)->ClassName);

                for (TVecPropertyBaseIter IterProp = (*IterClass)->InheritProperties.begin(); IterProp != (*IterClass)->InheritProperties.end(); ++IterProp)
                {
                    LVITEM lvI;
                    memset(&lvI, 0, sizeof(LVITEM));

                    lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
                    lvI.iSubItem = 0;
                    lvI.pszText = LPSTR_TEXTCALLBACK;
                    lvI.iImage = I_IMAGECALLBACK;
                    lvI.iItem = propertyListIndex++;
                    lvI.iIndent = 0;

                    InsertItem(&lvI);

                    // fill array 
                    if ((*IterProp) && (*IterProp)->GetCtrl() == CTRL_ARRAY && IsDisclosed((*IterProp)->GetName()))
                    {
                        int Dimension = (*IterProp)->GetSize();
                        while (Dimension > 0)
                        {
                            // go through child nodes
                            Property_Base * ChildProp = (*IterProp)->GetNext();
                            while (ChildProp)
                            {
                                LVITEM chlvI;
                                memset(&chlvI, 0, sizeof(LVITEM));

                                lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
                                lvI.iSubItem = 0;
                                lvI.pszText = LPSTR_TEXTCALLBACK;
                                lvI.iImage = I_IMAGECALLBACK;
                                lvI.iItem = PropertyListIndex++;
                                lvI.iIndent = 0;

                                InsertItem(&lvI);

                                ChildProp = ChildProp->GetNext();
                            }
                            --Dimension;
                        }
                    }

                    if (!bAllowClass && !*IterProp) {
                        break;
                    }
                }
            }*/
        }
        else if (m_GridViewStyle == EGV_SortByName)
        {
            // selected object property
            /* int PropertyListIndex = 0;
            for (TVecPropertyWrapperIter Iter = m_VSortedProperties.begin(); Iter != m_VSortedProperties.end(); ++Iter)
            {
                if ((*Iter)->pProp->GetGroupName() == pGroup->GroupName)
                {
                    LVITEM lvI;
                    memset(&lvI, 0, sizeof(LVITEM));

                    lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
                    lvI.iSubItem = 0;
                    lvI.pszText = LPSTR_TEXTCALLBACK;
                    lvI.iImage = I_IMAGECALLBACK;
                    lvI.iItem = PropertyListIndex++;
                    lvI.iIndent = 0;

                    InsertItem(&lvI);

                    // fill array 
                    if ((*Iter) && (*Iter)->pProp->GetCtrl() == CTRL_ARRAY && IsDisclosed((*Iter)->pProp->GetName()))
                    {
                        int Dimension = (*Iter)->pProp->GetSize();
                        while (Dimension > 0)
                        {
                            // go through child nodes
                            Property_Base * ChildProp = (*Iter)->pProp->GetNext();
                            while (ChildProp)
                            {
                                LVITEM chlvI;
                                memset(&chlvI, 0, sizeof(LVITEM));

                                lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
                                lvI.iSubItem = 0;
                                lvI.pszText = LPSTR_TEXTCALLBACK;
                                lvI.iImage = I_IMAGECALLBACK;
                                lvI.iItem = PropertyListIndex++;
                                lvI.iIndent = 0;

                                InsertItem(&lvI);

                                ChildProp = ChildProp->GetNext();
                            }
                            --Dimension;
                        }
                    }
                }
            }*/
        }
    }

    return TRUE;
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::FillPropertyDataTransient(T *pActor)
{
    oes::rflex::AppClassTree &CTree = oes::rflex::GetClassTree();

    if (oes::rflex::ClassNode *pCNode = CTree.Find(pActor->GetType()))
    {
        oes::rflex::SPropertyClass *pClassRoot = new oes::rflex::SPropertyClass(pCNode->GetName(), (void*)pActor);
        pClassRoot->InheritProperties.push_back(NULL);

        SPropertyGroup *pGroupValue = GetGroupByName("Value");
        assert(pGroupValue);

        pGroupValue->VecPropertyClasses.push_back(pClassRoot);

        while (pCNode)
        {
            TVecPropertyConstIterator IterProp = pCNode->propertyMap.begin();
            while (IterProp != pCNode->propertyMap.end())
            {
                //if ((*IterProp)->IsSerializable())
                {
                    oes::rflex::SPropertyClass *pClass = pClassRoot;

                    // find or alloc group
                    SPropertyGroup *pGroup = GetGroupByName((*IterProp)->GetGroupName());
                    if (!pGroup)
                    {
                        pGroup = new SPropertyGroup((*IterProp)->GetGroupName());
                        pClass = new oes::rflex::SPropertyClass(pCNode->GetName(), (void*)pActor);

                        pGroup->VecPropertyClasses.push_back(pClass);
                        pClass->InheritProperties.push_back(NULL);

                        m_PropertyGroups.push_back(pGroup);
                    }

                    pClass->InheritProperties.push_back(*IterProp);

                    // add sorted array
                    m_VSortedProperties.push_back(new SPropertyWrapper(*IterProp, pClass));
                }
                ++IterProp;
            }
            pCNode = pCNode->GetRootNode();
        }
    }

    const Registry *pReg = m_pAppMain->GetRegistry();
    assert(pReg);

    // transient properties
    for (TVecActorChildConstIterator IterActor = pActor->m_ChildNodes.begin(); IterActor != pActor->m_ChildNodes.end(); ++IterActor)
    {
        if ((*IterActor)->IsTransient() &&
            pReg->IsEditorVisible((*IterActor)->GetType()))
        {
            FillPropertyDataTransient(*IterActor);
        }
    }
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::UpdatePreview()
{
    m_PropertyCS.enter();

    int SelectedGroup = m_nSelectedGroup;

    if (SelectedGroup != INDEX_NONE)
    {
        std::string StrGroupName = m_PropertyGroups[m_nSelectedGroup]->GroupName; //ConvertWideStringToString(szText);
                                                                          // find group
        SPropertyGroup *pGroup = GetGroupByName(StrGroupName);
        assert(pGroup);

        int PropertyListIndex = 0;

        LockWindowUpdate();

        if (m_GridViewStyle == EGV_Categorized)
        {
            for (oes::rflex::TVecPropertyClassIter IterClass = pGroup->VecPropertyClasses.begin(); IterClass != pGroup->VecPropertyClasses.end(); ++IterClass)
            {
                for (oes::rflex::TVecPropertyBaseIter IterProp = (*IterClass)->InheritProperties.begin(); IterProp != (*IterClass)->InheritProperties.end(); ++IterProp)
                {
                    Update(PropertyListIndex++);
                }
            }
        }
        else if (m_GridViewStyle == EGV_SortByName)
        {
            for (TVecPropertyWrapperIter Iter = m_VSortedProperties.begin(); Iter != m_VSortedProperties.end(); ++Iter)
            {
                if ((*Iter)->pProp->GetGroupName() == pGroup->GroupName)
                {
                    Update(PropertyListIndex++);
                }
            }
        }
        LockWindowUpdate(FALSE);
    }
    m_PropertyCS.leave();
}

//----------------------------------------------------------------------------------------------
template<class T>
BOOL CWTLPropertyGrid<T>::GETDISPINFO_FillList(LVITEMA *pItem)
{
    TCHAR	szText[256];

    TCITEM tc;
    tc.mask = TCIF_TEXT;
    tc.pszText = szText;
    tc.cchTextMax = 255;

    SFetchData &data = m_cacheFill.at(pItem->iItem);

    switch (data.id)
    {
    case LIST_CLASS_ELEMENT:
    {
        const oes::rflex::SClassNode *pclass = data.pclass;
        assert(pclass);

        switch (pItem->iSubItem)
        {
        case 0:
        {
            MultiByteToWideChar(CP_ACP, 0, pclass->name.c_str(), -1, wbuf, 255);
            pItem->pszText = (LPSTR)wbuf;

            bool bCollapsed = IsClassAllowed(pclass->name);
            pItem->iImage = !bCollapsed ? 2 : 3;
        }break;
        default:
            break;
        };
    }break;

    case LIST_PROP_ELEMENT:
    {
        const oes::rflex::SClassNode *pclass = data.pclass;
        oes::rflex::Property_Base *prop = data.property;

        assert(pclass && prop);

        switch (pItem->iSubItem)
        {
        case 0:
        {
            MultiByteToWideChar(CP_ACP, 0, prop->GetName(), -1, wbuf, 255);
            pItem->pszText = (LPSTR)wbuf;
        }break;

        case 1:
        {
            // disable property if value differ in different objects
            std::string str_val;
            if (!GetPropertySelectedBatch(prop, str_val))
            {
                str_val = "...";
            }
            else
            {
                if (IsPropertyBOOL(prop) && (pItem->mask & LVIF_IMAGE))
                {
                    pItem->iImage = (str_val == "true") ? 1 : 0;
                }
            }

            MultiByteToWideChar(CP_ACP, 0, str_val.c_str(), -1, wbuf, 255);
            pItem->pszText = (LPSTR)wbuf;
        }break;
        default:
            break;
        }

    }break;

    default:
        break;
    }
 
    return FALSE;
}

//----------------------------------------------------------------------------------------------
template<class T>
bool CWTLPropertyGrid<T>::GetPropertyByIndex(int InPlainIndex,
    int IndexGroup,
    oes::rflex::SPropertyClass **OutClass,
    class oes::rflex::Property_Base **OutProperty,
    int &OutMemoryOffset) const
{
    bool bFind = false;
    const unsigned int sSize = 256;

    if (IndexGroup != INDEX_NONE)
    {
        int Counter = 0;

        *OutClass = 0;
        *OutProperty = 0;
        OutMemoryOffset = 0;

        std::string StrGroupName = m_PropertyGroups[IndexGroup]->GroupName;

        // find or alloc group
        SPropertyGroup *pGroup = GetGroupByName(StrGroupName);
        assert(pGroup);

        if (m_GridViewStyle == EGV_Categorized)
        {
            for (oes::rflex::TVecPropertyClassConstIter IterClass = pGroup->VecPropertyClasses.begin();
            !bFind && IterClass != pGroup->VecPropertyClasses.end(); ++IterClass)
            {
                // selection query
                bool bAllowClass = IsClassAllowed((*IterClass)->ClassName);

                for (oes::rflex::TVecPropertyBaseConstIter IterProp = (*IterClass)->InheritProperties.begin(); !bFind && IterProp != (*IterClass)->InheritProperties.end(); ++IterProp)
                {
                    if (InPlainIndex == Counter++)
                    {
                        *OutClass = (*IterClass);

                        if (*IterProp) // property
                        {
                            *OutProperty = *IterProp;
                            OutMemoryOffset = 0;
                        }

                        bFind = true;
                        break;
                    }
                    else if ((*IterProp) && (*IterProp)->GetCtrl() == CTRL_ARRAY && IsDisclosed((*IterProp)->GetName()))
                    {
                        int ElementDim = (*IterProp)->GetSize();
                        int ElementSize = (*IterProp)->GetElementSize();
                        int MemOffset = (*IterProp)->m_MemOffset;

                        while (ElementDim && !bFind)
                        {
                            // go through child nodes
                            oes::rflex::Property_Base * ChildProp = (*IterProp)->GetNext();

                            while (ChildProp)
                            {
                                if (Counter++ == InPlainIndex)
                                {
                                    *OutClass = (*IterClass);
                                    *OutProperty = ChildProp;
                                    OutMemoryOffset = (*IterProp)->m_MemOffset + ElementSize * ((*IterProp)->GetSize() - ElementDim);
                                    bFind = true;
                                    break;
                                }
                                MemOffset += ChildProp->m_MemOffset;
                                ChildProp = ChildProp->GetNext();
                            }

                            --ElementDim;
                            MemOffset = (*IterProp)->m_MemOffset + ElementSize * ((*IterProp)->GetSize() - ElementDim);
                        }
                    }

                    if (!bAllowClass) {
                        break;
                    }
                }
            }
        }
        else if (m_GridViewStyle == EGV_SortByName)
        {
            for (TVecPropertyWrapperConstIter Iter = m_VSortedProperties.begin(); !bFind && Iter != m_VSortedProperties.end(); ++Iter)
            {
                if ((*Iter)->pProp->GetGroupName() == pGroup->GroupName)
                {
                    if (InPlainIndex == Counter++)
                    {
                        *OutClass = (*Iter)->pClass;
                        *OutProperty = (*Iter)->pProp;

                        OutMemoryOffset = 0;
                        bFind = true;
                        break;
                    }
                    else if ((*Iter)->pProp && (*Iter)->pProp->GetCtrl() == CTRL_ARRAY && IsDisclosed((*Iter)->pProp->GetName()))
                    {
                        int ElementDim = (*Iter)->pProp->GetSize();
                        int ElementSize = (*Iter)->pProp->GetElementSize();
                        int MemOffset = (*Iter)->pProp->m_MemOffset;

                        while (ElementDim)
                        {
                            // go through child nodes
                            oes::rflex::Property_Base * ChildProp = (*Iter)->pProp->GetNext();

                            while (ChildProp)
                            {
                                if (Counter++ == InPlainIndex)
                                {
                                    *OutClass = (*Iter)->pClass;
                                    *OutProperty = ChildProp;
                                    OutMemoryOffset = (*Iter)->pProp->m_MemOffset + ElementSize * ((*Iter)->pProp->GetSize() - ElementDim);
                                    bFind = true;
                                    break;
                                }
                                MemOffset += ChildProp->m_MemOffset;
                                ChildProp = ChildProp->GetNext();
                            }

                            --ElementDim;
                            MemOffset = (*Iter)->pProp->m_MemOffset + ElementSize * ((*Iter)->pProp->GetSize() - ElementDim);
                        }
                    }
                }
            }
        }
    }
    return bFind;
}

//----------------------------------------------------------------------------------------------
template<class T>
bool CWTLPropertyGrid<T>::IsPropertyBOOL(const oes::rflex::Property_Base *prop)
{
    return prop->PredefinedValues.size() == 2 &&
        prop->PredefinedValues[0] == "true" &&
        prop->PredefinedValues[1] == "false";
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::_Update(const T *pSender, ESystemEventID EventId)
{
    if (EventId == Event_PreRelease) // release selected
    {
    }
    else
    {
        if (EventId == Event_OnSelected)
        {
            SetSelected(const_cast<T*>(pSender));

            PostMessage(WM_USER_DELETE_ALL, 0, 0);
            PostMessage(WM_USER_FILL_PROPS, 0, 0);
        }
        else if (EventId == Event_PropertyChanged)
        {
            PostMessage(WM_USER_UPDATE_PROPS, 0, 0);
        }
    }
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::ClearListProperties()
{
    DeleteAllItems();

    for (TVecPropertyGroupConstIter Iter = m_PropertyGroups.begin(); Iter != m_PropertyGroups.end(); ++Iter)
    {
        for (oes::rflex::TVecPropertyClassIter IterClass = (*Iter)->VecPropertyClasses.begin(); IterClass != (*Iter)->VecPropertyClasses.end(); ++IterClass)
        {
            delete *IterClass;
        }
        delete *Iter;
    }
    m_PropertyGroups.clear();

    for_each(m_VSortedProperties.begin(), m_VSortedProperties.end(), DeleteVectorFntor());

    m_VSortedProperties.clear();
}

//----------------------------------------------------------------------------------------------
template<class T>
BOOL CWTLPropertyGrid<T>::ClickListProperties(LPNMLISTVIEW plvdi)
{
    SFetchData &data = m_cacheFill.at((int)plvdi->iItem);

    switch (data.id)
    {
    case LIST_CLASS_ELEMENT:
    {
        const std::string &className = data.pclass->name;
        std::vector<std::string>::iterator IterCollapsedClass =
            std::find(m_VCollapsedClassNames.begin(), m_VCollapsedClassNames.end(), className);

        if (IterCollapsedClass != m_VCollapsedClassNames.end()) {
            m_VCollapsedClassNames.erase(IterCollapsedClass);
        }
        else {
            m_VCollapsedClassNames.push_back(className);
        }

        m_EditingPropertyIndex = INDEX_NONE;
        m_MemoryOffset = 0;

        DeleteAllItems();

        HideChildControls();
        FillListProperties();
    }break;

    case LIST_PROP_ELEMENT:
    {
        m_EditingPropertyIndex = (int)plvdi->iItem;

        ClickProperty((int)plvdi->iItem);
    }break;

    default:
        assert(false);
        break;
    };
    /*
    TCHAR	szText[256];

    TCITEM tc;
    tc.mask = TCIF_TEXT;
    tc.pszText = szText;
    tc.cchTextMax = 255;

    int SelectedGroup = m_nSelectedGroup;

    SPropertyClass *OutClass = 0;
    Property_Base *OutProperty = 0;
    int OutMemoryOffset = 0;

    if (GetPropertyByIndex((int)plvdi->iItem, SelectedGroup, &OutClass, &OutProperty, OutMemoryOffset))
    {
        if (OutProperty) // property
        {
            ProcessClickListProperties(plvdi, OutClass, OutProperty, OutMemoryOffset);
        }
        else if (OutClass) // Class filter
        {
           
        }
    }
    */
    return TRUE;
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::ClickProperty(int index)
{
    SFetchData &data = m_cacheFill.at(index);

    assert(data.property);

    std::string value;
    if (GetPropertySelectedBatch(data.property, value))
    {
        switch ((data.property->GetCtrl())) // switch property type
        {
        case CTRL_ARRAY:
        {
        }break;

        case CTRL_VECTOR:
        case CTRL_VECTOR2f:
        case CTRL_VECTOR4f:
        case CTRL_MATRIX16:
        case CTRL_MATRIX9:
        case CTRL_EDIT:
        {
            ShowEditWindowControl(index, data, ConvertStringToWideString(value));
        }break;

        case CTRL_EDIT_RESOURCE:
        {
            ShowEditResourceControl(index, data, ConvertStringToWideString(value));
        }break;

        case CTRL_COMBO:
        {
            ShowEditComboControl(index, ConvertStringToWideString(value));
        }break;

        case CTRL_COLOR:
        {
            ShowEditColorControl(index, value);
        }break;

        default:
            assert(false);
            break;
        }
    }
}

//----------------------------------------------------------------------------------------------
template<class T>
BOOL CWTLPropertyGrid<T>::ProcessClickListProperties(LPNMLISTVIEW plvdi, oes::rflex::SPropertyClass *Class,
                                                        Property_Base* Prop, int MemOffset /*= 0*/)
{
    POINT Point;

    if (GetItemPosition(plvdi->iItem, &Point))
    {
        if (m_EditingPropertyIndex != (int)plvdi->iItem)
        {
            m_EditingPropertyIndex = (int)plvdi->iItem; // save index
            m_MemoryOffset = MemOffset;
            HideChildControls();
        }

        wchar_t wbuf[256] = { 0 }; // temp buffers
        char ascii[256] = { 0 };

        int MemoryOffsetOverride = 0;
        if (Class->nOverrideByteShift != -1) { // interface relative shift
            MemoryOffsetOverride = Class->nOverrideByteShift;
        }

        MultiByteToWideChar(CP_ACP, 0, Prop->GetProperty((BYTE*)Class->pDataObject + MemOffset + MemoryOffsetOverride, ascii), -1, wbuf, 256);

        switch ((Prop->GetCtrl())) // switch property type
        {
        case CTRL_ARRAY:
        {
        }break;

        case CTRL_VECTOR:
        case CTRL_VECTOR2f:
        case CTRL_VECTOR4f:
        case CTRL_MATRIX16:
        case CTRL_MATRIX9:
        case CTRL_EDIT:
        {
            unsigned int Width01 = GetColumnWidth(0);
            unsigned int Width02 = GetColumnWidth(1);

            RECT irect;
            GetItemRect(plvdi->iItem, &irect, 0);

            m_pEdit->MoveWindow(Point.x + Width01, Point.y, Width02, irect.bottom - irect.top, TRUE);

            m_pEdit->ShowWindow(SW_SHOW);
            m_pEdit->SendMessage(WM_SETTEXT, 0, (LPARAM)wbuf);
            m_pEdit->SendMessage(EM_SETREADONLY, (WPARAM)(BOOL)Prop->GetPolicy() == READ_ONLY, 0);

            m_pEdit->SetFocus();
            m_pEdit->ShowCaret();

            m_pEdit->SetSel(0, INDEX_NONE); // select all text and move cursor at the end
            m_pEdit->SetSel(INDEX_NONE); //  remove selection

                                         //m_pEdit->Invalidate();
        }break;

        case CTRL_EDIT_RESOURCE:
        {
            unsigned int Width01 = GetColumnWidth(0);
            unsigned int Width02 = GetColumnWidth(1);

            RECT irect;
            GetItemRect(plvdi->iItem, &irect, 0);
            int row_height = irect.bottom - irect.top;

            m_pResourceEdit->MoveWindow(Point.x + Width01, Point.y, Width02 - (row_height * 2), row_height, TRUE);
            m_pResourceEdit->SendMessage(WM_SETTEXT, 0, (LPARAM)wbuf);

            m_pResourceEdit->GetButtonClear()->MoveWindow(Point.x + Width01 + Width02 - (row_height * 2), Point.y, row_height, row_height, TRUE);
            m_pResourceEdit->GetButtonBrowse()->MoveWindow(Point.x + Width01 + Width02 - (row_height * 1), Point.y, row_height, row_height, TRUE);

            m_pResourceEdit->ShowWindow(TRUE);
            m_pResourceEdit->SetFocus();
        }break;

        case CTRL_COMBO:
        {
            if ((int)plvdi->iSubItem == 1)
            {
                if (IsPropertyBOOL(Prop))
                {
                    char Buff[255];

                    int MemoryOffsetOverride = 0;
                    if (Class->nOverrideByteShift != -1) { // interface relative shift
                        MemoryOffsetOverride = Class->nOverrideByteShift;
                    }

                    bool bValue = (strcmp(Prop->GetProperty((BYTE*)Class->pDataObject + MemOffset + MemoryOffsetOverride, Buff), "true") == 0) ? true : false;

                    PushContext();

                    Prop->SetProperty((BYTE*)Class->pDataObject + MemOffset, bValue ? "false" : "true"); // set new value

                    PopContext();

                    ::PostMessage(GetParent(), WM_USER_UPDATE_PROPS, 0, 0);
                }
                else
                {
                    if (m_pComboBox->IsWindowVisible())
                    {
                        m_pComboBox->ShowWindow(SW_HIDE);
                        //SetFocus(m_hwndEditComboBtn);
                    }
                    else
                    {
                        m_pComboBox->DeleteAllItems();

                        size_t NumItems = Prop->PredefinedValues.size();

                        for (size_t IndexValue = 0; IndexValue < NumItems; ++IndexValue)
                        {
                            char Buff[256] = { 0 };
                            wchar_t wbuf2[256] = { 0 };

                            int MemoryOffsetOverride = 0;
                            if (Class->nOverrideByteShift != INDEX_NONE) { // interface relative shift
                                MemoryOffsetOverride = Class->nOverrideByteShift;
                            }

                            //Prop->GetProperty((BYTE*)Class->pDataObject + MemOffset + MemoryOffsetOverride, Buff);
                            MultiByteToWideChar(CP_ACP, 0, Prop->PredefinedValues[IndexValue].c_str(), -1, wbuf2, 256);

                            m_pComboBox->InsertItem(IndexValue, wbuf2);
                        }

                        RECT irc;
                        m_pComboBox->GetItemRect(0, &irc, 0);

                        int row_height = (irc.bottom - irc.top);

                        RECT rc;
                        rc.left = Point.x + GetColumnWidth(0);
                        rc.top = Point.y + row_height;
                        rc.right = rc.left + GetColumnWidth(1);
                        rc.bottom = rc.top + (NumItems * row_height + 4);

                        m_pComboBox->MoveWindow(&rc);

                        RECT rc_b;
                        rc_b.left = Point.x + GetColumnWidth(0) + GetColumnWidth(1) - row_height;
                        rc_b.top = Point.y + row_height - row_height;
                        rc_b.right = rc_b.left + row_height;
                        rc_b.bottom = rc_b.top + row_height;

                        m_pComboBox->GetButton()->MoveWindow(&rc_b);

                        m_pComboBox->ShowWindow(SW_SHOW);
                        m_pComboBox->GetButton()->ShowWindow(SW_SHOW);
                        m_pComboBox->SetFocus();
                    }
                }
            }
        }break;

        case CTRL_COLOR:
        {
            //	var argb  : int = (alpha<<24)|rgb;
            //	var rgb   : int = 0xFFFFFF & argb;
            //	var alpha : int = (argb>>24)&0xFF; 
            DWORD Buffer;
            CHOOSECOLOR cc;
            COLORREF acrCustClr[16];

            ZeroMemory(&cc, sizeof(cc));
            cc.lStructSize = sizeof(cc);
            cc.hwndOwner = m_hWnd;
            cc.lpCustColors = (LPDWORD)acrCustClr;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;

            Buffer = MAKELONG(MAKEWORD(ascii[4], ascii[3]), MAKEWORD(ascii[2], ascii[1])); //boost::lexical_cast<DWORD>(ascii);
            cc.rgbResult = RGB(GetBValue(Buffer), GetGValue(Buffer), GetRValue(Buffer)); // 0x00rrggbb -> 0x00bbggrr

            if (ChooseColor(&cc) == TRUE)
            {
                Buffer = (255 << 24) | RGB(GetBValue(cc.rgbResult), GetGValue(cc.rgbResult), GetRValue(cc.rgbResult)); // 0x00bbggrr to 0x00rrggbb

                char txt[256] = { '\0' };
                sprintf(txt, "%d", Buffer);

                PushContext();

                Prop->SetProperty((BYTE*)Class->pDataObject + MemOffset, txt);

                PopContext();

                UpdatePreview();
            }
        }break;

        default:
            break;
        };
    }
    return TRUE;
}

//----------------------------------------------------------------------------------------------
template<class T>
BOOL CWTLPropertyGrid<T>::ClickListComboProperties(LPNMLISTVIEW plvdi)
{
    m_PropertyCS.enter();

    TCHAR szText[256];

    TCITEM tc;
    tc.mask = TCIF_TEXT;
    tc.pszText = szText;
    tc.cchTextMax = 255;

    int SelectedGroup = m_nSelectedGroup;

    oes::rflex::SPropertyClass *pOutClass = 0;
    oes::rflex::Property_Base *pOutProperty = 0;
    int OutMemoryOffset = 0;

    if (GetPropertyByIndex(m_EditingPropertyIndex, SelectedGroup, &pOutClass, &pOutProperty, OutMemoryOffset))
    {
        if (pOutProperty) // property
        {
            size_t NumItems = pOutProperty->PredefinedValues.size();

            if (plvdi->iSubItem == 0 && (int)plvdi->iItem >= 0 && (int)plvdi->iItem < (int)NumItems)
            {
                int MemoryOffsetOverride = 0;
                if (pOutClass->nOverrideByteShift != INDEX_NONE) { // interface relative shift
                    MemoryOffsetOverride = pOutClass->nOverrideByteShift;
                }

                PushContext();

                pOutProperty->SetProperty((BYTE*)pOutClass->pDataObject + m_MemoryOffset, pOutProperty->PredefinedValues[(int)plvdi->iItem].c_str(), MemoryOffsetOverride); // set new value

                PopContext();
            }
            HideChildControls();
        }
        else if (pOutClass) // Class filter
        {
        }
    }

    m_PropertyCS.leave();
    return TRUE;
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::HideChildControls()
{
    if (m_pEdit->IsWindowVisible())
    {
        m_pEdit->ShowWindow(SW_HIDE);
    }

    if (m_pComboBox->IsWindowVisible()) {
        m_pComboBox->ShowWindow(SW_HIDE);
    }

    if (m_pResourceEdit->IsWindowVisible()) {
        m_pResourceEdit->ShowWindow(SW_HIDE);
    }

    SetFocus();
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::SetGridViewStyle(EGridView Value)
{
    if (m_GridViewStyle != Value)
    {
        m_GridViewStyle = Value;

        DeleteAllItems();

        FillListProperties();
        HideChildControls();
    }
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::LoadHelperImageList()
{
    HBITMAP hBitmap_Checked = NULL;
    HBITMAP hBitmap_Unchecked = NULL;
    HBITMAP hBitmap_Plus = NULL;
    HBITMAP hBitmap_Minus = NULL;

    m_ImageListHelper = ImageList_Create(16, 16, ILC_COLOR32, 255, 1024);

    hBitmap_Checked = (HBITMAP)LoadImage(::GetModuleHandle(NULL), L"resources/checkbox_checked.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
    hBitmap_Unchecked = (HBITMAP)LoadImage(::GetModuleHandle(NULL), L"resources/checkbox_unchecked.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
    hBitmap_Plus = (HBITMAP)LoadImage(::GetModuleHandle(NULL), L"resources/plus.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
    hBitmap_Minus = (HBITMAP)LoadImage(::GetModuleHandle(NULL), L"resources/minus.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);

    ImageList_Add(m_ImageListHelper, hBitmap_Checked, NULL); // Macro: Attach the image, to the image list
    ImageList_Add(m_ImageListHelper, hBitmap_Unchecked, NULL);
    ImageList_Add(m_ImageListHelper, hBitmap_Plus, NULL);
    ImageList_Add(m_ImageListHelper, hBitmap_Minus, NULL);

    HIMAGELIST H = SetImageList(m_ImageListHelper, LVSIL_SMALL);

    DeleteObject(hBitmap_Checked);
    DeleteObject(hBitmap_Unchecked);
    DeleteObject(hBitmap_Plus);
    DeleteObject(hBitmap_Minus);
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::PushContext()
{
    if (m_pRenderContext) {
        m_pAppMain->GetRenderSDK()->GetRenderDriver()->PushContext(m_pRenderContext);
    }
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::PopContext()
{
    if (m_pRenderContext) {
        m_pAppMain->GetRenderSDK()->GetRenderDriver()->PopContext();
    }
}

//----------------------------------------------------------------------------------------------
template<class T>
bool CWTLPropertyGrid<T>::GetPropertySelectedBatch(oes::rflex::Property_Base *prop, std::string &out)
{
    bool bResult = false;

    // disable property if value differ in different objects
    auto selected = m_editor->GetSelected();
    if (!selected.empty())
    {
        bResult = true;

        std::vector<IObjectAbstract*>::const_iterator iter = selected.begin();
        
       /* out = m_editor->GetProperty(*iter, prop);
        
        iter++;

        while (iter != selected.end())
        {
            if (m_editor->GetProperty(*iter, prop) != out)
            {
                out = "";
                bResult = false;
                break;
            }
            ++iter;
        }*/
    }
    return bResult;
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::ShowEditWindowControl(int index, SFetchData &data, const std::wstring &value)
{
    assert(data.property);

    POINT point;

    GetItemPosition(data.id, &point);
    
    const unsigned int width01 = GetColumnWidth(0);
    const unsigned int width02 = GetColumnWidth(1);

    RECT irect;
    GetItemRect(data.id, &irect, 0);

    m_pEdit->MoveWindow(point.x + width01, point.y, width02, irect.bottom - irect.top, TRUE);

    m_pEdit->ShowWindow(SW_SHOW);
    m_pEdit->SendMessage(WM_SETTEXT, 0, (LPARAM)value.c_str());
    m_pEdit->SendMessage(EM_SETREADONLY, (WPARAM)(BOOL)data.property->GetPolicy() == READ_ONLY, 0);

    m_pEdit->SetFocus();
    m_pEdit->ShowCaret();

    m_pEdit->SetSel(0, INDEX_NONE); // select all text and move cursor at the end
    m_pEdit->SetSel(INDEX_NONE); //  remove selection
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::ShowEditResourceControl(int index, SFetchData &data, const std::wstring &value)
{
    assert(data.property);

    POINT point;

    GetItemPosition(index, &point);

    const unsigned int width01 = GetColumnWidth(0);
    const unsigned int width02 = GetColumnWidth(1);

    RECT irect;
    GetItemRect(data.id, &irect, 0);
    int row_height = irect.bottom - irect.top;

    m_pResourceEdit->MoveWindow(point.x + width01, point.y, width02 - (row_height * 2), row_height, TRUE);
    m_pResourceEdit->SendMessage(WM_SETTEXT, 0, (LPARAM)value.c_str());

    m_pResourceEdit->GetButtonClear()->MoveWindow(point.x + width01 + width02 - (row_height * 2), point.y, row_height, row_height, TRUE);
    m_pResourceEdit->GetButtonBrowse()->MoveWindow(point.x + width01 + width02 - (row_height * 1), point.y, row_height, row_height, TRUE);

    m_pResourceEdit->ShowWindow(TRUE);
    m_pResourceEdit->SetFocus();
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::ShowEditColorControl(int index, const std::string &value)
{
    //	var argb  : int = (alpha<<24)|rgb;
    //	var rgb   : int = 0xFFFFFF & argb;
    //	var alpha : int = (argb>>24)&0xFF; 
    
    DWORD buffer;
    CHOOSECOLOR cc;
    COLORREF acrCustClr[16];

    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = m_hWnd;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    buffer = MAKELONG(MAKEWORD(value[4], value[3]), MAKEWORD(value[2], value[1]));
    cc.rgbResult = RGB(GetBValue(buffer), GetGValue(buffer), GetRValue(buffer)); // 0x00rrggbb -> 0x00bbggrr

    if (ChooseColor(&cc) == TRUE)
    {
        char newValue[256] = { '\0' };

        buffer = (255 << 24) | RGB(GetBValue(cc.rgbResult), GetGValue(cc.rgbResult), GetRValue(cc.rgbResult)); // 0x00bbggrr to 0x00rrggbb

        sprintf(newValue, "%d", buffer);

        SFetchData &data = m_cacheFill.at(index);

//        m_editor->SetProperty(newValue, value, m_editor->GetSelected(), data.property);

//        UpdatePreview();
    }
}

//----------------------------------------------------------------------------------------------
template<class T>
void CWTLPropertyGrid<T>::ShowEditComboControl(int index, const std::wstring &value)
{
    SFetchData &data = m_cacheFill.at(index);

    assert(data.property);

    if (IsPropertyBOOL(data.property))
    {
        bool oldValue = (value == TEXT("true"));
        bool newValue = !oldValue;

//        m_editor->SetProperty(newValue ? "true" : "false", oldValue ? "true" : "false", m_editor->GetSelected(), data.property);

        ::PostMessage(GetParent(), WM_USER_UPDATE_PROPS, 0, 0);
    }
    else
    {

    }
}
