#include "LauncherStdAfx.h"
/*
#define TAB_HEIGHT							25
#define LIST_BOTTOM							2
#define ID_BUTTON_CATEGORIZED				983
#define ID_BUTTON_SORT						984

const int nTopHeight = 30;

//----------------------------------------------------------------------------------------------
Win32PropertyGrid::Win32PropertyGrid(HWND hWndParent, const CWindowManager *WMgr)
: m_hwndParent(hWndParent)
, m_pWindowManager(WMgr)
, m_EditingPropertyIndex(INDEX_NONE)
, m_MemoryOffset(0)
, GridViewStyle(EGV_Categorized)
{
	// parents
	RECT parentRect;
	GetClientRect(hWndParent, &parentRect);

	int parentWidth = parentRect.right - parentRect.left;
	int parentHeight = parentRect.bottom - parentRect.top;

	CLIENTCREATESTRUCT MDIClientCreateStruct; // Structure to be used for MDI client area

	MDIClientCreateStruct.hWindowMenu = NULL;
	MDIClientCreateStruct.idFirstChild = IDM_FIRSTCHILD;

	m_hWndTop = CreateWindowEx(WS_EX_CLIENTEDGE,
		TEXT("MDIClient"), NULL,  
		WS_CHILD | WS_VISIBLE | SS_SUNKEN, 
		parentRect.left,
		parentRect.top,
		parentWidth,
		nTopHeight, 
		hWndParent,
		NULL,
		GetModuleHandle(NULL),
		(void*) &MDIClientCreateStruct);

	m_hWndBottom = CreateWindowEx(WS_EX_CLIENTEDGE,
		TEXT("MDIClient"), NULL,  
		WS_CHILD | WS_VISIBLE | SS_SUNKEN, 
		parentRect.left,
		nTopHeight,
		parentWidth,
		parentHeight - nTopHeight,
		hWndParent,
		NULL,
		GetModuleHandle(NULL),
		(void*) &MDIClientCreateStruct);

	TBBUTTON CustomButtons[]=
	{
		{1, ID_BUTTON_CATEGORIZED, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0, 0, 0},
		{2, ID_BUTTON_SORT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0, 0, 0},
		{-1, 0, 0, TBSTYLE_SEP, 0L, 0, 0, 0},
	};

	HBITMAP hBitmapProp = (HBITMAP)LoadImage(::GetModuleHandle(NULL), L"resources/properties.bmp", IMAGE_BITMAP, 0, 0, LR_SHARED|LR_DEFAULTSIZE|LR_LOADFROMFILE);

	HWND hToolbarWnd = CreateToolbarEx
		(m_hWndTop,	 // hwnd - Handle to the parent window for the toolbar. 
		WS_CHILD |	 // ws - Window styles for the toolbar. This parameter must specify at least the WS_CHILD style.
		WS_VISIBLE |
		TBSTYLE_TOOLTIPS | 
		//TBSTYLE_FLAT |
		//CCS_NORESIZE |
		CCS_NODIVIDER,
		//|TBSTYLE_TOOLTIPS WS_CHILD|WS_VISIBLE // |CCS_NORESIZE|CCS_NODIVIDER| ,
		170, // wID - Control identifier for the toolbar. 
		4, // nBitmaps - Number of button images contained in the bitmap specified by hBMInst and wBMID.
		NULL,//(HINSTANCE)hInstance, // hBMInst - Module instance with the executable file that contains the bitmap resource
		(UINT)hBitmapProp,//IDB_BITMAP2, // wBMID - Resource identifier for the bitmap resource. If hBMInst is NULL, this parameter must be a valid bitmap handle. 
		CustomButtons, // lpButtons - Address of an array of TBBUTTON structures that contain information about the buttons to add to the toolbar. 
		2, // iNumButtons - Number of buttons to add to the toolbar. 
		0, // dxButton - Width, in pixels, of the buttons to add to the toolbar. 
		0, // dyButton - Height, in pixels, of the buttons to add to the toolbar. 
		16, // dxBitmap - Width, in pixels, of the button images to add to the buttons in the toolbar. 
		16, // dyBitmap - Height, in pixels, of the button images to add to the buttons in the toolbar. 
		sizeof(TBBUTTON) // uStructSize - Size of a TBBUTTON structure. 
		);
	
	SetWindowLong(m_hWndTop, GWL_WNDPROC, (LONG)ProcPropWndTop);
	SetWindowLong(m_hWndBottom, GWL_WNDPROC, (LONG)ProcPropWndBottom);

	// bottom
	RECT rect;
	GetClientRect(m_hWndBottom, &rect);

	int Width = rect.right - rect.left;
	int Height = rect.bottom - rect.top;

	m_hwndTab = CreateWindow(WC_TABCONTROL,
		NULL,
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
		rect.left,
		rect.top,
		Width,
		TAB_HEIGHT,
		m_hWndBottom,
		NULL,
		(HINSTANCE)GetWindowLong(m_hWndBottom, GWL_HINSTANCE),
		NULL);

	m_hwndListProperty = CreateWindow(
		WC_LISTVIEW,
		NULL,
		WS_CHILD | LVS_REPORT | WS_VISIBLE,
		rect.left,
		TAB_HEIGHT,
		Width,
		Height - TAB_HEIGHT - LIST_BOTTOM,
		m_hWndBottom,
		NULL,
		(HINSTANCE)GetWindowLong(m_hWndBottom, GWL_HINSTANCE),
		NULL);

	LVCOLUMN lvc0, lvc1;
	memset(&lvc0, 0, sizeof(LVCOLUMN));
	lvc0.iSubItem = 0;
	lvc0.pszText = L"Property";
	lvc0.cx = 140;     // width of column in pixels
	lvc0.fmt = LVCFMT_LEFT;
	lvc0.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	memset(&lvc1, 0, sizeof(LVCOLUMN));
	lvc1.iSubItem = 1;
	lvc1.pszText = L"Value";
	lvc1.cx = Width - lvc0.cx - 22;     // width of column in pixels
	lvc1.fmt = LVCFMT_LEFT;
	lvc1.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_IMAGE | LVCF_SUBITEM;

	int k = ListView_InsertColumn(m_hwndListProperty, 0, &lvc0);
	int k1 = ListView_InsertColumn(m_hwndListProperty, 1, &lvc1);
	
	TCITEM tc = {0};
	tc.mask = TCIF_TEXT;
	tc.pszText = L"Value";
	TabCtrl_InsertItem(m_hwndTab, 0, &tc);

	ListView_SetExtendedListViewStyle(m_hwndListProperty, LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES);

	LoadHelperImageList();

	//ListView_SetExtendedListViewStyle(m_hwndListProperty, LVS_EX_DOUBLEBUFFER);

	// create sibling editbox
	m_hwndEditEditbox = CreateWindow(L"EDIT",
		NULL,
		WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL,
		0, 0, 0, 0,
		m_hwndListProperty, 
		(HMENU) (HINSTANCE)GetWindowLong(m_hwndListProperty, GWL_HINSTANCE), 
		NULL,
		NULL);

	m_hwndEditComboList = CreateWindow(WC_LISTVIEW,
		NULL,
		WS_CHILD | LVS_REPORT | WS_VISIBLE | LVS_NOCOLUMNHEADER,
		0, 0, 0, 0,
		m_hwndListProperty, 
		(HMENU) (HINSTANCE)GetWindowLong(m_hwndListProperty, GWL_HINSTANCE), 
		NULL,
		NULL);

	ListView_SetExtendedListViewStyle(m_hwndEditComboList, LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES);

	LVCOLUMN lvc2;
	memset(&lvc2, 0, sizeof(LVCOLUMN));
	lvc2.iSubItem = 0;
	lvc2.cx = 140;     // width of column in pixels
	lvc2.fmt = LVCFMT_LEFT;
	lvc2.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;

	ListView_InsertColumn(m_hwndEditComboList, 0, &lvc2);

	m_hwndEditComboBtn = CreateWindow(L"BUTTON",
		NULL,
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		0, 0, 0, 0,
		m_hwndListProperty,
		(HMENU) (HINSTANCE)GetWindowLong(m_hwndListProperty, GWL_HINSTANCE),
		NULL,
		NULL);

	m_hwndEditEditboxBtnBrowse = CreateWindow(L"BUTTON",
		NULL,
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,// | BS_BITMAP
		0, 0, 0, 0,
		m_hwndListProperty, 
		(HMENU) (HINSTANCE)GetWindowLong(m_hwndListProperty, GWL_HINSTANCE),
		NULL,
		NULL);

	m_hwndEditEditboxBtnClear = CreateWindow(L"BUTTON",
		NULL,
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE | BS_BITMAP,
		0, 0, 0, 0,
		m_hwndListProperty,
		(HMENU) (HINSTANCE)GetWindowLong(m_hwndListProperty, GWL_HINSTANCE),
		NULL,
		NULL);

	SetWindowLong(m_hWndTop, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hWndBottom, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hwndEditEditbox, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hwndListProperty, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hwndEditEditbox, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hwndEditComboList, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hwndEditComboBtn, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hwndEditEditboxBtnBrowse, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hwndEditEditboxBtnClear, GWL_USERDATA, (LONG)this);

	SendMessage(m_hwndTab, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
	SendMessage(m_hwndListProperty, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
	SendMessage(m_hwndEditEditbox, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
	SendMessage(m_hwndEditComboList, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
	SendMessage(m_hwndEditComboBtn, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
	SendMessage(m_hwndEditEditboxBtnBrowse, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));
	SendMessage(m_hwndEditEditboxBtnClear, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), MAKELPARAM(TRUE, 0));

	m_lpfnListProc				= (WNDPROC) SetWindowLong(m_hwndListProperty, GWL_WNDPROC, (LONG)SubClassProcList);
	m_lpfnEditboxWndProc		= (WNDPROC) SetWindowLong(m_hwndEditEditbox, GWL_WNDPROC, (LONG)SubClassProcEdit);
	m_lpfnComboboxListWndProc	= (WNDPROC) SetWindowLong(m_hwndEditComboList, GWL_WNDPROC, (LONG)SubClassProcComboList);
	m_lpfnComboboxBtnWndProc	= (WNDPROC) SetWindowLong(m_hwndEditComboBtn, GWL_WNDPROC, (LONG)SubClassProcComboBtn);
	m_lpfnEditEditboxBtnBrowse	= (WNDPROC) SetWindowLong(m_hwndEditEditboxBtnBrowse, GWL_WNDPROC, (LONG)SubClassProcEditBtn);
	m_lpfnEditEditboxBtnClear	= (WNDPROC) SetWindowLong(m_hwndEditEditboxBtnClear, GWL_WNDPROC, (LONG)SubClassProcEditBtnClear);

	SendMessage(m_hwndEditComboBtn,  WM_SETTEXT, 0, (LPARAM)L"...");

	HideChildControls();
}

//----------------------------------------------------------------------------------------------
Win32PropertyGrid::~Win32PropertyGrid()
{
	ClearListProperties();
	VCollapsedClassNames.clear();
}

//----------------------------------------------------------------------------------------------
void Win32PropertyGrid::LoadHelperImageList()
{
	HBITMAP hBitmap_Checked = NULL;
	HBITMAP hBitmap_Unchecked = NULL;
	HBITMAP hBitmap_Plus = NULL;
	HBITMAP hBitmap_Minus = NULL;

	ImageListHelper = ImageList_Create(16, 16, ILC_COLOR32, 255, 1024);

	hBitmap_Checked = (HBITMAP)LoadImage(::GetModuleHandle(NULL), L"resources/checkbox_checked.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	hBitmap_Unchecked = (HBITMAP)LoadImage(::GetModuleHandle(NULL), L"resources/checkbox_unchecked.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	hBitmap_Plus = (HBITMAP)LoadImage(::GetModuleHandle(NULL), L"resources/plus.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	hBitmap_Minus = (HBITMAP)LoadImage(::GetModuleHandle(NULL), L"resources/minus.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);

	ImageList_Add(ImageListHelper, hBitmap_Checked, NULL); // Macro: Attach the image, to the image list
	ImageList_Add(ImageListHelper, hBitmap_Unchecked, NULL);
	ImageList_Add(ImageListHelper, hBitmap_Plus, NULL);
	ImageList_Add(ImageListHelper, hBitmap_Minus, NULL);

	HIMAGELIST H = ListView_SetImageList(m_hwndListProperty, ImageListHelper, LVSIL_SMALL);

	DeleteObject(hBitmap_Checked);
	DeleteObject(hBitmap_Unchecked);
	DeleteObject(hBitmap_Plus);
	DeleteObject(hBitmap_Minus);
}

//----------------------------------------------------------------------------------------------
bool Win32PropertyGrid::AddTabPage(std::string &Value, int Index)
{
	wchar_t wbuf[256] = {0}; // temp buffers
	char ascii[256] = {0};

	MultiByteToWideChar(CP_ACP, 0, Value.c_str(), INDEX_NONE, wbuf, 256);

	TCITEM tc = {0};
	tc.mask = TCIF_TEXT;
	tc.pszText = wbuf;
	TabCtrl_InsertItem(m_hwndTab, Index, &tc);
	return true;
}

//----------------------------------------------------------------------------------------------
bool Win32PropertyGrid::ClearTabPages()
{
	TabCtrl_DeleteAllItems(m_hwndTab);
	return true;
}

//----------------------------------------------------------------------------------------------
void Win32PropertyGrid::FillPropertyData(CActor *Actor)
{
	PropertyCS.enter();

	m_pSelectedActor = Actor;

	AppClassTree &classTree = NObjectFactory::GetClassTree();

	if (ClassNode *pClassNode = classTree.Find(Actor->GetType()))
	{
		while (pClassNode)
		{
			TVecPropertyConstIterator IterProp = pClassNode->PropertyMap.begin();

			while (IterProp != pClassNode->PropertyMap.end())
			{
				// find or alloc group
				PropertyGroup *pGroup = GetGroupByName((*IterProp)->GetGroupName());
				if (!pGroup)
				{
					pGroup = new PropertyGroup((*IterProp)->GetGroupName());

					if (pGroup->GroupName == "Value"){
						PropertyGroups.insert(PropertyGroups.begin(), pGroup);
					}else{
						PropertyGroups.push_back(pGroup);
					}
				}

				// find or alloc class
				SPropertyClass *pClass = 0;
				TVecPropertyClassIter IterClass = pGroup->VecPropertyClasses.begin();

				while (IterClass != pGroup->VecPropertyClasses.end())
				{
					if ((*IterClass)->ClassName == (*IterProp)->GetClassName())
					{
						pClass = *IterClass;
						break;
					}
					++IterClass;
				}

				if (!pClass)
				{
					pClass = new SPropertyClass((*IterProp)->GetClassName(), (void*)Actor);
					pGroup->VecPropertyClasses.insert(pGroup->VecPropertyClasses.begin(), pClass);
					pClass->InheritProperties.push_back(NULL); // separator
				}

				pClass->InheritProperties.push_back(*IterProp);

				// add sorted array (for non grouped mode)
				VSortedProperties.push_back(new SPropertyWrapper(*IterProp, pClass));

				++IterProp;
			}

			// add interface properties
			ClassNode::TVecInterfaceIter IterIntf = pClassNode->m_VecInterfaces.begin();

			while (IterIntf != pClassNode->m_VecInterfaces.end())
			{
				ClassNode *pNodeInterface = classTree.FindInterface((*IterIntf)->strType);
				
				if (pNodeInterface)
				{
					TVecPropertyConstIterator IterIntfProp = pNodeInterface->PropertyMap.begin();

					while (IterIntfProp != pNodeInterface->PropertyMap.end())
					{
						// find or alloc group
						PropertyGroup *pGroup = GetGroupByName("Value");
						assert(pGroup);

						// find or alloc class
						SPropertyClass *pClass = 0;
						TVecPropertyClassIter IterClass = pGroup->VecPropertyClasses.begin();

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
							pClass = new SPropertyClass((*IterIntf)->strType, (void*)Actor, (*IterIntf)->byteShift);
							pGroup->VecPropertyClasses.insert(pGroup->VecPropertyClasses.begin(), pClass);
							pClass->InheritProperties.push_back(NULL);
						}

						pClass->InheritProperties.push_back(*IterIntfProp);

						// add sorted array (for non grouped mode)
						VSortedProperties.push_back(new SPropertyWrapper(*IterIntfProp, pClass));

						++IterIntfProp;
					}
				}
				++IterIntf;
			}

			pClassNode = pClassNode->GetRootNode();
		}
	}

	const Registry * Reg = m_pWindowManager->GetCoreSDK()->GetRegistry();

	// transient properties
	for (TVecActorChildConstIterator IterActor = Actor->m_ChildNodes.begin(); IterActor != Actor->m_ChildNodes.end(); ++IterActor)
	{
		if ((*IterActor)->IsTransient() &&
			!Reg->IsEditorVisible((*IterActor)->GetType()) &&
			!dynamic_cast<const Brush_AbstractInterface*>(*IterActor))
		{
			FillPropertyDataTransient(*IterActor);
		}
	}
	
	std::sort(VSortedProperties.begin(), VSortedProperties.end(), SPropertyWrapper::CompByName);

	int IndexGroup = 0;
	TVecPropertyGroupConstIter IterGroup = PropertyGroups.begin();
	while (IterGroup != PropertyGroups.end())
	{
		AddTabPage((*IterGroup)->GroupName, IndexGroup++);
		++IterGroup;
	}

	PropertyCS.leave();
}

//----------------------------------------------------------------------------------------------
void Win32PropertyGrid::FillPropertyDataTransient(CActor * Actor)
{
	AppClassTree &CTree = NObjectFactory::GetClassTree();

	if (ClassNode *CNode = CTree.Find(Actor->GetType()))
	{
		SPropertyClass *pClassRoot = new SPropertyClass(CNode->Name.c_str(), (void*)Actor);
		pClassRoot->InheritProperties.push_back(NULL);

		PropertyGroup *pGroupValue = GetGroupByName("Value");
		assert(pGroupValue);

		pGroupValue->VecPropertyClasses.push_back(pClassRoot);

		while (CNode)
		{
			TVecPropertyConstIterator IterProp = CNode->PropertyMap.begin();
			while (IterProp != CNode->PropertyMap.end())
			{
				//if ((*IterProp)->IsSerializable())
				{
					SPropertyClass *pClass = pClassRoot;

					// find or alloc group
					PropertyGroup *pGroup = GetGroupByName((*IterProp)->GetGroupName());
					if (!pGroup)
					{
						pGroup = new PropertyGroup((*IterProp)->GetGroupName());
						pClass = new SPropertyClass(CNode->Name.c_str(), (void*)Actor);

						pGroup->VecPropertyClasses.push_back(pClass);
						pClass->InheritProperties.push_back(NULL);

						PropertyGroups.push_back(pGroup);
					}

					pClass->InheritProperties.push_back(*IterProp);

					// add sorted array
					VSortedProperties.push_back(new SPropertyWrapper(*IterProp, pClass));
				}
				++IterProp;
			}
			CNode = CNode->GetRootNode();
		}
	}

	const Registry * Reg = m_pWindowManager->GetCoreSDK()->GetRegistry();
	// transient properties
	for (TVecActorChildConstIterator IterActor = Actor->m_ChildNodes.begin(); IterActor != Actor->m_ChildNodes.end(); ++IterActor)
	{
		if ((*IterActor)->IsTransient() &&
			!Reg->IsEditorVisible((*IterActor)->GetType()) &&
			!dynamic_cast<const Brush_AbstractInterface*>(*IterActor))
		{
			FillPropertyDataTransient(*IterActor);
		}
	}
}
//----------------------------------------------------------------------------------------------
void Win32PropertyGrid::UpdatePreview()
{
	PropertyCS.enter();
	
	TCHAR	szText[256];
	int SelectedGroup = TabCtrl_GetCurSel(m_hwndTab);

	TCITEM tc;
	tc.mask = TCIF_TEXT;
	tc.pszText = szText;
	tc.cchTextMax = 255;

	if (SelectedGroup != INDEX_NONE && TabCtrl_GetItem(m_hwndTab, SelectedGroup, &tc))
	{
		std::string StrGroupName = ConvertWideStringToString(szText);

		// find group
		PropertyGroup * pGroup = GetGroupByName(StrGroupName);
		assert(pGroup);

		int PropertyListIndex = 0;

		::LockWindowUpdate(m_hwndListProperty);

		if (GridViewStyle == EGV_Categorized)
		{
			for (TVecPropertyClassIter IterClass = pGroup->VecPropertyClasses.begin(); IterClass != pGroup->VecPropertyClasses.end(); ++IterClass)
			{
				for (TVecPropertyBaseIter IterProp = (*IterClass)->InheritProperties.begin(); IterProp != (*IterClass)->InheritProperties.end(); ++IterProp)
				{
					ListView_Update(m_hwndListProperty, PropertyListIndex++);
				}
			}
		}
		else if (GridViewStyle == EGV_SortByName)
		{
			for (TVecPropertyWrapperIter Iter = VSortedProperties.begin(); Iter != VSortedProperties.end(); ++Iter)
			{
				if ((*Iter)->pProp->GetGroupName() == pGroup->GroupName)
				{
					ListView_Update(m_hwndListProperty, PropertyListIndex++);
				}
			}
		}
		::LockWindowUpdate(NULL);
	}
	PropertyCS.leave();
}

//----------------------------------------------------------------------------------------------
bool Win32PropertyGrid::ProcessTabCtrl(HWND hwnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	ListView_DeleteAllItems(m_hwndListProperty);
	FillListProperties();
	HideChildControls();
	return true;
}

//----------------------------------------------------------------------------------------------
void Win32PropertyGrid::SetGridViewStyle(EGridView Value)
{
	if (GridViewStyle != Value)
	{
		GridViewStyle = Value;

		ListView_DeleteAllItems(m_hwndListProperty);
		FillListProperties();
		HideChildControls();
	}
}

//----------------------------------------------------------------------------------------------
void Win32PropertyGrid::Redraw() const
{
	InvalidateRect(m_hwndTab, NULL, TRUE);
	InvalidateRect(m_hwndListProperty, NULL, TRUE);
}

//----------------------------------------------------------------------------------------------
BOOL Win32PropertyGrid::SubClassProcList(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	Win32PropertyGrid * pGrid = reinterpret_cast<Win32PropertyGrid*>(GetWindowLong(hWnd, GWL_USERDATA));

	switch (messg) 
	{
	case WM_CREATE:
		{
		}break;

	case WM_COMMAND:
		{
		}break;

	case WM_MOUSEWHEEL:
	case WM_VSCROLL:
		{
			pGrid->HideChildControls();
		}break;

	case WM_NOTIFY:
		{
			{
				long lResult = pGrid->HandleNotifyCombobox(hWnd, (int)wParam, (LPNMHDR)lParam); // handle customdraw combobox
				if (lResult > 0)
				{
					SetWindowLong(pGrid->GetHWNDParent(), DWL_MSGRESULT, lResult); 
					return lResult;
				}
			}

			switch (((LPNMHDR)lParam)->code)
			{
				// process fill list
				case LVN_GETDISPINFO:
				{
					if (((LPNMHDR)lParam)->hwndFrom == pGrid->GetHWNDCombobox())
					{
						pGrid->GETDISPINFO_FillComboList(lParam);
					}
				}break;

				// process fill list
				case NM_CLICK:
				{
					if (((LPNMHDR)lParam)->hwndFrom == pGrid->GetHWNDCombobox())
					{
						pGrid->ClickComboList((LPNMLISTVIEW)lParam);
					}
				}break;
			};
		}break;
	}

	// Call the original window procedure for default processing. 
	return CallWindowProc(pGrid->m_lpfnListProc, hWnd, messg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
BOOL Win32PropertyGrid::SubClassProcComboBtn(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Win32PropertyGrid * pGrid = reinterpret_cast<Win32PropertyGrid*>(GetWindowLong(hWnd, GWL_USERDATA));

	switch(msg)
	{
	case WM_LBUTTONDOWN:
		{
		}break;

	case WM_LBUTTONUP:
		{
			tagNMLISTVIEW tag;
			tag.iItem = ListView_GetSelectionMark(pGrid->GetHWNDListProperty());
			tag.iSubItem = 1;
			pGrid->ClickListProperties(&tag);			
		}break;
	}
	// Call the original window procedure for default processing. 
	return CallWindowProc(pGrid->m_lpfnComboboxBtnWndProc, hWnd, msg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
BOOL Win32PropertyGrid::SubClassProcComboList(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Win32PropertyGrid * pGrid = reinterpret_cast<Win32PropertyGrid*>(GetWindowLong(hwnd, GWL_USERDATA));

	BOOL bResult = FALSE;

	switch (msg) 
	{
	case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
			case LVN_GETDISPINFO:
				{
					if (((LPNMHDR)lParam)->hwndFrom == pGrid->GetHWNDCombobox())
					{
						pGrid->GETDISPINFO_FillComboList(lParam);
					}
				};
			};
		}break;

	case WM_KEYDOWN:
		{
			switch ((DWORD)wParam) 
			{
			case VK_ESCAPE:
				pGrid->m_EditingPropertyIndex = INDEX_NONE;
				pGrid->m_MemoryOffset = 0;
				pGrid->HideChildControls();
				break;
			};
		}break;
	}

	// Call the original window procedure for default processing. 
	return CallWindowProc(pGrid->m_lpfnComboboxListWndProc, hwnd, msg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
BOOL Win32PropertyGrid::SubClassProcEdit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Win32PropertyGrid * pGrid = reinterpret_cast<Win32PropertyGrid*>(GetWindowLong(hwnd, GWL_USERDATA));

	switch(msg)
	{
	case WM_MEASUREITEM: 
		{
			LPMEASUREITEMSTRUCT lpmis; 
			lpmis = (LPMEASUREITEMSTRUCT) lParam;
			lpmis->itemHeight = 8;
		}break;

	case WM_KEYDOWN:
		switch ((DWORD)wParam) 
		{
		case VK_ESCAPE:
			pGrid->m_EditingPropertyIndex = INDEX_NONE;
			pGrid->m_MemoryOffset = 0;
			pGrid->HideChildControls();
			break;

		case VK_RETURN:
			{
				pGrid->PropertyCS.enter();

				TCHAR	szText[256];
				int SelectedGroup = TabCtrl_GetCurSel(pGrid->GetHWNDTab());

				TCITEM tc;
				tc.mask = TCIF_TEXT;
				tc.pszText = szText;
				tc.cchTextMax = 255;

				SPropertyClass * OutClass = 0;
				Property_Base * OutProperty = 0;
				int OutMemoryOffset = 0;

				if (pGrid->GetPropertyByIndex(pGrid->m_EditingPropertyIndex, SelectedGroup, &OutClass, &OutProperty, OutMemoryOffset))
				{
					if (OutProperty && OutClass)
					{
						wchar_t wbuf[256] = {0};
						char ascii[256] = {0};
						
						SendMessage(pGrid->GetHWNDEditbox(), WM_GETTEXT, 256, (LPARAM)wbuf);
						WideCharToMultiByte(CP_ACP, 0, wbuf, 256, ascii, 256, NULL, NULL);
						 
						int MemoryOffsetOverride = 0;
						if (OutClass->nOverrideByteShift != -1){ // interface relative shift
							MemoryOffsetOverride = OutClass->nOverrideByteShift;
						}
						 
						OutProperty->SetProperty((BYTE*)OutClass->pDataObject + OutMemoryOffset, ascii, MemoryOffsetOverride); // set new value
					}
				}

				pGrid->PropertyCS.leave();
				pGrid->m_EditingPropertyIndex = INDEX_NONE;
				pGrid->m_MemoryOffset = 0;
				pGrid->HideChildControls();
				pGrid->UpdatePreview();
			}break;
		};
	};

	// Call the original window procedure for default processing. 
	return CallWindowProc(pGrid->m_lpfnEditboxWndProc, hwnd, msg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
BOOL Win32PropertyGrid::SubClassProcEditBtn(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Win32PropertyGrid * pGrid = reinterpret_cast<Win32PropertyGrid*>(GetWindowLong(hwnd, GWL_USERDATA));

	switch(msg)
	{
	case WM_LBUTTONDOWN:
		{
		}break;

	case WM_LBUTTONUP:
		{
			// check cursor hit
			OPENFILENAME ofn;
			WCHAR szFileName[MAX_PATH] = L"";
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn); 
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = L"Bitmap Files (*.tga)\0*.tga\0DDS Files (*.dds)\0*.dds\0All Files (*.*)\0*.*\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = L"txt";

			CHAR chFileName[MAX_PATH] = "";
			if (GetOpenFileName(&ofn) && ConvertWideStringToAnsiCch(chFileName, szFileName, MAX_PATH))
			{
				pGrid->PropertyCS.enter();

				TCHAR	szText[256];
				int SelectedGroup = TabCtrl_GetCurSel(pGrid->GetHWNDTab());

				TCITEM tc;
				tc.mask = TCIF_TEXT;
				tc.pszText = szText;
				tc.cchTextMax = 255;

				SPropertyClass * OutClass = 0;
				Property_Base * OutProperty = 0;
				int OutMemoryOffset = 0;

				if (pGrid->GetPropertyByIndex(pGrid->m_EditingPropertyIndex, SelectedGroup, &OutClass, &OutProperty, OutMemoryOffset))
				{
					if (OutProperty && OutClass)
					{
						std::string ResourceTag(chFileName);
						std::string::size_type pos0 = ResourceTag.rfind("\\");
						ResourceTag = ResourceTag.substr((pos0 + 1));

						OutProperty->SetProperty((BYTE*)OutClass->pDataObject + OutMemoryOffset, ResourceTag.c_str()); // set new value
					}
				}
			}
			pGrid->m_EditingPropertyIndex = INDEX_NONE;
			pGrid->m_MemoryOffset = 0;
			pGrid->HideChildControls();
		}break;

	default:
		break;
	};
	// Call the original window procedure for default processing. 
	return CallWindowProc(pGrid->m_lpfnEditEditboxBtnBrowse, hwnd, msg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
BOOL Win32PropertyGrid::SubClassProcEditBtnClear(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Win32PropertyGrid * pGrid = reinterpret_cast<Win32PropertyGrid*>(GetWindowLong(hwnd, GWL_USERDATA));

	switch(message)
	{
	case WM_LBUTTONDOWN:
		{
		}break;

	case WM_LBUTTONUP:
		{
			pGrid->PropertyCS.enter();

			TCHAR	szText[256];
			int SelectedGroup = TabCtrl_GetCurSel(pGrid->GetHWNDTab());

			TCITEM tc;
			tc.mask = TCIF_TEXT;
			tc.pszText = szText;
			tc.cchTextMax = 255;

			SPropertyClass * OutClass = 0;
			Property_Base * OutProperty = 0;
			int OutMemoryOffset = 0;

			if (pGrid->GetPropertyByIndex(pGrid->m_EditingPropertyIndex, SelectedGroup, &OutClass, &OutProperty, OutMemoryOffset))
			{
				if (OutProperty && OutClass)
				{
					OutProperty->SetProperty((BYTE*)OutClass->pDataObject + OutMemoryOffset, "none"); // set new value
				}
			}

			pGrid->PropertyCS.leave();

			pGrid->m_EditingPropertyIndex = INDEX_NONE;
			pGrid->m_MemoryOffset = 0;
			
			pGrid->HideChildControls();
		}break;
	default:
		break;
	};
	// Call the original window procedure for default processing. 
	return CallWindowProc(pGrid->m_lpfnEditEditboxBtnClear, hwnd, message, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
BOOL Win32PropertyGrid::SubClassProcEditBtnArrayAdd(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

//----------------------------------------------------------------------------------------------
BOOL Win32PropertyGrid::SubClassProcEditBtnArrayDel(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

//----------------------------------------------------------------------------------------------
LRESULT CALLBACK Win32PropertyGrid::ProcPropWndTop(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Win32PropertyGrid *pSelf = reinterpret_cast<Win32PropertyGrid*>(GetWindowLong(hWnd, GWL_USERDATA));

	switch (message) 
	{
	case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);

			int Width = rect.right - rect.left;
			int Height = rect.bottom - rect.top;

			//MoveWindow(m_hwndTool, rect.left, rect.top, Width, Height, TRUE);
		}break;

	case WM_COMMAND:
	case BN_CLICKED:
		{
			INT_PTR buttonId = LOWORD(wParam);
			if (buttonId == ID_BUTTON_CATEGORIZED)
			{
				pSelf->SetGridViewStyle(Win32PropertyGrid::EGV_Categorized);
			}
			else if (buttonId == ID_BUTTON_SORT)
			{
				pSelf->SetGridViewStyle(Win32PropertyGrid::EGV_SortByName);
			}
		}break;

	default:
		break;
	};
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
LRESULT CALLBACK Win32PropertyGrid::ProcPropWndBottom(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Win32PropertyGrid *pSelf = reinterpret_cast<Win32PropertyGrid*>(GetWindowLong(hWnd, GWL_USERDATA));

	if (pSelf)
	{
		switch (message) 
		{
		case WM_SIZE:
			{
				if (pSelf){
					pSelf->Size(hWnd, message, wParam, lParam);
				}
			}break;

		case WM_NOTIFY:
			{
				{
					long lResult = pSelf->HandleNotify(hWnd, (int)wParam, (LPNMHDR)lParam); // handle customdraw
					if (lResult > 0)
					{
						SetWindowLong(hWnd, DWL_MSGRESULT, lResult);
						return lResult;
					}
				}

				switch (((LPNMHDR)lParam)->code)
				{
					// process fill list
				case LVN_GETDISPINFO:
					{
						if (((LPNMHDR)lParam)->hwndFrom == pSelf->GetHWNDListProperty()){
							pSelf->GETDISPINFO_FillList(lParam);
						}
						else if (((LPNMHDR)lParam)->hwndFrom == pSelf->GetHWNDCombobox()){
							pSelf->GETDISPINFO_FillComboList(lParam);
						}
					}break;

				case NM_CLICK:
					{
						if (((LPNMHDR)lParam)->hwndFrom == pSelf->GetHWNDTab()){
							pSelf->ProcessTabCtrl(hWnd, message, wParam, lParam);
						}
						else if (((LPNMHDR)lParam)->hwndFrom == pSelf->GetHWNDListProperty()){
							pSelf->ClickListProperties((LPNMLISTVIEW)lParam);
						}
					}break;

				default:
					break;
				};
			}break;

		case WM_USER_DELETE_ALL:
			{
				pSelf->ClearTabPages();
				pSelf->ClearListProperties();
				pSelf->HideChildControls();
			}break;

		case WM_USER_FILL_PROPS:
			{
				pSelf->FillPropertyTabs();
				pSelf->FillListProperties();
			}break;

		case WM_USER_UPDATE_PROPS:
			{
				pSelf->UpdatePreview();
			}break;

		default:
			break;
		};
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
bool Win32PropertyGrid::FillPropertyTabs()
{
	bool bResult = false;

	PropertyCS.enter();

	if (CActor* pSelected = GetSelected())
	{
		ClearTabPages();
		FillPropertyData(pSelected);
		bResult = true;
	}

	PropertyCS.leave();

	return bResult;
}

//----------------------------------------------------------------------------------------------
BOOL Win32PropertyGrid::FillListProperties()
{
	BOOL bResult = FALSE;
	
	PropertyCS.enter();

	TCHAR	szText[256];
	wchar_t wGroupBuff[256] = {0};
	wchar_t wPropBuff[256] = {0};
	wchar_t wPropValBuff[256] = {0};

	int SelectedGroup = TabCtrl_GetCurSel(m_hwndTab);

	TCITEM tc;
	tc.mask = TCIF_TEXT;
	tc.pszText = szText;
	tc.cchTextMax = 255;

	if (SelectedGroup != INDEX_NONE && TabCtrl_GetItem(m_hwndTab, SelectedGroup, &tc))
	{
		std::string StrGroupName = ConvertWideStringToString(szText);

		// find or alloc group
		PropertyGroup *pGroup = GetGroupByName(StrGroupName);

		if (pGroup)
		{
			// selected object property
			int PropertyListIndex = 0;

			if (GridViewStyle == EGV_Categorized)
			{
				for (TVecPropertyClassIter IterClass = pGroup->VecPropertyClasses.begin(); IterClass != pGroup->VecPropertyClasses.end(); ++IterClass)
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
						lvI.iItem = PropertyListIndex++;
						lvI.iIndent = 0;

						ListView_InsertItem(m_hwndListProperty, &lvI);

						// fill array 
						if ((*IterProp) && (*IterProp)->GetCtrl() == CTRL_ARRAY && IsDisclosed((*IterProp)->GetName()))
						{
							int Dimension = (*IterProp)->ElementDim;
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

									ListView_InsertItem(m_hwndListProperty, &lvI);

									ChildProp = ChildProp->GetNext();
								}
								--Dimension;
							}
						}

						if (!bAllowClass && !*IterProp){
							break;
						}
					}
				}
			}
			else if (GridViewStyle == EGV_SortByName)
			{
				// selected object property
				int PropertyListIndex = 0;
				for (TVecPropertyWrapperIter Iter = VSortedProperties.begin(); Iter != VSortedProperties.end(); ++Iter)
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

						ListView_InsertItem(m_hwndListProperty, &lvI);

						// fill array 
						if ((*Iter) && (*Iter)->pProp->GetCtrl() == CTRL_ARRAY && IsDisclosed((*Iter)->pProp->GetName()))
						{
							int Dimension = (*Iter)->pProp->ElementDim;
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

									ListView_InsertItem(m_hwndListProperty, &lvI);

									ChildProp = ChildProp->GetNext();
								}
								--Dimension;
							}
						}
					}
				}
			}
		}
	}

	PropertyCS.leave();

	return TRUE;
}

//----------------------------------------------------------------------------------------------
int	Win32PropertyGrid::FillTransientPropertyList(const CActor* InActor, int &InPropertyListIndex)
{
	return 0;
}

//----------------------------------------------------------------------------------------------
void Win32PropertyGrid::ClearListProperties()
{
	ListView_RemoveAllGroups(m_hwndListProperty);
	ListView_DeleteAllItems(m_hwndListProperty);

	for (TVecPropertyGroupConstIter Iter = PropertyGroups.begin(); Iter != PropertyGroups.end(); ++Iter)
	{
		for (TVecPropertyClassIter IterClass = (*Iter)->VecPropertyClasses.begin(); IterClass != (*Iter)->VecPropertyClasses.end(); ++IterClass)
		{
			delete *IterClass;
		}
 		delete *Iter;
 	}
	PropertyGroups.clear();

	for_each(VSortedProperties.begin(), VSortedProperties.end(), DeleteVectorFntor());
	VSortedProperties.clear();
}

//----------------------------------------------------------------------------------------------
BOOL Win32PropertyGrid::FillComboProperties(LPNMLISTVIEW plvdi)
{
	return TRUE;
}

//----------------------------------------------------------------------------------------------
bool IsPropertyBOOL(const Property_Base * Prop)
{
	if (Prop->PredefinedValues.size() >= 2 &&
		Prop->PredefinedValues[0] == "true" &&
		Prop->PredefinedValues[1] == "false")
	{
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------------
wchar_t wbuf[256] = {0};
BOOL Win32PropertyGrid::GETDISPINFO_FillList(LPARAM lParam)
{
	PropertyCS.enter();
	
	LPNMLVDISPINFOA plvdi = (LPNMLVDISPINFOA)lParam;
	TCHAR	szText[256];

	TCITEM tc;
	tc.mask = TCIF_TEXT;
	tc.pszText = szText;
	tc.cchTextMax = 255;

	int SelectedGroup = TabCtrl_GetCurSel(m_hwndTab);

	SPropertyClass * OutClass = 0;
	Property_Base * OutProperty = 0;
	int OutMemoryOffset = 0;

	if (GetPropertyByIndex((int)plvdi->item.iItem, SelectedGroup, &OutClass, &OutProperty, OutMemoryOffset))
	{
		if (OutProperty) // ordinary property
		{
			FillListParam(lParam, OutClass, OutProperty, OutMemoryOffset);
		}
		else if (OutClass) // class name selector
		{
			switch (((LPNMLVDISPINFOA)lParam)->item.iSubItem)
			{
			case 0:
				{
					MultiByteToWideChar(CP_ACP, 0, OutClass->ClassName.c_str(), -1, wbuf, 255);
					plvdi->item.pszText = (LPSTR)wbuf;

					bool bCollapsed = IsClassAllowed(OutClass->ClassName);
					plvdi->item.iImage = !bCollapsed ? 2 : 3;
				}break;
			default:
				break;
			};
		}
	}

	PropertyCS.leave();

	return FALSE;
}

//----------------------------------------------------------------------------------------------
BOOL Win32PropertyGrid::FillListParam(LPARAM lParam, const SPropertyClass* PropClass, const Property_Base* Prop, int MemOffset)
{
	LPNMLVDISPINFOA plvdi = (LPNMLVDISPINFOA)lParam;
	switch (((LPNMLVDISPINFOA)lParam)->item.iSubItem)
	{
	case 0:
		{
			MultiByteToWideChar(CP_ACP, 0, Prop->GetName(), -1, wbuf, 255);
			plvdi->item.pszText = (LPSTR)wbuf;
		}break;

	case 1:
		{
			char Buff[1024] = {0};

			int MemoryOffsetOverride = 0;
			if (PropClass->nOverrideByteShift != -1){ // interface relative shift
				MemoryOffsetOverride = PropClass->nOverrideByteShift;
			}

			Prop->GetProperty((BYTE*)PropClass->pDataObject + MemOffset + MemoryOffsetOverride, Buff);

			if (IsPropertyBOOL(Prop) && (plvdi->item.mask & LVIF_IMAGE))
			{
				plvdi->item.iImage = strcmp(Buff, "true") ? 1 : 0;
			}
			else if (plvdi->item.mask & LVIF_TEXT)
			{
				MultiByteToWideChar(CP_ACP, 0, Buff, -1, wbuf, 1024);
				plvdi->item.pszText = (LPSTR)wbuf;
			}
		}break;
	default:
		break;
	};
	return TRUE;
}

//----------------------------------------------------------------------------------------------
wchar_t wbuf2[256] = {0};
BOOL Win32PropertyGrid::GETDISPINFO_FillComboList(LPARAM lParam)
{
	BOOL bResult = FALSE;

	PropertyCS.enter();

	LPNMLVDISPINFOA plvdi = (LPNMLVDISPINFOA)lParam;
	TCHAR	szText[256];

	TCITEM tc;
	tc.mask = TCIF_TEXT;
	tc.pszText = szText;
	tc.cchTextMax = 255;

	int SelectedGroup = TabCtrl_GetCurSel(m_hwndTab);

	SPropertyClass * OutClass = 0;
	Property_Base * OutProperty = 0;
	int OutMemoryOffset = 0;

	if (GetPropertyByIndex(m_EditingPropertyIndex, SelectedGroup, &OutClass, &OutProperty, OutMemoryOffset))
	{
		if (OutProperty) // ordinary property
		{
			if (OutProperty->GetCtrl() == CTRL_ARRAY)
			{

			}
			else
			{
				size_t NumItems = OutProperty->PredefinedValues.size();

				if ((int)plvdi->item.iItem >= 0 && (int)plvdi->item.iItem < (int)NumItems)
				{
					if (plvdi->item.pszText && plvdi->item.iSubItem == 0)
					{
						MultiByteToWideChar(CP_ACP, 0, OutProperty->PredefinedValues[(int)plvdi->item.iItem].c_str(), -1, wbuf2, 255);
						plvdi->item.pszText = (LPSTR)wbuf2;
					}
				}
			}
		}

		bResult = TRUE;
	}

	PropertyCS.leave();

	return bResult;
}

//----------------------------------------------------------------------------------------------
long Win32PropertyGrid::HandleNotify(HWND hWndDlg, int nIDCtrl, LPNMHDR pNMHDR)
{
 	if (pNMHDR->hwndFrom == m_hwndListProperty && pNMHDR)
 	{
 		if (pNMHDR->code == NM_CUSTOMDRAW)
 		{
 			LPNMLVCUSTOMDRAW pNMTVCD = (LPNMLVCUSTOMDRAW) pNMHDR;
 			return HandleCustomDraw(m_hwndListProperty, pNMTVCD);
 		}
 	}
	return 0;
}

//----------------------------------------------------------------------------------------------
long Win32PropertyGrid::HandleCustomDraw(HWND hWndTreeView, LPNMLVCUSTOMDRAW pNMLVCD)
{
	if (pNMLVCD == NULL){
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
			DWORD CDResult = CDRF_NOTIFYSUBITEMDRAW;
			return CDResult;
		}break;

	case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
		{
			PropertyCS.enter();

			TCHAR	szText[256];

			TCITEM tc;
			tc.mask = TCIF_TEXT;
			tc.pszText = szText;
			tc.cchTextMax = 255;

			int SelectedGroup = TabCtrl_GetCurSel(m_hwndTab);

			SPropertyClass * OutClass = 0;
			Property_Base * OutProperty = 0;
			int OutMemoryOffset = 0;

			if (GetPropertyByIndex((int)pNMLVCD->nmcd.dwItemSpec, SelectedGroup, &OutClass, &OutProperty, OutMemoryOffset))
			{
				CustomDrawProperty(pNMLVCD, OutClass, OutProperty, OutMemoryOffset);
			}

			PropertyCS.leave();

			return (CDRF_NEWFONT);// | CDRF_NOTIFYPOSTPAINT);
		}break;

	case CDDS_ITEMPOSTPAINT:
		{
			return CDRF_DODEFAULT;
		}
		break;

	default:
		{
		}break;
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
void Win32PropertyGrid::CustomDrawProperty(LPNMLVCUSTOMDRAW pNMLVCD, const SPropertyClass* PropClass, const Property_Base * Prop, int MemoryOffset)
{
	if (Prop == NULL || Prop->GetCtrl() == CTRL_ARRAY)
	{
		pNMLVCD->clrText	= 0x00000011; // 0x00bbggrr
		pNMLVCD->clrTextBk	= RGB(220, 220, 220);
		pNMLVCD->nmcd.uItemState &= ~CDIS_SELECTED; // kill sys sel
	}
	else
	{
		int Policy = Prop->GetPolicy();

		if (Policy == READ_ONLY || Policy == NO_READ_WRITE || Prop->GetCtrl() == CTRL_ARRAY)
		{
			pNMLVCD->nmcd.uItemState &= ~CDIS_SELECTED; // kill sys sel
			pNMLVCD->clrText	= RGB(128, 128, 128);
		}

		if (pNMLVCD->iSubItem == 1)
		{
			switch (Prop->GetCtrl()) // switch property type
			{
			case CTRL_COLOR:
				{
					pNMLVCD->nmcd.uItemState &= ~CDIS_SELECTED; // kill sys sel

					char ascii[256] = {0};

					int MemoryOffsetOverride = 0;
					if (PropClass->nOverrideByteShift != -1){ // interface relative shift
						MemoryOffsetOverride = PropClass->nOverrideByteShift;
					}

					Prop->GetProperty((BYTE*)PropClass->pDataObject + MemoryOffset + MemoryOffsetOverride, ascii);

					DWORD Buffer = boost::lexical_cast<DWORD>(ascii);
					pNMLVCD->clrText	= RGB(GetBValue(Buffer), GetGValue(Buffer), GetRValue(Buffer)); // 0x00rrggbb -> 0x00bbggrr
					pNMLVCD->clrTextBk	= RGB(GetBValue(Buffer), GetGValue(Buffer), GetRValue(Buffer)); // 0x00rrggbb -> 0x00bbggrr
				}break;

			case CTRL_COMBO:
				{
					pNMLVCD->nmcd.uItemState &= ~CDIS_SELECTED; // kill sys sel
				}break;
			}
		}
	}
}
//----------------------------------------------------------------------------------------------
long Win32PropertyGrid::HandleNotifyCombobox(HWND hWndDlg, int nIDCtrl, LPNMHDR pNMHDR)
{
 	if (pNMHDR->hwndFrom == m_hwndEditComboList && pNMHDR)
 	{
 		if (pNMHDR->code == NM_CUSTOMDRAW)
 		{
 			LPNMLVCUSTOMDRAW pNMTVCD = (LPNMLVCUSTOMDRAW) pNMHDR;
 			return HandleCustomDrawCombobox(m_hwndEditComboList, pNMTVCD);
 		}
 	}
	return 0;
}

//----------------------------------------------------------------------------------------------
long Win32PropertyGrid::HandleCustomDrawCombobox(HWND hWndTreeView, LPNMLVCUSTOMDRAW pNMLVCD)
{
	if (pNMLVCD == NULL){
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
			DWORD CDResult = CDRF_NOTIFYSUBITEMDRAW;
			return CDResult;
		}break;

	case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
		{
			if (pNMLVCD->iSubItem == 0)
			{
				pNMLVCD->clrTextBk	= RGB(220, 220, 220);
				pNMLVCD->nmcd.uItemState &= ~CDIS_SELECTED; // kill sys sel
			}
			return (CDRF_NEWFONT);// | CDRF_NOTIFYPOSTPAINT);
		}break;

	case CDDS_ITEMPOSTPAINT:
		{
			return CDRF_DODEFAULT;
		}
		break;

	default:
		{
		}break;
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
BOOL Win32PropertyGrid::ClickListProperties(LPNMLISTVIEW plvdi)
{
	PropertyCS.enter();

	TCHAR	szText[256];

	TCITEM tc;
	tc.mask = TCIF_TEXT;
	tc.pszText = szText;
	tc.cchTextMax = 255;

	int SelectedGroup = TabCtrl_GetCurSel(m_hwndTab);

	SPropertyClass * OutClass = 0;
	Property_Base * OutProperty = 0;
	int OutMemoryOffset = 0;

	if (GetPropertyByIndex((int)plvdi->iItem, SelectedGroup, &OutClass, &OutProperty, OutMemoryOffset))
	{
		if (OutProperty) // property
		{
			ProcessClickListProperties(plvdi, OutClass, OutProperty, OutMemoryOffset);
		}
		else if (OutClass) // Class filter
		{
			std::vector<std::string>::iterator IterCollapsedClass = std::find(VCollapsedClassNames.begin(), VCollapsedClassNames.end(), OutClass->ClassName);

			if (IterCollapsedClass != VCollapsedClassNames.end()){
				VCollapsedClassNames.erase(IterCollapsedClass);
			}else{
				VCollapsedClassNames.push_back(OutClass->ClassName);
			}

			m_EditingPropertyIndex = INDEX_NONE;
			m_MemoryOffset = 0;

			ListView_DeleteAllItems(m_hwndListProperty);

			HideChildControls();
			FillListProperties();
		}
	}

	PropertyCS.leave();

	return TRUE;
}

//----------------------------------------------------------------------------------------------
BOOL Win32PropertyGrid::ProcessClickListProperties(LPNMLISTVIEW plvdi, SPropertyClass *Class, Property_Base* Prop, int MemOffset)
{
	POINT Point;

	if (ListView_GetItemPosition(m_hwndListProperty, plvdi->iItem, &Point))
	{
		if (m_EditingPropertyIndex != (int)plvdi->iItem)
		{
			m_EditingPropertyIndex = (int)plvdi->iItem; // save index
			m_MemoryOffset = MemOffset;
			HideChildControls();
		}

		wchar_t wbuf[256] = {0}; // temp buffers
		char ascii[256] = {0};

		int MemoryOffsetOverride = 0;
		if (Class->nOverrideByteShift != -1){ // interface relative shift
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
			unsigned int Width01 = ListView_GetColumnWidth(m_hwndListProperty, 0);
			unsigned int Width02 = ListView_GetColumnWidth(m_hwndListProperty, 1);

			MoveWindow(m_hwndEditEditbox, 
				Point.x + Width01,
				Point.y,
				Width02, 16, TRUE);

			ShowWindow(m_hwndEditEditbox, SW_SHOW);
			SendMessage(m_hwndEditEditbox, WM_SETTEXT, 0, (LPARAM)wbuf);
			SendMessage(m_hwndEditEditbox, EM_SETREADONLY, (WPARAM) (BOOL)Prop->GetPolicy() == READ_ONLY, 0);
			SetFocus(m_hwndEditEditbox);
		}break;

		case CTRL_EDIT_RESOURCE:
			{
				INT StartX, StartY;
				INT EditboxWidth, EditboxHeight;
				StartX = Point.x  + ListView_GetColumnWidth(m_hwndListProperty, 0) + 4;
				StartY = Point.y;
				EditboxWidth  = ListView_GetColumnWidth(m_hwndListProperty, 1);
				EditboxHeight = 16;

				MoveWindow(m_hwndEditEditbox, StartX, StartY, EditboxWidth - (EditboxHeight * 2), EditboxHeight, TRUE);
				MoveWindow(m_hwndEditEditboxBtnBrowse, StartX + (EditboxWidth - 16), StartY, EditboxHeight, EditboxHeight, TRUE);
				MoveWindow(m_hwndEditEditboxBtnClear, StartX + (EditboxWidth - 16 * 2), StartY, EditboxHeight, EditboxHeight, TRUE);

				SendMessage(m_hwndEditEditbox, WM_SETTEXT, 0, (LPARAM)wbuf);

				ShowWindow(m_hwndEditEditbox, SW_SHOW);
				ShowWindow(m_hwndEditEditboxBtnBrowse, SW_SHOW);
				ShowWindow(m_hwndEditEditboxBtnClear, SW_SHOW);
				SetFocus(m_hwndEditEditbox);
			}break;

		case CTRL_COMBO:
			{
				if ((int)plvdi->iSubItem == 1)
				{
					if (IsPropertyBOOL(Prop))
					{
						char Buff[255];

						int MemoryOffsetOverride = 0;
						if (Class->nOverrideByteShift != -1){ // interface relative shift
							MemoryOffsetOverride = Class->nOverrideByteShift;
						}

						bool bValue = (strcmp(Prop->GetProperty((BYTE*)Class->pDataObject + MemOffset + MemoryOffsetOverride, Buff), "true") == 0) ? true : false;
						Prop->SetProperty((BYTE*)Class->pDataObject + MemOffset, bValue ? "false" : "true"); // set new value

						::PostMessage(GetHWNDChildBottom(), WM_USER_UPDATE_PROPS, 0, 0);
					}
					else
					{
						if (IsWindowVisible(m_hwndEditComboBtn))
						{
							if (IsWindowVisible(m_hwndEditComboList))
							{
								ShowWindow(m_hwndEditComboList, SW_HIDE);
								SetFocus(m_hwndEditComboBtn);
							}
							else
							{
								ListView_SetColumnWidth(m_hwndEditComboList, 1, 0);
								ListView_DeleteAllItems(m_hwndEditComboList);

								size_t NumItems = Prop->PredefinedValues.size();

								for (size_t IndexValue = 0; IndexValue < NumItems; ++IndexValue)
								{
									char Buff[255];

									int MemoryOffsetOverride = 0;
									if (Class->nOverrideByteShift != -1){ // interface relative shift
										MemoryOffsetOverride = Class->nOverrideByteShift;
									}

									Prop->GetProperty((BYTE*)Class->pDataObject + MemOffset + MemoryOffsetOverride, Buff);
									
									LVITEM lvI;
									memset(&lvI, 0, sizeof(LVITEM));
									lvI.mask = LVIF_TEXT | LVIF_STATE |  LVIF_PARAM;
									int Cmp = strcmp(Buff, Prop->PredefinedValues[IndexValue].c_str());
									lvI.state = !Cmp ? LVIS_SELECTED : 0;
									lvI.stateMask = !Cmp ? LVIS_SELECTED | LVIS_DROPHILITED : 0;
									lvI.iSubItem = 0;
									lvI.pszText = LPSTR_TEXTCALLBACK;
									lvI.iItem = IndexValue;
									lvI.lParam = INDEX_NONE;

									SendMessage(m_hwndEditComboList, LVM_INSERTITEM, 0, (LPARAM)&lvI);
								}

								MoveWindow(m_hwndEditComboList, 
									Point.x + ListView_GetColumnWidth(m_hwndListProperty, 0),
									Point.y + 17,
									ListView_GetColumnWidth(m_hwndListProperty, 1),
									(NumItems > 4 ? 4 : NumItems) * 17 + 4,
									TRUE);

								ShowWindow(m_hwndEditComboList, SW_SHOW);
								SetFocus(m_hwndEditComboList);
							}
						}
						else
						{
							MoveWindow(m_hwndEditComboBtn, 
								Point.x + ListView_GetColumnWidth(m_hwndListProperty, 0) + ListView_GetColumnWidth(m_hwndListProperty, 1) - 16,
								Point.y,
								16,
								16,
								TRUE);

							ShowWindow(m_hwndEditComboBtn, SW_SHOW);
							SetFocus(m_hwndEditComboBtn);
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
				cc.hwndOwner = m_hwndParent;
				cc.lpCustColors = (LPDWORD) acrCustClr;
				cc.Flags = CC_FULLOPEN | CC_RGBINIT;

				Buffer = boost::lexical_cast<DWORD>(ascii);
				cc.rgbResult = RGB(GetBValue(Buffer), GetGValue(Buffer), GetRValue(Buffer)); // 0x00rrggbb -> 0x00bbggrr

				if (ChooseColor(&cc) == TRUE)
				{
					Buffer = (255 << 24) | RGB(GetBValue(cc.rgbResult), GetGValue(cc.rgbResult), GetRValue(cc.rgbResult)); // 0x00bbggrr to 0x00rrggbb

					char txt[256] = {'\0'};
					sprintf(txt, "%d", Buffer);

					Prop->SetProperty((BYTE*)Class->pDataObject + MemOffset, txt);

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
BOOL Win32PropertyGrid::ClickComboList(LPNMLISTVIEW plvdi)
{
	PropertyCS.enter();

	TCHAR	szText[256];

	TCITEM tc;
	tc.mask = TCIF_TEXT;
	tc.pszText = szText;
	tc.cchTextMax = 255;

	int SelectedGroup = TabCtrl_GetCurSel(m_hwndTab);

	SPropertyClass * OutClass = 0;
	Property_Base * OutProperty = 0;
	int OutMemoryOffset = 0;

	if (GetPropertyByIndex(m_EditingPropertyIndex, SelectedGroup, &OutClass, &OutProperty, OutMemoryOffset))
	{
		if (OutProperty) // property
		{
			size_t NumItems = OutProperty->PredefinedValues.size();

			if (plvdi->iSubItem == 0 &&	(int)plvdi->iItem >= 0 && (int)plvdi->iItem < (int)NumItems)
			{
				int MemoryOffsetOverride = 0;
				if (OutClass->nOverrideByteShift != -1){ // interface relative shift
					MemoryOffsetOverride = OutClass->nOverrideByteShift;
				}

				OutProperty->SetProperty((BYTE*)OutClass->pDataObject + m_MemoryOffset, OutProperty->PredefinedValues[(int)plvdi->iItem].c_str(), MemoryOffsetOverride); // set new value
			}
			HideChildControls();
		}
		else if (OutClass) // Class filter
		{
		}
	}

	PropertyCS.leave();
	return TRUE;
}

//----------------------------------------------------------------------------------------------
void Win32PropertyGrid::HideChildControls()
{
	ShowWindow(m_hwndEditEditbox, SW_HIDE);
	ShowWindow(m_hwndEditComboList, SW_HIDE);
	ShowWindow(m_hwndEditComboBtn, SW_HIDE);
	ShowWindow(m_hwndEditEditboxBtnBrowse, SW_HIDE);
	ShowWindow(m_hwndEditEditboxBtnClear, SW_HIDE);
}

//----------------------------------------------------------------------------------------------
void Win32PropertyGrid::Size(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	int Width = rect.right - rect.left;
	int Height = rect.bottom - rect.top;

	MoveWindow(m_hwndTab, rect.left, rect.top, Width, TAB_HEIGHT, TRUE);
	MoveWindow(m_hwndListProperty, rect.left, TAB_HEIGHT, Width, Height - TAB_HEIGHT - LIST_BOTTOM, TRUE);
}

//----------------------------------------------------------------------------------------------
Win32PropertyGrid::PropertyGroup* Win32PropertyGrid::GetGroupByName(const std::string &Name) const
{
	// find or alloc group
	PropertyGroup * pOutGroup = NULL;

	TVecPropertyGroupConstIter IterGroup = PropertyGroups.begin();
	while (IterGroup != PropertyGroups.end())
	{
		if ((*IterGroup)->GroupName == Name){
			pOutGroup = *IterGroup;
			break;
		}
		++IterGroup;
	}
	return pOutGroup;
}

//----------------------------------------------------------------------------------------------
bool Win32PropertyGrid::IsClassAllowed(const std::string& Name) const
{
	// selection query
	bool bAllowClass = true;
	std::vector<std::string>::const_iterator IterCollapsedClass = std::find(VCollapsedClassNames.begin(), VCollapsedClassNames.end(), Name);
	if (IterCollapsedClass != VCollapsedClassNames.end()){
		bAllowClass = false;
	}
	return bAllowClass;
}

//----------------------------------------------------------------------------------------------
bool Win32PropertyGrid::IsDisclosed(const std::string &Name) const
{
	return true;
}

//----------------------------------------------------------------------------------------------
bool Win32PropertyGrid::GetPropertyByIndex( int InPlainIndex,
											int IndexGroup,
											struct SPropertyClass **OutClass, 
											class Property_Base **OutProperty,
											int &OutMemoryOffset) const
{
	bool bFind = false;
	const unsigned int sSize = 256;
	TCHAR	szText[sSize];

	TCITEM tc;
	tc.mask = TCIF_TEXT;
	tc.pszText = szText;
	tc.cchTextMax = sSize - 1;

	if (IndexGroup != INDEX_NONE && TabCtrl_GetItem(m_hwndTab, IndexGroup, &tc))
	{
		int Counter = 0;

		*OutClass = 0;
		*OutProperty = 0;
		OutMemoryOffset = 0;

		std::string StrGroupName(ConvertWideStringToString(szText));

		// find or alloc group
		PropertyGroup *pGroup = GetGroupByName(StrGroupName);
		assert(pGroup);

		if (GridViewStyle == EGV_Categorized)
		{
			for (TVecPropertyClassConstIter IterClass = pGroup->VecPropertyClasses.begin();
				!bFind && IterClass != pGroup->VecPropertyClasses.end(); ++IterClass)
			{
				// selection query
				bool bAllowClass = IsClassAllowed((*IterClass)->ClassName);

				for (TVecPropertyBaseConstIter IterProp = (*IterClass)->InheritProperties.begin(); !bFind && IterProp != (*IterClass)->InheritProperties.end(); ++IterProp)
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
						int ElementDim = (*IterProp)->ElementDim;
						int ElementSize = (*IterProp)->ElementSize;
						int MemOffset = (*IterProp)->m_MemOffset;

						while (ElementDim && !bFind)
						{
							// go through child nodes
							Property_Base * ChildProp = (*IterProp)->GetNext();

							while (ChildProp)
							{
								if (Counter++ == InPlainIndex)
								{
									*OutClass = (*IterClass);
									*OutProperty = ChildProp;
									OutMemoryOffset = (*IterProp)->m_MemOffset + ElementSize * ((*IterProp)->ElementDim - ElementDim);
									bFind = true;
									break;
								}
								MemOffset += ChildProp->m_MemOffset;
								ChildProp = ChildProp->GetNext();
							}

							--ElementDim;
							MemOffset = (*IterProp)->m_MemOffset + ElementSize * ((*IterProp)->ElementDim - ElementDim);
						}
					}
					
					if (!bAllowClass){
						break;
					}
				}
			}
		}
		else if (GridViewStyle == EGV_SortByName)
		{
			for (TVecPropertyWrapperConstIter Iter = VSortedProperties.begin(); !bFind && Iter != VSortedProperties.end(); ++Iter)
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
						int ElementDim = (*Iter)->pProp->ElementDim;
						int ElementSize = (*Iter)->pProp->ElementSize;
						int MemOffset = (*Iter)->pProp->m_MemOffset;

						while (ElementDim)
						{
							// go through child nodes
							Property_Base * ChildProp = (*Iter)->pProp->GetNext();

							while (ChildProp)
							{
								if (Counter++ == InPlainIndex)
								{
									*OutClass = (*Iter)->pClass;
									*OutProperty = ChildProp;
									OutMemoryOffset = (*Iter)->pProp->m_MemOffset + ElementSize * ((*Iter)->pProp->ElementDim - ElementDim);
									bFind = true;
									break;
								}
								MemOffset += ChildProp->m_MemOffset;
								ChildProp = ChildProp->GetNext();
							}

							--ElementDim;
							MemOffset = (*Iter)->pProp->m_MemOffset + ElementSize * ((*Iter)->pProp->ElementDim - ElementDim);
						}
					}
				}
			}
		}
	}
	return bFind;
}*/