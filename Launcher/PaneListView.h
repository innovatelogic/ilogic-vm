#ifndef __panelistview_h__
#define __panelistview_h__

#pragma once

#include "IAssetBrowserFrame.h"
#include "DragDropSource.h"
#include "editors/scene_editor.h"

struct __declspec(uuid("DE5BF786-477A-11d2-839D-00C04FD918D0")) IDragSourceHelper;

typedef CWinTraitsOR<WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, WS_EX_CLIENTEDGE>	CListTraitsA;

// Version of CComObjectStack that doesn't freak out and assert when IUnknown
// methods are called.
template <class Base>
class CComObjectStack2 : public CComObjectStack<Base>
{
public:
	CComObjectStack2() : CComObjectStack<Base>() { }

	STDMETHOD_(ULONG, AddRef)() { return 1; }
	STDMETHOD_(ULONG, Release)() { return 1; }

	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
	{ return _InternalQueryInterface(iid, ppvObject); }
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template<class T_CLASS>
class CWTLList : public CWindowImpl<CWTLList<T_CLASS>, CListViewCtrl, CListTraitsA>
{
	typedef typename std::map<int, const T_CLASS*>	TListMapItem;
	typedef typename TListMapItem::iterator			TListMapItemIterator;
	typedef typename TListMapItem::const_iterator	TListMapItemConstIterator;

public:
	DECLARE_WND_SUPERCLASS(NULL, WC_LISTVIEW)

	BEGIN_MSG_MAP(CWTLList<T_CLASS>)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	//----------------------------------------------------------------------------------------------
	HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
		DWORD dwStyle = 0, DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U, 
		LPVOID lpCreateParam = NULL)
	{
		dwStyle |= LVS_REPORT; // "Details" mode
		dwStyle |= LVS_SINGLESEL;

		return CWindowImpl<CWTLList, CListViewCtrl, CListTraitsA>::Create(hWndParent,
			rect.m_lpRect, szWindowName, dwStyle, dwExStyle, MenuOrID.m_hMenu,
			lpCreateParam);
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		/*DWORD dwExStyle = 0;
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

		SetFont(m_Font);*/

		RECT rect;
		GetClientRect(&rect);

		int Width = rect.right - rect.left;
		int Height = rect.bottom - rect.top;

		AddColumn(TEXT("File"), 0, 0, LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT);
		//AddColumn(TEXT("Value"), 1, 1, LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_IMAGE, LVCFMT_LEFT);

		SetColumnWidth(0, (int)(Width * 0.95f));

		bHandled = FALSE;
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	void AddItem(const T_CLASS *pNodeItem)
	{
		m_CS.enter();

		LVITEM lvI;
		memset(&lvI, 0, sizeof(LVITEM));

		std::wstring wstr = getPathFileName(pNodeItem->sName);

		lvI.mask = LVIF_TEXT | LVIF_PARAM;
		lvI.iSubItem = 0;
		lvI.pszText = (LPWSTR)wstr.c_str();
		lvI.iItem = GetItemCount();
		lvI.iIndent = 0;

		int idx = InsertItem(&lvI);
		assert(idx != INDEX_NONE);

		m_ListMap.insert(std::make_pair(idx, pNodeItem)); // cache current

		m_CS.leave();
	}

	//----------------------------------------------------------------------------------------------
	void DeleteAll()
	{
		m_CS.enter();

		DeleteAllItems();

		m_ListMap.clear();

		m_CS.leave();
	}

	//----------------------------------------------------------------------------------------------
	const T_CLASS* GetNodeByIdx(int idx)
	{
		TListMapItemConstIterator iter_find = m_ListMap.find(idx);

		if (iter_find != m_ListMap.end()){
			return iter_find->second;
		}
		return NULL;
	}

private:
	TListMapItem		m_ListMap;
	CriticalSection		m_CS;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template<class T_CLASS, class T_NODE> /*template<typename>*/ 
class CPaneListView : public CPaneContainer
{
	BEGIN_MSG_MAP(CPaneListView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
		CHAIN_MSG_MAP(CPaneContainer)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

public:
	CPaneListView(IAssetBrowserFrame *pOwner)
		: m_pOwner(pOwner)
	{
		m_pList = new CWTLList<T_NODE>();
	}

	//----------------------------------------------------------------------------------------------
	~CPaneListView()
	{
		delete m_pList;
	}

    //----------------------------------------------------------------------------------------------
    void SetEditor(editors::TIEditor editor) { m_editor = editor; }

	//----------------------------------------------------------------------------------------------
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&bHendled)
	{
		RECT rcDefault = {0, 25, 200, 200};

		m_pList->Create(m_hWnd, rcDefault, 0, WS_CHILD | WS_VISIBLE);

		SetClient(m_pList->m_hWnd);

		bHendled = FALSE;
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL& bHendled)
	{
		const int scrollWidth = 20;

		RECT rc;
		GetClientRect(&rc);

		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;

		//m_pList->MoveWindow(0, 0, width, height);

		m_pList->SetColumnWidth(0, (int)(width - scrollWidth));

		bHendled = FALSE;
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnNotify(UINT iunt_, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		// select item handler
		if (((LPNMHDR)lParam)->code == LVN_ITEMCHANGED)
		{
			LPNMLISTVIEW hListItem = (LPNMLISTVIEW) lParam;

			UINT oldState = hListItem->uOldState; 
			UINT newState = hListItem->uNewState;

			BOOL oldSelected = (BOOL) (oldState & LVIS_SELECTED); 
			BOOL newSelected = (BOOL) (newState & LVIS_SELECTED);
			
			if (newSelected && !oldSelected) 
			{ 
				const T_NODE *pNode = m_pList->GetNodeByIdx(hListItem->iItem);

				assert(pNode);
                
				if (m_editor->Open(pNode->sName))
				{
					m_pOwner->SetOpenFileName(pNode->sName.c_str());
				}
			}
		}
		else if (((LPNMHDR)lParam)->code == NM_RCLICK) // context menu
		{	

		}
		else if (((LPNMHDR)lParam)->code == LVN_BEGINDRAG) // begin drag
		{
			LPNMLISTVIEW hListItem = (LPNMLISTVIEW) lParam;

			const T_NODE *pNode = m_pList->GetNodeByIdx(hListItem->iItem);
			assert(pNode);

			CComObjectStack2<CDragDropSource> dropsrc;
			CComPtr<IDragSourceHelper> pdsh;

			// Init the drag/drop data object.
			if (!dropsrc.Init(pNode->sName.c_str()))
			{
				ATLTRACE("Error: Couldn't init drop source object\n");
				return 0;   // do nothing
			}

			DWORD dwEffect = 0;

			HRESULT hr = dropsrc.DoDragDrop(DROPEFFECT_COPY, &dwEffect);
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	void FillList(TNodeMap<T_NODE, T_NODE> *pChild)
	{
		m_pList->DeleteAll();

		TNodeMap<T_NODE, T_NODE> *pNode = pChild;

		assert(pNode);

		size_t nNum = 0;

		do
		{
			const T_NODE *pNodeItem = pNode->m_pValue;

			if (pNodeItem->fAttribute != FILE_ATTR_DIRECTORY)
			{
				m_pList->AddItem(pNodeItem);
			}
			pNode = pNode->m_pNodeNextPlain;

		} while (pNode);
	}

	//----------------------------------------------------------------------------------------------
	void ClearList()
	{
		m_pList->DeleteAllItems();
	}

private:
	IAssetBrowserFrame *m_pOwner;
	class CWTLList<T_NODE>	*m_pList;

    editors::TIEditor    m_editor;
};

#endif//__panelistveiw_h__