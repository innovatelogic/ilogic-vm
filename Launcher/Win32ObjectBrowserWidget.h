#pragma once

#include "scene_editor_main.h"
#include "MSelectTree.h"

#define WM_USER_INSERTOBJECT				WM_APP+1
#define WM_USER_REMOVEOBJECT_BRWSR			WM_APP+2
#define WM_USER_SELECTOBJECT_BRWSR			WM_APP+3
#define WM_USER_RENAMEOBJECT_BRWSR			WM_APP+4
#define WM_USER_MOVEOBJECT					WM_APP+5
#define WM_USER_REARRANGE					WM_APP+6

class Registry;

template<class T_CLASS>
class Win32ObjectBrowserWidget
{
	typedef typename void (*pContextMenuFunction) (HWND hwnd, POINT pt, T_CLASS *pActor);
	typedef typename bool (*pContextMenuProcessor)(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	typedef typename int  (*pGetResourceIconIndex)(const std::string &TypeName);

public:
	typedef typename std::map<const T_CLASS*, HTREEITEM>  TTreeMapActor;
	typedef typename TTreeMapActor::iterator			  TTreeMapActorIterator;
	typedef typename TTreeMapActor::const_iterator		  TTreeMapActorConstIterator;

	typedef int (*CALLBACK_FN) (const T_CLASS *pSender);

	struct SwapHTree
	{
		const T_CLASS *ActorMove;
		bool bUp;
	};

public:

	HWND GetHWndTree() const { return m_hwndTree; }

    Win32ObjectBrowserWidget(HWND hWndParent,
        pContextMenuFunction pfMenu,
        pContextMenuProcessor pfnMenuProcessor,
        pGetResourceIconIndex pfnGetResourceIconIndex,
        CALLBACK_FN pfnInvokeObject,
        CALLBACK_FN pfnDirectInvokeObject,
        CALLBACK_FN pfnClearObject,
        CALLBACK_FN pfnDirectClearObject,
        HIMAGELIST hImageList,
        SRenderContext *pRenderContext = 0);

    ~Win32ObjectBrowserWidget();

    void SetEditor(editors::TIEditor editor);

	void SetRenderContext(SRenderContext *pContext) { m_pRenderContext = pContext; }

    void InvokeActor(const T_CLASS *pSender);

    void Update(const T_CLASS *pSender, ESystemEventID EventId);

    bool ClearActor(const T_CLASS *pSender);

    int SelectActor(const T_CLASS * Sender);

    int RenameActor(const T_CLASS * Sender);

    int MoveActor(const T_CLASS * Sender, bool bUp);

    int RearrangeActor(const T_CLASS * Sender);

    void WndProcessInsertObject();

    void WndRemoveObject();

    void WndSelectObject();

    void WndRenameObject();

    void WndMoveObject();

    void WndReArrange();

    int DirectInvokeActor(const T_CLASS *sender);

    int	DirectClearActor(const T_CLASS *sender);

    T_CLASS* GetActorByData(const HTREEITEM lpnmtv) const;

    void SelChangedTreeObject();

    /** object rename operation*/
    bool RenameTreeObject();
 
    bool Copy(const HTREEITEM hTreeItem);
    bool Paste(const HTREEITEM hTreeItem);

    bool BeginDragTreeObject(const LPNMTREEVIEW lpnmtv);
    bool MouseMoveTreeObject(const LPARAM lParam);
    bool EndDragTreeObject();

    bool MoveObjectNearTo(T_CLASS * Picked, const T_CLASS * Target, bool Up /*= false*/);
    void Redraw() const;
    void ProcessRightClick();
    bool ContextMenuProcessor(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

    // notification
    void OnNotifySelected();

    void MoveWindow(int x, int y, int width, int height);

    void SetRoot(T_CLASS  *root) { m_pRoot = root;  }

    bool IsChildOfRoot(const T_CLASS  *root);

    // model management
    void ClearModel();
    void FillModel();

    void LockModel(bool lock) { m_bLockModel = lock;  }

protected:
    void UnselectTreeAll();

    /*!
     * Compare two models (editor vs widget) and returns arrays of difference
     * input arrays would be clear.
     * returns empty array if no difference
     * new_selected [out] - array of newly selected
     * new_deselected [out] - array of newly deselected
     */
    void GetSelectionModelsDiff(std::vector<const T_CLASS*> &new_selected,
                                std::vector<const T_CLASS*> &new_deselected) const;

//----------------------------------------------------------------------------------------------
private:

    const T_CLASS           *m_pRoot;
	Registry			    *m_pRegistry;

	TVecActorChild			m_ActorAddList;
	std::vector<HTREEITEM>	m_HTreeClearList;

	HTREEITEM				m_HTreeSelected;
	HTREEITEM				m_HTreeRename;
	HTREEITEM				m_HTreeMove;
	HTREEITEM				m_HTreeMoveTo;

	TTreeMapActor			m_TreeMap;
	HTREEITEM				m_hitTarget;
	T_CLASS*				m_ActorDrop;
	T_CLASS*				m_ActorPicked;
	std::vector<T_CLASS*>	m_ActorsRearrange;

	std::vector<SwapHTree>  m_TMapSwapTree;

	HWND m_hwndParent;
	HWND m_hwndTree;

    CMultiSelectTreeCtrl m_hwndLeft;

	CriticalSection m_CS;

	pContextMenuFunction	m_pfnContextMenu;
	pContextMenuProcessor	m_pfnContextMenuProcessor;
	pGetResourceIconIndex	m_pfnGetResourceIconIndex;

	bool m_bDragging;
	HCURSOR	m_hCursHand, m_hCursArrow;     // cursor handles

	SRenderContext			*m_pRenderContext;

    std::shared_ptr<editors::IEditor> m_editor;

    bool m_bLockModel;

public:
	CALLBACK_FN				m_pfnInvokeObject;
	CALLBACK_FN				m_pfnDirectInvokeObject;
	CALLBACK_FN				m_pfnClearObject;
	CALLBACK_FN				m_pfnDirectClearObject;

private:
    bool m_bLockUpdate;
};

#include "Win32ObjectBrowserWidget.ipp"