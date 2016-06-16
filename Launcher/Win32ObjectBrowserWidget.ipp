#include "Win32ObjectBrowserWidget.h"

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
Win32ObjectBrowserWidget<T_CLASS>::Win32ObjectBrowserWidget(HWND hWndParent,
							 pContextMenuFunction pfMenu,
							 pContextMenuProcessor pfnMenuProcessor,
							 pGetResourceIconIndex pfnGetResourceIconIndex,
							 CALLBACK_FN pfnInvokeObject,
							 CALLBACK_FN pfnDirectInvokeObject,
							 CALLBACK_FN pfnClearObject,
							 CALLBACK_FN pfnDirectClearObject,
							 HIMAGELIST hImageList,
							 SRenderContext *pRenderContext = 0)
	: m_editor(nullptr)
    , m_pRegistry(nullptr)
	, m_hwndParent(hWndParent)
	, m_pfnContextMenu(pfMenu)
	, m_pfnContextMenuProcessor(pfnMenuProcessor)
	, m_pfnGetResourceIconIndex(pfnGetResourceIconIndex)
	, m_ActorPicked(0)
	, m_bDragging(false)
	, m_pfnInvokeObject(pfnInvokeObject)
	, m_pfnDirectInvokeObject(pfnDirectInvokeObject)
	, m_pfnClearObject(pfnClearObject)
	, m_pfnDirectClearObject(pfnDirectClearObject)
	, m_pRenderContext(pRenderContext)
	{
		RECT rect;
		GetClientRect(hWndParent, &rect);

		/*int Width = rect.right - rect.left;
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
			NULL);*/

     /*   RECT rc;
        rc.left = rect.left;
        rc.top = rect.top;
        rc.left = Width;
        rc.bottom = Height;
        m_hwndLeft.SubclassWindow(hWndParent);*/

        m_hwndTree = m_hwndLeft.Create(hWndParent, rect, NULL, 
            WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_READONLY | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE);

        //m_hwndLeft.SubclassWindow(m_hwndTree);

		SetWindowLong(m_hwndTree, GWL_USERDATA, (LONG)this);

		TreeView_SetImageList(m_hwndTree, hImageList, TVSIL_NORMAL);

		m_lpfnTreeProc = (WNDPROC) GetWindowLong(m_hwndTree, GWL_WNDPROC);

		m_hCursHand = LoadCursor(NULL, IDC_HAND);
		m_hCursArrow = LoadCursor(NULL, IDC_ARROW);
	}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
