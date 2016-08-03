#ifndef __wtlitemstree_h__
#define __wtlitemstree_h__

#pragma once
typedef CWinTraitsOR<  
	WS_BORDER  
	| TVS_HASBUTTONS  
	| TVS_HASLINES  
	| TVS_LINESATROOT  
	| TVS_TRACKSELECT  
	| TVS_FULLROWSELECT> CTreeCtrlTraits;

template<class T_CLASS>
class CWTLItemsTree : public CWindowImpl<CWTLItemsTree<T_CLASS>, CTreeViewCtrl, CTreeCtrlTraits>
{
	typedef typename std::map<const T_CLASS*, HTREEITEM>	TTreeMapItem;
	typedef typename TTreeMapItem::iterator					TTreeMapItemIterator;
	typedef typename TTreeMapItem::const_iterator			TTreeMapItemConstIterator;

	struct SAddPair
	{
		SAddPair(const T_CLASS *child, const T_CLASS *parent)
			: pChild(child)
			, pParent(parent)
		{}

		const T_CLASS *pChild;
		const T_CLASS *pParent;
	};

	typedef typename std::vector<SAddPair*>	TVecItems;
	typedef typename std::vector<SAddPair>	TVecAddPairItems;

public:
	BEGIN_MSG_MAP(CWTLItemsTree<T_CLASS>)
		MESSAGE_HANDLER(WM_USER_INSERTOBJECT, OnInsertObject)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	//----------------------------------------------------------------------------------------------
	CWTLItemsTree()
	{

	}

	//----------------------------------------------------------------------------------------------
	int AddItem(const T_CLASS *pSender, const T_CLASS *pParent)
	{
		int bResult = 0;

		m_CS.enter();

		m_VecAddPairItems.push_back(SAddPair(pSender, pParent));

		m_CS.leave();

		::PostMessage(m_hWnd, WM_USER_INSERTOBJECT, 0, 0);

		bResult = 1;
		
		return bResult;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnInsertObject(UINT, WPARAM, LPARAM, BOOL&bHendled)
	{
		m_CS.enter();

		::LockWindowUpdate(m_hWnd);

		for (TVecAddPairItems::iterator Iter = m_VecAddPairItems.begin(); Iter != m_VecAddPairItems.end(); ++Iter)
		{
			const int nMagicIndexBitmap = 53; // TODO: get from registry

			const T_CLASS *pParent = Iter->pParent;
			TTreeMapItemConstIterator IterFind = pParent ? m_TreeMap.find(pParent) : m_TreeMap.end();

			TV_INSERTSTRUCT tvinsert;													 // struct to config out tree control
			tvinsert.hParent = (IterFind == m_TreeMap.end()) ? NULL : IterFind->second;	 // top most level no need handle
			tvinsert.hInsertAfter = (IterFind == m_TreeMap.end()) ? TVI_ROOT : TVI_LAST; // work as root level

			std::wstring sName = getPathFileName((*Iter).pChild->sName);

			tvinsert.item.mask |= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvinsert.item.iImage = nMagicIndexBitmap;
			tvinsert.item.iSelectedImage = nMagicIndexBitmap;
			tvinsert.item.pszText = (LPWSTR)sName.c_str();

			HTREEITEM hItem = TreeView_InsertItem(m_hWnd, &tvinsert);

			m_TreeMap.insert(std::make_pair((*Iter).pChild, hItem)); // cache current

			// update preview
			TreeView_SelectDropTarget(m_hWnd, hItem);
			TreeView_SelectDropTarget(m_hWnd, NULL);
		}

		m_VecAddPairItems.clear();

		::LockWindowUpdate(NULL);

		m_CS.leave();

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	T_CLASS* GetTByItem(HTREEITEM item) const
	{
		T_CLASS *pOut = NULL;

		TTreeMapItemConstIterator iter = m_TreeMap.begin();

		while (iter != m_TreeMap.end())
		{
			if (iter->second == item)
			{
				pOut = const_cast<T_CLASS*>(iter->first);
				break;
			}
			++iter;
		}
		return pOut;
	}

private:
	TTreeMapItem		m_TreeMap;

	TVecAddPairItems	m_VecAddPairItems;

	TVecItems			m_VecAddParentItems;

	CriticalSection		m_CS;
};

#endif//__wtlitemstree_h__