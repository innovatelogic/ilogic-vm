#ifndef __win32itemstree_h__
#define __win32itemstree_h__

template<class T_CLASS>
class CWin32ItemsTree : public IWindowLayout
{
	typedef typename void (*pContextMenuFunction) (HWND hwnd, POINT pt, T_CLASS *pActor);
	typedef typename bool (*pContextMenuProcessor)(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	typedef typename int  (*pGetResourceIconIndex)(const std::string &TypeName);

	typedef typename std::map<const T_CLASS*, HTREEITEM>	TTreeMapItem;
	typedef typename TTreeMapItem::iterator					TTreeMapItemIterator;
	typedef typename TTreeMapItem::const_iterator			TTreeMapItemConstIterator;

	typedef typename std::vector<T_CLASS*>					TVecItems;

	struct SAddPair
	{
		SAddPair(const T_CLASS *child, const T_CLASS *parent)
			: pChild(child)
			, pParent(parent)
		{}

		const T_CLASS *pChild;
		const T_CLASS *pParent;
	};

	typedef typename std::vector<SAddPair>					TVecAddPairItems;

public:
	HWND GetHWndTree() const { return m_hwndTree; }

	CWin32ItemsTree(IWindowLayout *pParent,
		HWND hWndParent,
		const CWindowManager *pWMgr,
		pContextMenuFunction pfMenu,
		pContextMenuProcessor pfnMenuProcessor,
		pGetResourceIconIndex pfnGetResourceIconIndex,
		HIMAGELIST hImageList)
		: m_pWindowManager(pWMgr)
		, m_hwndParent(hWndParent)
		, m_pfnContextMenu(pfMenu)
		, m_pfnContextMenuProcessor(pfnMenuProcessor)
		, m_pfnGetResourceIconIndex(pfnGetResourceIconIndex)
		: IWindowLayout(pParent)
	{
		RECT rect;
		GetClientRect(hWndParent, &rect);

		int Width = rect.right - rect.left;
		int Height = rect.bottom - rect.top;

		m_hwndTree = CreateWindow(
			WC_TREEVIEW,
			NULL,
			WS_CHILD | WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_SHOWSELALWAYS | WS_VISIBLE,
			rect.left,
			rect.top,
			Width,
			Height,
			hWndParent,
			NULL,
			(HINSTANCE)GetWindowLong(hWndParent, GWL_HINSTANCE),
			NULL);

		SetWindowLong(m_hwndTree, GWL_USERDATA, (LONG)this);

		TreeView_SetImageList(m_hwndTree, hImageList, TVSIL_NORMAL);

		m_lpfnTreeProc = (WNDPROC) SetWindowLong(m_hwndTree, GWL_WNDPROC, (LONG)SubClassProcTree);

		m_hCursHand = LoadCursor(NULL, IDC_HAND);
		m_hCursArrow = LoadCursor(NULL, IDC_ARROW);
	}

	//----------------------------------------------------------------------------------------------
	static BOOL SubClassProcTree(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		CWin32ItemsTree *pBrowser = reinterpret_cast<CWin32ItemsTree*>(GetWindowLong(hWnd, GWL_USERDATA));

		switch (messg) 
		{
		case WM_CREATE:
			{
			}break;

		case WM_COMMAND:
			{
				if (pBrowser->m_pfnContextMenuProcessor(hWnd, messg, wParam, lParam)){
					break;
				}
			}break;

		case WM_MOUSEWHEEL:
		case WM_VSCROLL:
			{
			}break;

		case WM_MOUSEMOVE:
			{
			}break;

		case WM_LBUTTONUP:
			{
			}break;

		case WM_NOTIFY:
			{
				if (((LPNMHDR)lParam)->code == TVN_KEYDOWN)
				{
					LPNMTVKEYDOWN ptvkd = (LPNMTVKEYDOWN) lParam;
					switch ((DWORD)ptvkd->wVKey) 
					{
						// move Up/Down
						//case 'U': {} break;
						//case 'J': {} break;

					case VK_DELETE:{
								   }break;
					case VK_F2:{
							   }break;
					}break;
				}
			}break;
		}

		// Call the original window procedure for default processing. 
		return CallWindowProc(pBrowser->m_lpfnTreeProc, hWnd, messg, wParam, lParam);
	}

	//----------------------------------------------------------------------------------------------
	int AddItem(const T_CLASS *pSender, const T_CLASS *pParent)
	{
		int bResult = 0;

		m_CS.enter();

		/*if ((std::find(m_VecAddItems.begin(), m_VecAddItems.end(), pSender) == m_VecAddItems.end() && m_TreeMap.find(pSender) == m_TreeMap.end()) && // not to add already
			((!pParent || m_TreeMap.find(pParent) != m_TreeMap.end()) || // !pParent - root or parent exists
			 std::find(m_VecAddParentItems.begin(), m_VecAddParentItems.end(), pParent) != m_VecAddParentItems.end())) // parent already pushed to add
		{
			m_VecAddParentItems.push_back(const_cast<T_CLASS*>(pParent));
			m_VecAddItems.push_back(const_cast<T_CLASS*>(pSender));
		}*/

		m_VecAddPairItems.push_back(SAddPair(pSender, pParent));

		m_CS.leave();

		::PostMessage(m_hwndParent, WM_USER_INSERTOBJECT, 0, 0);

		bResult = 1;
		
		return bResult;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT WndProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT bResult = 0;

		switch (message) 
		{
		case WM_USER_INSERTOBJECT:
			{
				WndProcessInsertObject();
				bResult = 1;
			}break;

		default:
			break;
		};
		return bResult;
	}

	//----------------------------------------------------------------------------------------------
	void WndProcessInsertObject()
	{
		m_CS.enter();

		::LockWindowUpdate(m_hwndTree);

/*		int i = 0;
		for (TVecItems::iterator Iter = m_VecAddItems.begin(); Iter != m_VecAddItems.end(); ++Iter)
		{
			//int IndexBitmap = m_pfnGetResourceIconIndex((*Iter)->GetType());

			// add data to tree view
			T_CLASS *pParent = m_VecAddParentItems[i];
			TTreeMapItemConstIterator IterFind = pParent ? m_TreeMap.find(pParent) : m_TreeMap.end();

			TV_INSERTSTRUCT tvinsert;													 // struct to config out tree control
			tvinsert.hParent = (IterFind == m_TreeMap.end()) ? NULL : IterFind->second;	 // top most level no need handle
			tvinsert.hInsertAfter = (IterFind == m_TreeMap.end()) ? TVI_ROOT : TVI_LAST;   // work as root level

			std::wstring Name = (*Iter)->sName;

			tvinsert.item.mask = TVIF_TEXT;

			tvinsert.item.pszText = (LPWSTR)Name.c_str();

			HTREEITEM hItem = TreeView_InsertItem(m_hwndTree, &tvinsert);

			m_TreeMap.insert(std::make_pair(*Iter, hItem));	// cache current

			// update preview
			TreeView_SelectDropTarget(m_hwndTree, hItem);
			TreeView_SelectDropTarget(m_hwndTree, NULL);

			++i;
		}

		m_VecAddItems.clear();
		m_VecAddParentItems.clear();*/


		for (TVecAddPairItems::iterator Iter = m_VecAddPairItems.begin(); Iter != m_VecAddPairItems.end(); ++Iter)
		{
			const T_CLASS *pParent = Iter->pParent;
			TTreeMapItemConstIterator IterFind = pParent ? m_TreeMap.find(pParent) : m_TreeMap.end();

			TV_INSERTSTRUCT tvinsert;													 // struct to config out tree control
			tvinsert.hParent = (IterFind == m_TreeMap.end()) ? NULL : IterFind->second;	 // top most level no need handle
			tvinsert.hInsertAfter = (IterFind == m_TreeMap.end()) ? TVI_ROOT : TVI_LAST;   // work as root level

			std::wstring sName = (*Iter).pChild->sName;

			tvinsert.item.mask = TVIF_TEXT;

			tvinsert.item.pszText = (LPWSTR)sName.c_str();

			HTREEITEM hItem = TreeView_InsertItem(m_hwndTree, &tvinsert);

			m_TreeMap.insert(std::make_pair((*Iter).pChild, hItem));	// cache current

			// update preview
			TreeView_SelectDropTarget(m_hwndTree, hItem);
			TreeView_SelectDropTarget(m_hwndTree, NULL);
		}

		m_VecAddPairItems.clear();

		::LockWindowUpdate(NULL);

		m_CS.leave();
	}

	virtual void Free() { }

	virtual HWND GetHWND() const { return m_hwndTree; }
protected:
private:

	TTreeMapItem			m_TreeMap;

	TVecAddPairItems		m_VecAddPairItems;
	TVecItems				m_VecAddItems;
	TVecItems				m_VecAddParentItems;

	HWND m_hwndParent;
	HWND m_hwndTree;

	const class CWindowManager *m_pWindowManager;

	CriticalSection m_CS;

	pContextMenuFunction	m_pfnContextMenu;
	pContextMenuProcessor	m_pfnContextMenuProcessor;
	pGetResourceIconIndex	m_pfnGetResourceIconIndex;

	WNDPROC					m_lpfnTreeProc;

	HCURSOR	m_hCursHand;
	HCURSOR m_hCursArrow;     // cursor handles
};


#endif//__win32itemstree_h__