Win32ObjectBrowserWidget<T_CLASS>::~Win32ObjectBrowserWidget()
{

}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void Win32ObjectBrowserWidget<T_CLASS>::SetEditor(editors::TIEditor editor)
{
    m_editor = editor;
    m_pRegistry = editor->GetApp()->GetRegistry();
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
int Win32ObjectBrowserWidget<T_CLASS>::InvokeActor(const T_CLASS *pSender)
{
    int bResult = 0;

    //m_CS.enter();

    if (m_pRegistry) // TODO: fragile code REDESIGN
    {
        if (m_pRegistry->IsEditorVisible(pSender->GetType()))
        {
            T_CLASS * pParent = pSender->GetParent();

            if ((m_ActorAddList.size() == 0) // initial enter
                ||
                (std::find(m_ActorAddList.begin(), m_ActorAddList.end(), pParent) != m_ActorAddList.end() ||
                    m_TreeMap.find(pParent) != m_TreeMap.end())) // already or been added
            {
                m_ActorAddList.push_back(const_cast<T_CLASS*>(pSender));
            }

            ::PostMessage(m_hwndParent, WM_USER_INSERTOBJECT, 0, 0);

            bResult = 1;
        }
    }

    //m_CS.leave();

    return bResult;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void Win32ObjectBrowserWidget<T_CLASS>::Update(const T_CLASS *pSender, ESystemEventID EventId)
{
    switch (EventId)
    {
    case Event_ObjectGenerated:
        InvokeActor(pSender);
        break;

    case Event_PreRelease:
    case Event_OnRemoveObject:
        ClearActor(pSender);
        break;

    case Event_OnSelected:
        SelectActor(pSender);
        break;

    case Event_ObjectRename:
        RenameActor(pSender);
        break;

    case Event_MoveObjectUp:
    case Event_MoveObjectDown:
        MoveActor(pSender, EventId == Event_MoveObjectUp);
        break;

    case Event_ObjectReArranged:
        RearrangeActor(pSender);
        break;
    };
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
bool Win32ObjectBrowserWidget<T_CLASS>::ClearActor(const T_CLASS *pSender)
{
    m_CS.enter();

    bool bPostMsg = false;

    TTreeMapActorIterator IterFind = m_TreeMap.find(pSender);

    if (IterFind != m_TreeMap.end())
    {
        m_HTreeClearList.push_back(IterFind->second);
        m_TreeMap.erase(IterFind);
        bPostMsg = true;
    }

    std::vector<T_CLASS*>::iterator Iter = std::find(m_ActorsRearrange.begin(), m_ActorsRearrange.end(), pSender);
    if (Iter != m_ActorsRearrange.end()) {
        m_ActorsRearrange.erase(Iter);
    }

    if (bPostMsg) {
        ::PostMessage(m_hwndParent, WM_USER_REMOVEOBJECT_BRWSR, 0, 0);
    }
    m_CS.leave();

    return bPostMsg;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
int Win32ObjectBrowserWidget<T_CLASS>::SelectActor(const T_CLASS * Sender)
{
    m_CS.enter();

    TTreeMapActorIterator Iter = m_TreeMap.find(Sender);
    m_HTreeSelected = (Iter != m_TreeMap.end()) ? Iter->second : NULL;

    ::PostMessage(m_hwndParent, WM_USER_SELECTOBJECT_BRWSR, 0, 0);

    m_CS.leave();

    return 1;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
int Win32ObjectBrowserWidget<T_CLASS>::RenameActor(const T_CLASS * Sender)
{
    bool bReturn = false;

    m_CS.enter();

    TTreeMapActorIterator Iter = m_TreeMap.find(Sender);
    m_HTreeRename = (Iter != m_TreeMap.end()) ? Iter->second : NULL;

    if (m_HTreeRename != NULL)
    {
        ::PostMessage(m_hwndParent, WM_USER_RENAMEOBJECT_BRWSR, 0, 0);
        bReturn = true;
    }
    m_CS.leave();

    return bReturn;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
int Win32ObjectBrowserWidget<T_CLASS>::MoveActor(const T_CLASS * Sender, bool bUp)
{
    m_CS.enter();

    TTreeMapActorIterator IterMove = m_TreeMap.find(Sender);

    if (IterMove != m_TreeMap.end())
    {
        SwapHTree Temp;
        Temp.ActorMove = Sender;
        Temp.bUp = bUp;

        m_TMapSwapTree.push_back(Temp);
    }

    ::PostMessage(m_hwndParent, WM_USER_MOVEOBJECT, 0, 0);

    m_CS.leave();

    return 1;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
int Win32ObjectBrowserWidget<T_CLASS>::RearrangeActor(const T_CLASS * Sender)
{
    m_CS.enter();

    std::vector<T_CLASS*>::iterator IterFind = std::find(m_ActorsRearrange.begin(), m_ActorsRearrange.end(), Sender);

    if (IterFind == m_ActorsRearrange.end())
    {
        m_ActorsRearrange.push_back(const_cast<T_CLASS*>(Sender));
    }

    ::PostMessage(m_hwndParent, WM_USER_REARRANGE, 0, 0);

    m_CS.leave();

    return 1;
}

template<class T_CLASS>
void Win32ObjectBrowserWidget<T_CLASS>::WndProcessInsertObject()
{
    m_CS.enter();

    ::LockWindowUpdate(m_hwndTree);

    for (TVecActorChildIterator Iter = m_ActorAddList.begin(); Iter != m_ActorAddList.end(); ++Iter)
    {
        int IndexBitmap = m_pfnGetResourceIconIndex((*Iter)->GetType());

        // add data to tree view
        TTreeMapActorIterator Iterator = m_TreeMap.find((*Iter)->GetParent());

        TV_INSERTSTRUCT tvinsert;													 // struct to config out tree control
        tvinsert.hParent = (Iterator == m_TreeMap.end()) ? NULL : Iterator->second;	 // top most level no need handle
        tvinsert.hInsertAfter = (Iterator == m_TreeMap.end()) ? TVI_ROOT : TVI_LAST;   // work as root level

        std::wstring Name = ConvertStringToWideString((*Iter)->GetName());

        tvinsert.item.mask = TVIF_TEXT;

        if (IndexBitmap != INDEX_NONE)
        {
            tvinsert.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
            tvinsert.item.iImage = IndexBitmap;
            tvinsert.item.iSelectedImage = IndexBitmap;
        }

        tvinsert.item.pszText = (LPWSTR)Name.c_str();

        HTREEITEM hItem = TreeView_InsertItem(m_hwndTree, &tvinsert);

        m_TreeMap.insert(std::make_pair(*Iter, hItem));	// cashe current

                                                        // update preview
        TreeView_SelectDropTarget(m_hwndTree, hItem);
        TreeView_SelectDropTarget(m_hwndTree, NULL);
    }

    m_ActorAddList.clear();

    ::LockWindowUpdate(NULL);

    m_CS.leave();
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void Win32ObjectBrowserWidget<T_CLASS>::WndRemoveObject()
{
    m_CS.enter();

    ::LockWindowUpdate(m_hwndTree);

    for (std::vector<HTREEITEM>::iterator Iter = m_HTreeClearList.begin(); Iter != m_HTreeClearList.end(); ++Iter)
    {
        TreeView_DeleteItem(m_hwndTree, *Iter);
    }

    m_HTreeClearList.clear();

    ::LockWindowUpdate(NULL);

    m_CS.leave();
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void Win32ObjectBrowserWidget<T_CLASS>::WndSelectObject()
{
    m_CS.enter();

    TreeView_Select(m_hwndTree, m_HTreeSelected, TVGN_CARET);
    TreeView_EnsureVisible(m_hwndTree, m_HTreeSelected);

    m_HTreeSelected = NULL;

    m_CS.leave();
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void Win32ObjectBrowserWidget<T_CLASS>::WndRenameObject()
{
    m_CS.enter();

    for (TTreeMapActorIterator Iter = m_TreeMap.begin(); Iter != m_TreeMap.end(); ++Iter)
    {
        if (Iter->second == m_HTreeRename)
        {
            TVITEMEX NodeItem;
            NodeItem.hItem = Iter->second;
            NodeItem.cchTextMax = 100;
            NodeItem.pszText = 0;
            NodeItem.mask = TVIF_TEXT;

            if (TreeView_GetItem(m_hwndTree, &NodeItem))
            {
                std::wstring Name = ConvertStringToWideString(Iter->first->GetName());
                NodeItem.pszText = (LPWSTR)Name.c_str();
                TreeView_SetItem(m_hwndTree, &NodeItem);
            }
            break;
        }
    }
    m_CS.leave();
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void Win32ObjectBrowserWidget<T_CLASS>::WndMoveObject()
{
    m_CS.enter();

    std::vector<SwapHTree>::iterator HIter = m_TMapSwapTree.begin();

    while (HIter != m_TMapSwapTree.end())
    {
        TTreeMapActorIterator IterMove = m_TreeMap.find(HIter->ActorMove);

        HTREEITEM HMoveTo = TreeView_GetNextItem(m_hwndTree, IterMove->second, (HIter->bUp) ? TVGN_PREVIOUS : TVGN_NEXT);
        TTreeMapActorIterator IterMoveTo = m_TreeMap.find(GetActorByData(HMoveTo));

        if (IterMove != m_TreeMap.end() && IterMoveTo != m_TreeMap.end())
        {
            wchar_t NameMove[100] = { 0 };
            wchar_t NameMoveTo[100] = { 0 };

            MultiByteToWideChar(CP_ACP, 0, IterMove->first->GetName(), -1, NameMove, 100);
            MultiByteToWideChar(CP_ACP, 0, IterMoveTo->first->GetName(), -1, NameMoveTo, 100);

            // prepare tree items directly
            TVITEMEX NodeItemMove;
            NodeItemMove.hItem = IterMove->second;
            NodeItemMove.cchTextMax = 100;
            NodeItemMove.pszText = (LPWSTR)NameMoveTo;
            NodeItemMove.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
            NodeItemMove.iImage = m_pfnGetResourceIconIndex(IterMoveTo->first->GetType());
            NodeItemMove.iSelectedImage = NodeItemMove.iImage;

            TVITEMEX NodeItemMoveTo;
            NodeItemMoveTo.hItem = IterMoveTo->second;
            NodeItemMoveTo.cchTextMax = 100;
            NodeItemMoveTo.pszText = (LPWSTR)NameMove;
            NodeItemMoveTo.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
            NodeItemMoveTo.iImage = m_pfnGetResourceIconIndex(IterMove->first->GetType());
            NodeItemMoveTo.iSelectedImage = NodeItemMoveTo.iImage;

            BOOL b1 = TreeView_SetItem(m_hwndTree, &NodeItemMove);
            BOOL b2 = TreeView_SetItem(m_hwndTree, &NodeItemMoveTo);

            assert(b1 && b2);

            // swap TreeMap HTREEITEMs
            std::swap(IterMove->second, IterMoveTo->second);

            // clear all child from tree
            // update tree items directly
            TVecActorChild ChildNodes_ActorMove = IterMove->first->m_ChildNodes;
            TVecActorChild ChildNodes_ActorMoveTo = IterMoveTo->first->m_ChildNodes;

            for (size_t Index = 0; Index < ChildNodes_ActorMove.size(); ++Index) {
                ChildNodes_ActorMove[Index]->ForeachTreeNodes(m_pfnDirectClearObject);//GCALLBACK_DirectClearActor
            }
            for (size_t Index = 0; Index < ChildNodes_ActorMoveTo.size(); ++Index) {
                ChildNodes_ActorMoveTo[Index]->ForeachTreeNodes(m_pfnDirectClearObject);//GCALLBACK_DirectClearActor
            }

            // register again
            for (size_t Index = 0; Index < ChildNodes_ActorMove.size(); ++Index) {
                ChildNodes_ActorMove[Index]->ForeachTreeNodes(m_pfnDirectInvokeObject);//GCALLBACK_DirectInvokeActor
            }
            for (size_t Index = 0; Index < ChildNodes_ActorMoveTo.size(); ++Index) {
                ChildNodes_ActorMoveTo[Index]->ForeachTreeNodes(m_pfnDirectInvokeObject);//GCALLBACK_DirectInvokeActor
            }

            // select at last
            TreeView_SelectItem(m_hwndTree, IterMove->second);
        }

        HIter++;
    }

    m_TMapSwapTree.clear();
    m_CS.leave();
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void Win32ObjectBrowserWidget<T_CLASS>::WndReArrange()
{
    m_CS.enter();
    std::vector<T_CLASS*>::iterator Iter = m_ActorsRearrange.begin();

    while (Iter != m_ActorsRearrange.end())
    {
        // clear
        for (std::vector<T_CLASS*>::iterator IterChild = (*Iter)->m_ChildNodes.begin(); IterChild != (*Iter)->m_ChildNodes.end(); ++IterChild) {
            (*IterChild)->ForeachTreeNodes(m_pfnDirectClearObject); //CALLBACK_DirectClearActor
        }

        // register again
        for (std::vector<T_CLASS*>::iterator IterChild = (*Iter)->m_ChildNodes.begin(); IterChild != (*Iter)->m_ChildNodes.end(); ++IterChild) {
            (*IterChild)->ForeachTreeNodes(m_pfnDirectInvokeObject);//CALLBACK_DirectInvokeActor
        }

        ++Iter;
    }

    m_ActorsRearrange.clear();
    m_CS.leave();
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
int Win32ObjectBrowserWidget<T_CLASS>::DirectInvokeActor(const T_CLASS * Sender)
{
    int OutResult = 0;

    m_CS.enter();

    if (m_hwndTree &&
        !dynamic_cast<const Brush_AbstractInterface*>(Sender) && m_pRegistry->IsEditorVisible(Sender->GetType()))
    {
        int IndexBitmap = m_pfnGetResourceIconIndex(Sender->GetType());

        // add data to tree view
        TTreeMapActorIterator Iterator = m_TreeMap.find(Sender->GetParent());

        TV_INSERTSTRUCT tvinsert;                                                  // struct to config out tree control
        tvinsert.hParent = (Iterator == m_TreeMap.end()) ? NULL : Iterator->second;  // top most level no need handle
        tvinsert.hInsertAfter = (Iterator == m_TreeMap.end()) ? TVI_ROOT : TVI_LAST; // work as root level

        tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

        std::wstring Name = ConvertStringToWideString(Sender->GetName());
        tvinsert.item.pszText = (LPWSTR)Name.c_str();
        tvinsert.item.iImage = IndexBitmap;
        tvinsert.item.iSelectedImage = IndexBitmap;

        HTREEITEM hItem = TreeView_InsertItem(m_hwndTree, &tvinsert);

        // cashe current
        m_TreeMap.insert(std::make_pair(Sender, hItem));

        // update preview
        TreeView_SelectDropTarget(m_hwndTree, hItem);
        TreeView_SelectDropTarget(m_hwndTree, NULL);

        OutResult = 1;
    }

    m_CS.leave();

    return OutResult;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
int	Win32ObjectBrowserWidget<T_CLASS>::DirectClearActor(const T_CLASS * Sender)
{
    m_CS.enter();

    TTreeMapActorIterator Iter = m_TreeMap.find(Sender);

    if (Iter != m_TreeMap.end())
    {
        TreeView_DeleteItem(m_hwndTree, Iter->second);
        m_TreeMap.erase(Iter);
    }

    m_CS.leave();

    return 1;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
T_CLASS* Win32ObjectBrowserWidget<T_CLASS>::GetActorByData(const HTREEITEM lpnmtv)
{
    m_CS.enter();

    T_CLASS *pOutValue = NULL;

    if (lpnmtv)
    {
        for (std::vector<HTREEITEM>::iterator Iter = m_HTreeClearList.begin(); Iter != m_HTreeClearList.end(); ++Iter)
        {
            if (*Iter == lpnmtv) {
                return 0;
            }
        }

        TTreeMapActorConstIterator Iter = m_TreeMap.begin();
        while (Iter != m_TreeMap.end())
        {
            if (Iter->second == lpnmtv)
            {
                pOutValue = const_cast<T_CLASS*>(Iter->first);
                break;
            }
            ++Iter;
        }
    }
    m_CS.leave();

    return pOutValue;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
bool Win32ObjectBrowserWidget<T_CLASS>::SelChangedTreeObject()
{
    bool bResult = false;

    m_CS.enter();

    std::vector<CActor*> actors;

    for (size_t i = 0; i < m_hwndLeft.m_aData.GetSize(); i++)
    {
        if (m_hwndLeft.m_aData.GetValueAt(i).bSelected)
        {
            T_CLASS *actor = GetActorByData(m_hwndLeft.m_aData.GetValueAt(i).hItem);

            actors.push_back(actor);

           // actor->BroadcastEvent(Event_OnSelected);
        }
    }

    m_editor->SelectActors(actors);

    

    /*
    HTREEITEM hTreeItem = TreeView_GetSelection(m_hwndTree);

    TVITEMEX NodeItem;
    NodeItem.hItem = hTreeItem;
    NodeItem.cchTextMax = 100;
    NodeItem.pszText = 0;
    NodeItem.mask = TVIF_TEXT;
    TreeView_GetItem(m_hwndTree, &NodeItem);

    CCoreSDK *pCoreSDK = m_editor->GetApp();

    T_CLASS *pActor = GetActorByData(hTreeItem);

    if (pActor) // find corresponding
    {
        //IDrawInterface *pIFocused = m_editor->GetByActor(pActor);

        //if (pIFocused)
        //{
        //pCoreSDK->GetViewportManager()->SetFocus(pIFocused); // set focused state

        std::vector<CActor*> actors = { pActor };
        m_editor->SelectActors(actors);
        //}
        //else
        //{
        //pCoreSDK->GetViewportManager()->SetFocus(0);

        //    m_editor->DeselectAll();
        //}

        pActor->BroadcastEvent(Event_OnSelected);
        bResult = true;
    }*/

    m_CS.leave();

    return bResult;
}

/*
#include "LauncherStdAfx.h"

#define WM_USER_INSERTOBJECT				WM_APP+1
#define WM_USER_REMOVEOBJECT				WM_APP+2
#define WM_USER_SELECTOBJECT				WM_APP+3
#define WM_USER_RENAMEOBJECT				WM_APP+4
#define WM_USER_MOVEOBJECT					WM_APP+5
#define WM_USER_REARRANGE					WM_APP+6

//----------------------------------------------------------------------------------------------
Win32ObjectBrowserWidget::Win32ObjectBrowserWidget(HWND hWndParent,
												   const CWindowManager *WMgr,
												   pContextMenuFunction pfMenu,
												   pContextMenuProcessor pfnMenuProcessor,
												   pGetResourceIconIndex pfnGetResourceIconIndex,
												   HIMAGELIST hImageList)
: m_pWindowManager(WMgr)
, m_hwndParent(hWndParent)
, m_pfnContextMenu(pfMenu)
, m_pfnContextMenuProcessor(pfnMenuProcessor)
, m_pfnGetResourceIconIndex(pfnGetResourceIconIndex)
, m_ActorPicked(0)
, m_bDragging(false)

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
Win32ObjectBrowserWidget::~Win32ObjectBrowserWidget()
{

}

//----------------------------------------------------------------------------------------------
int Win32ObjectBrowserWidget::InvokeActor(const CActor * Sender)
{
	int bResult = 0;

	const Registry * Reg = m_pWindowManager->GetCoreSDK()->GetRegistry();

	m_CS.enter();

	if (!dynamic_cast<const Brush_AbstractInterface*>(Sender) && Reg->IsEditorVisible(Sender->GetType()))
	{
		CActor * pParent = Sender->GetParent();
		
		if (!pParent || 
			(pParent &&	(std::find(m_ActorAddList.begin(), m_ActorAddList.end(), pParent) != m_ActorAddList.end() || 
			m_TreeMap.find(pParent) != m_TreeMap.end())) // already or been added
			)
		{
			m_ActorAddList.push_back(const_cast<CActor*>(Sender));
		}

		::PostMessage(m_hwndParent, WM_USER_INSERTOBJECT, 0, 0);
		bResult = 1;
	}

	m_CS.leave();

	return bResult;
}

//----------------------------------------------------------------------------------------------
int Win32ObjectBrowserWidget::ClearActor(const CActor * Sender)
{
	m_CS.enter();

	TTreeMapActorIterator IterFind = m_TreeMap.find(Sender);
	if (IterFind != m_TreeMap.end())
	{
		m_HTreeClearList.push_back(IterFind->second);
		m_TreeMap.erase(IterFind);
	}

	std::vector<CActor*>::iterator Iter = std::find(m_ActorsRearrange.begin(), m_ActorsRearrange.end(), Sender);
	if (Iter != m_ActorsRearrange.end()){
		m_ActorsRearrange.erase(Iter);
	}

	::PostMessage(m_hwndParent, WM_USER_REMOVEOBJECT, 0, 0);

	m_CS.leave();

	return 1;
}

//----------------------------------------------------------------------------------------------
int Win32ObjectBrowserWidget::SelectActor(const CActor * Sender)
{
	m_CS.enter();

	TTreeMapActorIterator Iter = m_TreeMap.find(Sender);
	m_HTreeSelected = (Iter != m_TreeMap.end()) ? Iter->second : NULL;
	
	::PostMessage(m_hwndParent, WM_USER_SELECTOBJECT, 0, 0);
	m_CS.leave();

	return 1;
}

//----------------------------------------------------------------------------------------------
int Win32ObjectBrowserWidget::RenameActor(const class CActor * Sender)
{
	m_CS.enter();

	TTreeMapActorIterator Iter = m_TreeMap.find(Sender);
	m_HTreeRename = (Iter != m_TreeMap.end()) ? Iter->second : NULL;

	::PostMessage(m_hwndParent, WM_USER_RENAMEOBJECT, 0, 0);
	m_CS.leave();

	return 1;
}

//----------------------------------------------------------------------------------------------
int Win32ObjectBrowserWidget::MoveActor(const CActor * Sender, bool bUp)
{
	m_CS.enter();

	TTreeMapActorIterator IterMove = m_TreeMap.find(Sender);

	if (IterMove != m_TreeMap.end())
	{
		SwapHTree Temp;
		Temp.ActorMove = Sender;
		Temp.bUp = bUp; 

		m_TMapSwapTree.push_back(Temp);
	}

	::PostMessage(m_hwndParent, WM_USER_MOVEOBJECT, 0, 0);

	m_CS.leave();

	return 1;
}

//----------------------------------------------------------------------------------------------
int Win32ObjectBrowserWidget::RearrangeActor(const CActor * Sender)
{
	m_CS.enter();

	std::vector<CActor*>::iterator IterFind = std::find(m_ActorsRearrange.begin(), m_ActorsRearrange.end(), Sender);

	if (IterFind == m_ActorsRearrange.end())
	{
		m_ActorsRearrange.push_back(const_cast<CActor*>(Sender));
	}

	::PostMessage(m_hwndParent, WM_USER_REARRANGE, 0, 0);

	m_CS.leave();

	return 1;
}

//----------------------------------------------------------------------------------------------
void Win32ObjectBrowserWidget::WndProcessInsertObject()
{
	m_CS.enter();

	::LockWindowUpdate(m_hwndTree);

	for (TVecActorChildIterator Iter = m_ActorAddList.begin(); Iter != m_ActorAddList.end(); ++Iter)
	{
		int IndexBitmap = m_pfnGetResourceIconIndex((*Iter)->GetType());

		// add data to tree view
		TTreeMapActorIterator Iterator = m_TreeMap.find((*Iter)->GetParent());

		TV_INSERTSTRUCT tvinsert;													 // struct to config out tree control
		tvinsert.hParent = (Iterator == m_TreeMap.end()) ? NULL : Iterator->second;	 // top most level no need handle
		tvinsert.hInsertAfter = (Iterator == m_TreeMap.end()) ? TVI_ROOT : TVI_LAST;   // work as root level

		std::wstring Name = ConvertStringToWideString((*Iter)->GetName());

		tvinsert.item.mask = TVIF_TEXT;

		if (IndexBitmap != INDEX_NONE)
		{
			tvinsert.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvinsert.item.iImage = IndexBitmap;
			tvinsert.item.iSelectedImage = IndexBitmap;
		}
				
		tvinsert.item.pszText = (LPWSTR)Name.c_str();
		
		HTREEITEM hItem = TreeView_InsertItem(m_hwndTree, &tvinsert);
		
		m_TreeMap.insert(std::make_pair(*Iter, hItem));	// cashe current

		// update preview
		TreeView_SelectDropTarget(m_hwndTree, hItem);
		TreeView_SelectDropTarget(m_hwndTree, NULL);
	}

	m_ActorAddList.clear();

	::LockWindowUpdate(NULL);

	m_CS.leave();
}

//----------------------------------------------------------------------------------------------
void Win32ObjectBrowserWidget::WndRemoveObject()
{
	m_CS.enter();

	::LockWindowUpdate(m_hwndTree);

	for (std::vector<HTREEITEM>::iterator Iter = m_HTreeClearList.begin(); Iter != m_HTreeClearList.end(); ++Iter)
	{
		TreeView_DeleteItem(m_hwndTree, *Iter);
	}

	m_HTreeClearList.clear();

	::LockWindowUpdate(NULL);

	m_CS.leave();
}

//----------------------------------------------------------------------------------------------
void Win32ObjectBrowserWidget::WndSelectObject()
{
	m_CS.enter();

	TreeView_Select(m_hwndTree, m_HTreeSelected, TVGN_CARET);
	TreeView_EnsureVisible(m_hwndTree, m_HTreeSelected);

	m_HTreeSelected = NULL;

	m_CS.leave();
}

//----------------------------------------------------------------------------------------------
void Win32ObjectBrowserWidget::WndRenameObject()
{
	m_CS.enter();

	for (TTreeMapActorIterator Iter = m_TreeMap.begin(); Iter != m_TreeMap.end(); ++Iter)
	{
		if (Iter->second == m_HTreeRename)
		{
			TVITEMEX NodeItem;
			NodeItem.hItem = Iter->second;
			NodeItem.cchTextMax = 100;
			NodeItem.pszText = 0;
			NodeItem.mask = TVIF_TEXT;

			if (TreeView_GetItem(m_hwndTree, &NodeItem))
			{
				std::wstring Name = ConvertStringToWideString(Iter->first->GetName());
				NodeItem.pszText =(LPWSTR)Name.c_str();
				TreeView_SetItem(m_hwndTree, &NodeItem);
			}
			break;
		}
	}
	m_CS.leave();
}

//----------------------------------------------------------------------------------------------
void Win32ObjectBrowserWidget::WndMoveObject()
{
	m_CS.enter();

	std::vector<SwapHTree>::iterator HIter = m_TMapSwapTree.begin();

	while (HIter != m_TMapSwapTree.end())
	{
		TTreeMapActorIterator IterMove = m_TreeMap.find(HIter->ActorMove);

		HTREEITEM HMoveTo = TreeView_GetNextItem(m_hwndTree, IterMove->second, (HIter->bUp) ? TVGN_PREVIOUS : TVGN_NEXT);
		TTreeMapActorIterator IterMoveTo = m_TreeMap.find(GetActorByData(HMoveTo));

		if (IterMove != m_TreeMap.end() && IterMoveTo != m_TreeMap.end())
		{
			wchar_t NameMove[100] = {0};
			wchar_t NameMoveTo[100] = {0};

			MultiByteToWideChar(CP_ACP, 0, IterMove->first->GetName(), -1, NameMove, 100);
			MultiByteToWideChar(CP_ACP, 0, IterMoveTo->first->GetName(), -1, NameMoveTo, 100);

			// prepare tree items directly
			TVITEMEX NodeItemMove;
			NodeItemMove.hItem = IterMove->second;
			NodeItemMove.cchTextMax = 100;
			NodeItemMove.pszText = (LPWSTR)NameMoveTo;
			NodeItemMove.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			NodeItemMove.iImage = m_pfnGetResourceIconIndex(IterMoveTo->first->GetType());
			NodeItemMove.iSelectedImage = NodeItemMove.iImage;

			TVITEMEX NodeItemMoveTo;
			NodeItemMoveTo.hItem = IterMoveTo->second;
			NodeItemMoveTo.cchTextMax = 100;
			NodeItemMoveTo.pszText = (LPWSTR)NameMove;
			NodeItemMoveTo.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			NodeItemMoveTo.iImage = m_pfnGetResourceIconIndex(IterMove->first->GetType());
			NodeItemMoveTo.iSelectedImage = NodeItemMoveTo.iImage;

			BOOL b1 = TreeView_SetItem(m_hwndTree, &NodeItemMove);
			BOOL b2 = TreeView_SetItem(m_hwndTree, &NodeItemMoveTo);

			assert(b1 && b2);

			// swap TreeMap HTREEITEMs
			std::swap(IterMove->second, IterMoveTo->second);

			// clear all child from tree
			// update tree items directly
			TVecActorChild ChildNodes_ActorMove	= IterMove->first->ChildNodes;
			TVecActorChild ChildNodes_ActorMoveTo = IterMoveTo->first->ChildNodes;

			for (size_t Index = 0; Index < ChildNodes_ActorMove.size(); ++Index){
				ChildNodes_ActorMove[Index]->ForeachTreeNodes(CWin32ObjectBrowser::CALLBACK_DirectClearActor);
			}
			for (size_t Index = 0; Index < ChildNodes_ActorMoveTo.size(); ++Index){
				ChildNodes_ActorMoveTo[Index]->ForeachTreeNodes(CWin32ObjectBrowser::CALLBACK_DirectClearActor);
			}

			// register again
			for (size_t Index = 0; Index < ChildNodes_ActorMove.size(); ++Index){
				ChildNodes_ActorMove[Index]->ForeachTreeNodes(CWin32ObjectBrowser::CALLBACK_DirectInvokeActor);
			}
			for (size_t Index = 0; Index < ChildNodes_ActorMoveTo.size(); ++Index){
				ChildNodes_ActorMoveTo[Index]->ForeachTreeNodes(CWin32ObjectBrowser::CALLBACK_DirectInvokeActor);
			}

			// select at last
			TreeView_SelectItem(m_hwndTree, IterMove->second);
		}

		HIter++;
	}

	m_TMapSwapTree.clear();
	m_CS.leave();
}

//----------------------------------------------------------------------------------------------
void Win32ObjectBrowserWidget::WndReArrange()
{
	m_CS.enter();
	std::vector<CActor*>::iterator Iter = m_ActorsRearrange.begin();

	while (Iter != m_ActorsRearrange.end())
	{
		// clear
		for (std::vector<CActor*>::iterator IterChild = (*Iter)->ChildNodes.begin(); IterChild != (*Iter)->ChildNodes.end(); ++IterChild){
			(*IterChild)->ForeachTreeNodes(CWin32ObjectBrowser::CALLBACK_DirectClearActor);
		}

		// register again
		for (std::vector<CActor*>::iterator IterChild = (*Iter)->ChildNodes.begin(); IterChild != (*Iter)->ChildNodes.end(); ++IterChild){
			(*IterChild)->ForeachTreeNodes(CWin32ObjectBrowser::CALLBACK_DirectInvokeActor);
		}

		++Iter;
	}

	m_ActorsRearrange.clear();
	m_CS.leave();
}

//----------------------------------------------------------------------------------------------
int Win32ObjectBrowserWidget::DirectInvokeActor(const CActor * Sender)
{
	int OutResult = 0;
	const Registry * Reg = m_pWindowManager->GetCoreSDK()->GetRegistry();

	m_CS.enter();

	if (m_hwndTree &&
		!dynamic_cast<const Brush_AbstractInterface*>(Sender) &&
		Reg->IsEditorVisible(Sender->GetType()))
	{
		int IndexBitmap = m_pfnGetResourceIconIndex(Sender->GetType());

		// add data to tree view
		TTreeMapActorIterator Iterator = m_TreeMap.find(Sender->GetParent());

		TV_INSERTSTRUCT tvinsert;                                                  // struct to config out tree control
		tvinsert.hParent = (Iterator == m_TreeMap.end()) ? NULL : Iterator->second;  // top most level no need handle
		tvinsert.hInsertAfter = (Iterator == m_TreeMap.end()) ? TVI_ROOT : TVI_LAST; // work as root level

		tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

		std::wstring Name = ConvertStringToWideString(Sender->GetName());
		tvinsert.item.pszText = (LPWSTR)Name.c_str();
		tvinsert.item.iImage = IndexBitmap;
		tvinsert.item.iSelectedImage = IndexBitmap;

		HTREEITEM hItem = TreeView_InsertItem(m_hwndTree, &tvinsert);

		// cashe current
		m_TreeMap.insert(std::make_pair(Sender, hItem));

		// update preview
		TreeView_SelectDropTarget(m_hwndTree, hItem);
		TreeView_SelectDropTarget(m_hwndTree, NULL);

		OutResult = 1;
	}

	m_CS.leave();

	return OutResult;
}

//----------------------------------------------------------------------------------------------
int	Win32ObjectBrowserWidget::DirectClearActor(const CActor * Sender)
{
	m_CS.enter();

	TTreeMapActorIterator Iter = m_TreeMap.find(Sender);

	if (Iter != m_TreeMap.end())
	{
		TreeView_DeleteItem(m_hwndTree, Iter->second);
		m_TreeMap.erase(Iter);
	}

	m_CS.leave();

	return 1;
}

//----------------------------------------------------------------------------------------------
CActor* Win32ObjectBrowserWidget::GetActorByData(const HTREEITEM lpnmtv)
{
	m_CS.enter();

	CActor *pOutValue = NULL;

	if (lpnmtv)
	{
		for (std::vector<HTREEITEM>::iterator Iter = m_HTreeClearList.begin(); Iter != m_HTreeClearList.end(); ++Iter)
		{
			if (*Iter == lpnmtv){
				return 0;
			}
		}

		TTreeMapActorConstIterator Iter = m_TreeMap.begin();
		while (Iter != m_TreeMap.end())
		{
			if (Iter->second == lpnmtv)
			{
				pOutValue = const_cast<CActor*>(Iter->first);
				break;
			}
			++Iter;
		}
	}
	m_CS.leave();

	return pOutValue;
}

//----------------------------------------------------------------------------------------------
bool Win32ObjectBrowserWidget::SelChangedTreeObject()
{
	bool bResult = false;

	m_CS.enter();

	HTREEITEM hTreeItem = TreeView_GetSelection(m_hwndTree);

	TVITEMEX NodeItem;
	NodeItem.hItem = hTreeItem;
	NodeItem.cchTextMax = 100;
	NodeItem.pszText = 0;
	NodeItem.mask = TVIF_TEXT;
	TreeView_GetItem(m_hwndTree, &NodeItem);

	CCoreSDK *pCoreSDK = m_pWindowManager->GetCoreSDK();

	CActor *pActor = GetActorByData(hTreeItem);

	if (pActor) // find corresponding
	{
		IDrawInterface *pIFocused = pCoreSDK->GetViewportManager()->GetByActor(pActor);
		if (pIFocused && !pIFocused->IsFocused())
		{
			pCoreSDK->GetViewportManager()->SetFocus(pIFocused); // set focused state
		}
		else
		{
			pCoreSDK->GetViewportManager()->SetFocus(0);
		}

		pActor->BroadcastEvent(Event_OnSelected);
		bResult = true;
	}

	m_CS.leave();

	return bResult;
}

//------------------------------------------------------------------------
bool Win32ObjectBrowserWidget::RenameTreeObject()
{
	bool bResult = false;

	HTREEITEM hTreeItem = TreeView_GetSelection(m_hwndTree);

	CActor* Actor = GetActorByData(hTreeItem);

	if (Actor) // do not allow rename root node
	{
		wchar_t wbuf[100] = {0};
		char ascii[100] = {0};

		MultiByteToWideChar(CP_ACP, 0, Actor->GetName(), -1, wbuf, 100);

		if (CWin32InputBox::InputBox(L"Rename", L"Please enter new name", wbuf, 100, false) == IDOK)
		{
			wcstombs(ascii, wbuf, wcslen(wbuf));
			Actor->Rename(ascii);
		}
		bResult = true; // operation clear
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool Win32ObjectBrowserWidget::WndProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool bResult = false;

	switch (message) 
	{
	case WM_USER_INSERTOBJECT:
		{
			WndProcessInsertObject();
			bResult = true;
		}break;

	case WM_USER_REMOVEOBJECT:
		{
			WndRemoveObject();
			bResult = true;
		}break;

	case WM_USER_SELECTOBJECT:
		{
			WndSelectObject();
			bResult = true;
		}break;

	case WM_USER_RENAMEOBJECT:
		{
			WndRenameObject();
			bResult = true;
		}break;

	case WM_USER_MOVEOBJECT:
		{
			WndMoveObject();
			bResult = true;
		}break;

	case WM_USER_REARRANGE:
		{
			WndReArrange();
			bResult = true;
		}break;

	default:
		break;
	};
	return bResult;
}

//------------------------------------------------------------------------
long Win32ObjectBrowserWidget::HandleNotify(HWND hWndDlg, int nIDCtrl, LPNMHDR pNMHDR)
{
	if (pNMHDR->hwndFrom == m_hwndTree)
	{
		if (pNMHDR->code == NM_CUSTOMDRAW)
		{
			LPNMTVCUSTOMDRAW pNMTVCD = (LPNMTVCUSTOMDRAW) pNMHDR;
			HWND hWndTreeView = pNMHDR->hwndFrom;

			return HandleCustomDraw(hWndTreeView, pNMTVCD);
		}
	}
	return 0;
}

//------------------------------------------------------------------------
long Win32ObjectBrowserWidget::HandleCustomDraw(HWND hWndTreeView, LPNMTVCUSTOMDRAW pNMTVCD)
{
	if (pNMTVCD == NULL){
		return -1;
	}
	switch (pNMTVCD->nmcd.dwDrawStage)
	{ 
	case CDDS_PREPAINT:
		return (CDRF_NOTIFYPOSTPAINT | CDRF_NOTIFYITEMDRAW);

	case CDDS_ITEMPREPAINT:
		{
			if (pNMTVCD->nmcd.uItemState & CDIS_SELECTED){ // selected
				pNMTVCD->clrTextBk = RGB(200, 200, 200);
			}
			return (CDRF_NOTIFYPOSTPAINT | CDRF_NEWFONT);
		}
	case CDDS_ITEMPOSTPAINT:
		{
			return CDRF_DODEFAULT;
		}
		break;
	default:
		break;
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
BOOL Win32ObjectBrowserWidget::SubClassProcTree(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	Win32ObjectBrowserWidget * pBrowser = reinterpret_cast<Win32ObjectBrowserWidget*>(GetWindowLong(hWnd, GWL_USERDATA));

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
			pBrowser->MouseMoveTreeObject(lParam);
		}break;

	case WM_LBUTTONUP:
		{
			pBrowser->EndDragTreeObject();
		}break;

	case WM_NOTIFY:
		{
			if (((LPNMHDR)lParam)->code == TVN_KEYDOWN)
			{
				LPNMTVKEYDOWN ptvkd = (LPNMTVKEYDOWN) lParam;
				switch ((DWORD)ptvkd->wVKey) 
				{
					// move Up/Down
				//case 'U': { MoveObjectTree(TreeView_GetSelection(::GetDlgItem(hwndDialogBrowser, IDC_TREE1)), true); } break;
				//case 'J': { MoveObjectTree(TreeView_GetSelection(::GetDlgItem(hwndDialogBrowser, IDC_TREE1)), false); } break;

				case VK_DELETE:{
						//DeleteTreeObject((HTREEITEM)::SendDlgItemMessage(hwndDialogBrowser, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CARET, 0));
					}break;

				case VK_F2:{
						pBrowser->RenameTreeObject();
					}break;
				}break;
			}
		}break;
	}

	// Call the original window procedure for default processing. 
	return CallWindowProc(pBrowser->m_lpfnTreeProc, hWnd, messg, wParam, lParam);
}

//------------------------------------------------------------------------
bool Win32ObjectBrowserWidget::Copy(const HTREEITEM hTreeItem)
{
	bool bResult = false;

	CActor * ActorCopy = GetActorByData(hTreeItem);

	if (ActorCopy) // do not allow rename root node
	{
		CActor * RootNode = ActorCopy->GetRootNode();
		if (RootNode)
		{
			RootNode->Copy(ActorCopy);
		}
	}
	return bResult;
}

//------------------------------------------------------------------------
bool Win32ObjectBrowserWidget::Paste(const HTREEITEM hTreeItem)
{
	bool bResult = false;

	CActor * ActorPaste = GetActorByData(hTreeItem);

	if (ActorPaste) // do not allow rename root node
	{
		CActor * RootNode = ActorPaste->GetRootNode();
		if (RootNode)
		{
			RootNode->Paste(ActorPaste);
		}
	}
	return bResult;
}

//------------------------------------------------------------------------
bool Win32ObjectBrowserWidget::BeginDragTreeObject(const LPNMTREEVIEW lpnmtv)
{
	bool bResult = false;

	if (lpnmtv != NULL)
	{
		HIMAGELIST hImg;
		m_bDragging = true; // set global flag

		hImg = TreeView_CreateDragImage(m_hwndTree, lpnmtv->itemNew.hItem);

		ImageList_BeginDrag(hImg, 0, 0, 0);

		ImageList_DragEnter(m_hwndTree, lpnmtv->ptDrag.x, lpnmtv->ptDrag.y);

		SetCapture(m_hwndTree);
		SetCursor(m_hCursHand);

		m_ActorPicked = GetActorByData((HTREEITEM)lpnmtv->itemNew.hItem);
	}
	return bResult;
}

//------------------------------------------------------------------------
bool Win32ObjectBrowserWidget::MouseMoveTreeObject(const LPARAM lParam)
{
	bool bResult = false;

	if (m_bDragging && lParam)
	{
		TVHITTESTINFO tvht;

		POINTS Pos = MAKEPOINTS(lParam);

		ImageList_DragMove(Pos.x + 16, Pos.y); // where to draw the drag from
		ImageList_DragShowNolock(FALSE);

		tvht.pt.x = Pos.x; // the highlight items should be as the same points as the drag
		tvht.pt.y = Pos.y;

		m_hitTarget = (HTREEITEM)SendMessage(m_hwndTree, TVM_HITTEST, NULL, (LPARAM)&tvht);
		if (m_hitTarget) // if there is a hit
		{
			SendMessage(m_hwndTree, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)m_hitTarget);   // highlight it
		}
		ImageList_DragShowNolock(TRUE);
	}
	return bResult;
}

//------------------------------------------------------------------------
bool Win32ObjectBrowserWidget::EndDragTreeObject()
{
	CCoreSDK *pCoreSDK = m_pWindowManager->GetCoreSDK();

	SHORT CtrlPressed = GetAsyncKeyState(VK_LCONTROL);

	if (m_bDragging) 
	{	
		HTREEITEM hTreeItem = TreeView_GetNextItem(m_hwndTree, 0, TVGN_DROPHILITE);
		
		ImageList_DragLeave(m_hwndTree);
		ImageList_EndDrag();

		HTREEITEM Selected = TreeView_GetNextItem(m_hwndTree, 0, TVGN_DROPHILITE);
		TreeView_Select(m_hwndTree, (LPARAM)Selected, TVGN_CARET);
		TreeView_Select(m_hwndTree, NULL, TVGN_DROPHILITE);

		ReleaseCapture();
		SetCursor(m_hCursArrow);

		CActor * HitActor = m_ActorDrop = GetActorByData(hTreeItem);

		if (m_ActorDrop && CtrlPressed){ // Plain move means get target actor parent
			m_ActorDrop = m_ActorDrop->GetParent();
		}

		if (m_ActorDrop && m_ActorPicked && (m_ActorDrop != m_ActorPicked) && !m_ActorPicked->IsTransient())
		{
			CActor * TempActor = m_ActorDrop->GetParent(); // Check ActorPicked not parent of ActorDrop
			while (TempActor)
			{
				if (TempActor == m_ActorPicked) { // unable put parent object to child directly
					return false;
				}
				TempActor = TempActor->GetParent();
			}

			Matrix LTM;
			//if (CActor::GetObjectAInLocalSpaceB(LTM, m_ActorDrop, m_ActorPicked))
			//{
			//	// remove actor B from prev node and push to selected
			//	m_ActorPicked->GetParent()->RemoveChildNode(m_ActorPicked);
			//	m_ActorPicked->SetParent(m_ActorDrop);
			//	m_ActorPicked->SetPivot(LTM);
			//	m_ActorDrop->AddChildNode(m_ActorPicked);

			//	InvokeActor(m_ActorPicked);

			//	pCoreSDK->GetViewportManager()->RebuildTransform(m_ActorDrop);

			//	if (CtrlPressed){
			//		MoveObjectNearTo(m_ActorPicked, HitActor);
			//	}

			//	TTreeMapActorIterator Iter = m_TreeMap.find(m_ActorPicked);
			//	if (Iter != m_TreeMap.end()){
			//		TreeView_Select(m_hwndTree, Iter->second, TVGN_CARET);
			//	}
			//}
		}
		m_ActorDrop = m_ActorPicked = NULL;
		m_bDragging = false; // drop global flag
	}
	return true;
}

//------------------------------------------------------------------------
bool Win32ObjectBrowserWidget::MoveObjectNearTo(CActor * Picked, const CActor * Target, bool Up)
{
	const Registry * Reg = m_pWindowManager->GetCoreSDK()->GetRegistry();
	assert(Reg);

	if (Picked && Target && 
		Picked != Target && 
		Picked->GetParent() == Target->GetParent()) // check objects are in same plane
	{
		CActor * ParentObject = Picked->GetParent();

		if (ParentObject != NULL)
		{
			TVecActorChildIterator IterActorMove   = std::find(ParentObject->ChildNodes.begin(), ParentObject->ChildNodes.end(), Picked);
			TVecActorChildIterator IterActorMoveTo = std::find(ParentObject->ChildNodes.begin(), ParentObject->ChildNodes.end(), Target);

			if (IterActorMove != ParentObject->ChildNodes.end() && 
				IterActorMoveTo != ParentObject->ChildNodes.end())
			{
				TVecActorChild::difference_type n = IterActorMoveTo - IterActorMove;

				int CountNum = abs(int(n));
				while (CountNum > 0)
				{
					ActorAllocator::MoveObjectHierarchy(Picked, Reg, (n < 0));
					CountNum--;
				}
			}
		}
	}
	return true;
}

//----------------------------------------------------------------------------------------------
void Win32ObjectBrowserWidget::Redraw() const
{
	InvalidateRect(m_hwndTree, NULL, TRUE);
}

//----------------------------------------------------------------------------------------------
void Win32ObjectBrowserWidget::ProcessRightClick()
{
	POINT pt;
	if (::GetCursorPos(&pt)) // address of structure for cursor position
	{
		TVHITTESTINFO tvht;
		tvht.pt = pt;
		ScreenToClient(m_hwndTree, &tvht.pt);

		m_hitTarget = (HTREEITEM)SendMessage(m_hwndTree, TVM_HITTEST, NULL, (LPARAM)&tvht);
		if (m_hitTarget != NULL) // if there is a hit
		{
			m_pfnContextMenu(m_hwndTree, pt, GetActorByData(m_hitTarget));
		}
	}
}*/