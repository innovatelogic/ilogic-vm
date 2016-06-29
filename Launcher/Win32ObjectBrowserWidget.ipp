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
    , bLockUpdate(false)
	{
		RECT rect;
		GetClientRect(hWndParent, &rect);

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

//----------------------------------------------------------------------------------------------
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

    if (!bLockUpdate)
    {
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
    }

    return bResult;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
bool Win32ObjectBrowserWidget<T_CLASS>::RenameTreeObject()
{
    bool bResult = false;

    HTREEITEM hTreeItem = TreeView_GetSelection(m_hwndTree);

    T_CLASS* Actor = GetActorByData(hTreeItem);

    if (Actor) // do not allow rename root node
    {
        wchar_t wbuf[100] = { 0 };
        char ascii[100] = { 0 };

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
template<class T_CLASS>
bool Win32ObjectBrowserWidget<T_CLASS>::Copy(const HTREEITEM hTreeItem)
{
    bool bResult = false;

    T_CLASS * ActorCopy = GetActorByData(hTreeItem);

    if (ActorCopy) // do not allow rename root node
    {
        T_CLASS * RootNode = ActorCopy->GetRootNode();
        if (RootNode)
        {
            RootNode->Copy(ActorCopy);
        }
    }
    return bResult;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
bool Win32ObjectBrowserWidget<T_CLASS>::Paste(const HTREEITEM hTreeItem)
{
    bool bResult = false;

    T_CLASS * ActorPaste = GetActorByData(hTreeItem);

    if (ActorPaste) // do not allow rename root node
    {
        T_CLASS * RootNode = ActorPaste->GetRootNode();
        if (RootNode)
        {
            RootNode->Paste(ActorPaste);
        }
    }
    return bResult;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
bool Win32ObjectBrowserWidget<T_CLASS>::BeginDragTreeObject(const LPNMTREEVIEW lpnmtv)
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

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
bool Win32ObjectBrowserWidget<T_CLASS>::MouseMoveTreeObject(const LPARAM lParam)
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

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
bool Win32ObjectBrowserWidget<T_CLASS>::EndDragTreeObject()
{
    CCoreSDK *pCoreSDK = m_editor->GetApp();

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

        T_CLASS *HitActor = m_ActorDrop = GetActorByData(hTreeItem);

        if (m_ActorDrop && CtrlPressed) { // Plain move means get target actor parent
            m_ActorDrop = m_ActorDrop->GetParent();
        }

        if (m_ActorDrop && m_ActorPicked && (m_ActorDrop != m_ActorPicked) && !m_ActorPicked->IsTransient())
        {
            T_CLASS * TempActor = m_ActorDrop->GetParent(); // Check ActorPicked not parent of ActorDrop
            while (TempActor)
            {
                if (TempActor == m_ActorPicked) { // unable put parent object to child directly
                    return false;
                }
                TempActor = TempActor->GetParent();
            }

            Matrix LTM;
            /*if (T_CLASS::GetObjectAInLocalSpaceB(LTM, m_ActorDrop, m_ActorPicked))
            {
            // remove actor B from prev node and push to selected
            m_ActorPicked->GetParent()->RemoveChildNode(m_ActorPicked);
            m_ActorPicked->SetParent(m_ActorDrop);
            m_ActorPicked->SetPivot(LTM);
            m_ActorDrop->AddChildNode(m_ActorPicked);

            InvokeActor(m_ActorPicked);

            pCoreSDK->GetViewportManager()->RebuildTransform(m_ActorDrop);

            if (CtrlPressed){
            MoveObjectNearTo(m_ActorPicked, HitActor);
            }

            TTreeMapActorIterator Iter = m_TreeMap.find(m_ActorPicked);
            if (Iter != m_TreeMap.end()){
            TreeView_Select(m_hwndTree, Iter->second, TVGN_CARET);
            }
            }*/
        }
        m_ActorDrop = m_ActorPicked = NULL;
        m_bDragging = false; // drop global flag
    }
    return true;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
bool Win32ObjectBrowserWidget<T_CLASS>::MoveObjectNearTo(T_CLASS * Picked, const T_CLASS * Target, bool Up /*= false*/)
{
    if (Picked && Target &&
        Picked != Target &&
        Picked->GetParent() == Target->GetParent()) // check objects are in same plane
    {
        T_CLASS * ParentObject = Picked->GetParent();

        if (ParentObject != NULL)
        {
            TVecActorChildIterator IterActorMove = std::find(ParentObject->m_ChildNodes.begin(), ParentObject->m_ChildNodes.end(), Picked);
            TVecActorChildIterator IterActorMoveTo = std::find(ParentObject->m_ChildNodes.begin(), ParentObject->m_ChildNodes.end(), Target);

            if (IterActorMove != ParentObject->m_ChildNodes.end() &&
                IterActorMoveTo != ParentObject->m_ChildNodes.end())
            {
                TVecActorChild::difference_type n = IterActorMoveTo - IterActorMove;

                int CountNum = abs(int(n));
                while (CountNum > 0)
                {
                    ActorAllocator::MoveObjectHierarchy(Picked, m_pRegistry, (n < 0));
                    CountNum--;
                }
            }
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void Win32ObjectBrowserWidget<T_CLASS>::Redraw() const
{
    InvalidateRect(m_hwndTree, NULL, TRUE);
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void Win32ObjectBrowserWidget<T_CLASS>::ProcessRightClick()
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
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
bool Win32ObjectBrowserWidget<T_CLASS>::ContextMenuProcessor(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
    SRenderContext *swapContext = 0;
    if (m_pRenderContext) {
        m_editor->GetApp()->GetRenderSDK()->GetRenderDriver()->PushContext(m_pRenderContext);
    }

    bool bResult = m_pfnContextMenuProcessor(hWnd, messg, wParam, lParam);

    if (m_pRenderContext) {
        m_editor->GetApp()->GetRenderSDK()->GetRenderDriver()->PopContext();
    }
    return bResult;
}

//----------------------------------------------------------------------------------------------
template<class T_CLASS>
void Win32ObjectBrowserWidget<T_CLASS>::OnNotifySelected()
{
    bLockUpdate = true;

    const std::vector<CActor*> &selected = m_editor->GetSelected();

    std::vector<CActor*> actorsWidget;

    bool equal = false;

    if (selected.size() == m_hwndLeft.m_aData.GetSize())
    {
        equal = true;

        for (size_t i = 0; i < m_hwndLeft.m_aData.GetSize(); i++)
        {
            if (m_hwndLeft.m_aData.GetValueAt(i).bSelected)
            {
                T_CLASS *actor = GetActorByData(m_hwndLeft.m_aData.GetValueAt(i).hItem);

                if (std::find(selected.begin(), selected.end(), actor) == selected.end())
                {
                    equal = false;
                    break;
                }
            }
        }
    }

    if (!equal)
    {
        // unselect all
        for (size_t i = 0; i < m_hwndLeft.m_aData.GetSize(); i++)
        {
            if (m_hwndLeft.m_aData.GetValueAt(i).bSelected)
            {
                m_hwndLeft.SelectItem(m_hwndLeft.m_aData.GetValueAt(i).hItem, false);
            }
        }

        for each (auto actor in selected)
        {
            bool bFind = false;

            TTreeMapActorConstIterator iter = m_TreeMap.begin();
            while (iter != m_TreeMap.end())
            {
                if (iter->first == actor)
                {
                    m_hwndLeft.SelectItem(iter->second, true);

                    bFind = true;
                    break;
                }
                ++iter;
            }

            assert(bFind);
        }
    }

    bLockUpdate = false;
}