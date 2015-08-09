#ifndef __panetreeveiw_h__
#define __panetreeview_h__

#pragma once

#include "IAssetBrowserFrame.h"
#include "WTLItemsTree.h"

template<class T_CLASS> struct CVoid {}; 

#define FILE_ATTR_DIRECTORY  0x00000010  

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template< class T_CLASS, class T_NODE > /*template<typename>*/
class CPaneTreeView : public CPaneContainer
{
public:
	BEGIN_MSG_MAP(CPaneTreeView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
		CHAIN_MSG_MAP(CPaneContainer)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	CPaneTreeView(IAssetBrowserFrame *pOwner)
		: m_pOwner(pOwner)
	{
		m_pTree = new CWTLItemsTree<T_NODE>();
	}

	//----------------------------------------------------------------------------------------------
	~CPaneTreeView()
	{
		delete m_pTree;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&bHendled)
	{
		RECT rcDefault = {0, 25, 200, 200};

		m_pTree->Create(m_hWnd, rcDefault, 0, WS_CHILD | WS_VISIBLE);

		HIMAGELIST pImageList = m_pOwner->GetImageList();
		if (pImageList){
			TreeView_SetImageList(m_pTree->m_hWnd, pImageList, TVSIL_NORMAL);
		}

		SetClient(m_pTree->m_hWnd);

		bHendled = FALSE;
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

		m_pTree->MoveWindow(0, 0, width, height);
		
		bHendled = FALSE;
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	void FillTree(TTreeMap<T_NODE, T_NODE> &tree)
	{
		if (TNodeMap<T_NODE, T_NODE> *pNode = tree.m_pFirstElement)
		{
			do
			{
				TNodeMap<T_NODE, T_NODE> *pRoot = pNode->GetRootNode();
				const T_NODE *pParentItem = pRoot ? pRoot->m_pValue : NULL;
				const T_NODE *pNodeItem = pNode->m_pValue;

				if (pNodeItem->fAttribute == FILE_ATTR_DIRECTORY){
					m_pTree->AddItem(pNodeItem, pParentItem);
				}

				pNode = tree.GetNext(pNode);
			} while (pNode);
		}
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnNotify(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		// select item handler
		if (((LPNMHDR)lParam)->code == TVN_SELCHANGED)
		{
			HTREEITEM hTreeItem = TreeView_GetSelection(m_pTree->m_hWnd);

			assert(hTreeItem);

			T_NODE *pItem = m_pTree->GetTByItem(hTreeItem);

			assert(pItem);

			m_pOwner->OnFolderSelected(pItem);

		}
		else if (((LPNMHDR)lParam)->code == NM_RCLICK) // context menu
		{	
			
		}
		return 0;
	}

private:
	class CWTLItemsTree<T_NODE>		*m_pTree;
	class IAssetBrowserFrame		*m_pOwner;
};

#endif//__panetreeview_h__