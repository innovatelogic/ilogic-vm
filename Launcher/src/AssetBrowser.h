#ifndef __assetbrowser_h__
#define __assetbrowser_h__

#pragma once

#include "IAssetBrowserFrame.h"
#include "ToolbarControl.h"
#include "scene_editor.h"

//template<class T_CLASS> class CVoid {};
//template<class T_CLASS> struct CVoid_ {}; 
//template< template<typename> class T_CLASS = CVoid_ >

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template<class T_CLASS>
class CAssetBrowserFrame : public CFrameWindowImpl<CAssetBrowserFrame<T_CLASS> >, public IAssetBrowserFrame
{
	typedef typename void (*pContextMenuFunction)(HWND hwnd, POINT pt, T_CLASS *pActor);
	typedef typename bool (*pContextMenuProcessor)(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	typedef typename int  (*pGetResourceIconIndex)(const std::string &TypeName);
	typedef typename int  (*CALLBACK_FN) (const T_CLASS *pSender);
	typedef typename void (*CALLBACK_EV)(const T_CLASS *pSender, ESystemEventID EventId);
	
	typedef TTreeMap<SNode, SNode> TTreeMapNodes;
	typedef CPaneTreeView<T_CLASS, SNode> TPaneTreeView;
	typedef CPaneListView<T_CLASS, SNode> TPaneListView;

public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MENU_ASSETBROWSER)

	CHorSplitterWindow	m_hzSplitTop;
	CSplitterWindow		m_vSplitLeft;
	CSplitterWindow		m_vSplitRight;
	CHorSplitterWindow	m_hzSplitBottom;
	CHorSplitterWindow	m_hzSplitSubRight;
	CHorSplitterWindow	m_hzSplitBottomRight;

	TPaneListView		*m_pPaneListView;
	CPaneContainer		m_RightTopPane;
	CPaneContainer		m_RightBottomPane;

	CToolbarControl		*m_pToolbarControl;

	// controls
	CPanePropertyContainer<T_CLASS>	*m_pPropertyGridPane;
	CTreePaneContainer<T_CLASS> *m_pRightBottomPane;

	TPaneTreeView		*m_pPaneTreeView;
	
	class CViewAssetContainer *m_pViewCtrl;

	SRenderContext		*m_pRenderContext;
	class CCoreSDK		*m_pAppMain;

    editors::TIEditor    m_editor;

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		COMMAND_ID_HANDLER(ID_ASSET_IMPORT, OnAssetImport)
		COMMAND_ID_HANDLER(ID_ASSET_SAVE, OnAssetSave)
		COMMAND_ID_HANDLER(ID_ASSET_SAVE_AS, OnAssetSaveAs)
		COMMAND_ID_HANDLER(ID_ASSET_CLOSE, OnAssetClose)
		COMMAND_ID_HANDLER(ID_BUTTON_AB_IMPORT, OnAssetImport);
		COMMAND_ID_HANDLER(ID_BUTTON_AB_SAVE, OnAssetSave);
		COMMAND_ID_HANDLER(ID_BUTTON_AB_UNDO, OnButtonUndo);
		COMMAND_ID_HANDLER(ID_BUTTON_AB_REDO, OnButtonRedo);
		COMMAND_ID_HANDLER(ID_BUTTON_AB_MOVE, OnModeMove);
		COMMAND_ID_HANDLER(ID_BUTTON_AB_ROTATE, OnModeRotate);
		COMMAND_ID_HANDLER(ID_BUTTON_AB_SCALE, OnModeScale);
		CHAIN_MSG_MAP(CFrameWindowImpl<CAssetBrowserFrame<T_CLASS> >)
        REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	//----------------------------------------------------------------------------------------------
	CAssetBrowserFrame(CCoreSDK *api,
        pContextMenuFunction pfMenu,
		pContextMenuProcessor pfnMenuProcessor,
		pGetResourceIconIndex pfnGetResourceIconIndex,
		CALLBACK_FN pfnInvokeObject,
		CALLBACK_FN pfnDirectInvokeObject,
		CALLBACK_FN pfnClearObject,
		CALLBACK_FN pfnDirectClearObject,
		CALLBACK_EV pfnOnEventUpdate,
		HIMAGELIST hImageList)
		: CFrameWindowImpl<CAssetBrowserFrame<T_CLASS> >()
        , m_pAppMain(api)
		, m_hImageList(hImageList)
	{
        m_editor = editors::EditorScene3D::CreateEdtior("scene_viewer",
            m_pAppMain,
            m_pAppMain->GetExplorerInstance(),
            editors::EEditorType::EEditorDefault);

		m_pViewCtrl = new CViewAssetContainer(m_editor);

		m_pPaneTreeView = new TPaneTreeView(this);

		m_pPaneListView = new TPaneListView(this, m_editor);

		m_pRightBottomPane = new CTreePaneContainer<T_CLASS>(m_editor,
			pfMenu, 
			pfnMenuProcessor,
			pfnGetResourceIconIndex,
			pfnInvokeObject,
			pfnDirectInvokeObject,
			pfnClearObject,
			pfnDirectClearObject,
			pfnOnEventUpdate,
			hImageList);

		m_pPropertyGridPane = new CPanePropertyContainer<T_CLASS>(m_editor);

		std::wstring new_path = BuildFolderPath(L"data\\$cashe");

		BuildContentTree(new_path.c_str());
	}

	//----------------------------------------------------------------------------------------------
	virtual ~CAssetBrowserFrame()
	{
		delete m_pViewCtrl;

		delete m_pPaneTreeView;

		delete m_pPaneListView;

		delete m_pRightBottomPane;

		delete m_pPropertyGridPane;

		delete m_pToolbarControl;
	}

	virtual CCoreSDK* GetAppMain() const { return m_pAppMain; }

	//----------------------------------------------------------------------------------------------
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&)
	{
		CreateSimpleToolBar();

		// set flat toolbar style
		CToolBarCtrl tool = m_hWndToolBar;
		tool.SetStyle(tool.GetStyle() | TBSTYLE_FLAT);

		m_pToolbarControl = new CToolbarControl(m_hWndToolBar, 50, 40, 6);

		// attach the hicolor bitmaps to the toolbar
		m_pToolbarControl->AttachToolbarImages(IDB_BITMAP_TOOLBAR_ASSET, IDB_BITMAP_TOOLBAR_ASSET, IDB_BITMAP_TOOLBAR_ASSET);

		CreateSimpleStatusBar();

		m_hWndClient = CreateClient();

		m_pPaneTreeView->FillTree(m_TreeMapNodes);

		m_pRightBottomPane->SetRenderContext(m_editor->GetRenderContext());
		m_pPropertyGridPane->SetRenderContext(m_editor->GetRenderContext());

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	HWND CreateClient()
	{
		CreatePaneGrid();

		if (m_pViewCtrl->Create(m_RightTopPane.m_hWnd) == NULL)
		{
			ATLTRACE(_T("View window creation failed!\n"));
			return 0;
		}

		// assign the edit to the bottom container
		m_RightTopPane.SetClient(m_pViewCtrl->m_hWnd);

		return m_hzSplitTop.m_hWnd;
	}

	//----------------------------------------------------------------------------------------------
	void CreatePaneGrid()
	{
		CRect rc;
		GetClientRect(&rc);

		// create the horizontal top splitter
		m_hzSplitTop.Create(m_hWnd, rc, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_hzSplitTop.m_cxyMin = 35; // minimum size
		m_hzSplitTop.SetSplitterPos(50); // from top
		m_hzSplitTop.m_bFullDrag = false; // ghost bar enabled

		// create the vertical splitter
		m_vSplitLeft.Create(m_hzSplitTop.m_hWnd, rc, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_vSplitLeft.m_cxyMin = 35; // minimum size
		m_vSplitLeft.SetSplitterPos(125); // from left
		m_vSplitLeft.m_bFullDrag = false; // ghost bar enabled

		m_hzSplitTop.SetSplitterPane(1, m_vSplitLeft);

		// create the vertical splitter
		m_vSplitRight.Create(m_vSplitLeft.m_hWnd, rc, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_vSplitRight.m_cxyMin = 35; // minimum size
		m_vSplitRight.SetSplitterPos(725); // from left
		m_vSplitRight.m_bFullDrag = false; // ghost bar enabled

		m_vSplitLeft.SetSplitterPane(1, m_vSplitRight);

		// create horizontal right sub split
		m_hzSplitSubRight.Create(m_vSplitRight.m_hWnd, rc, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_hzSplitSubRight.m_cxyMin = 35; // minimum size
		m_hzSplitSubRight.SetSplitterPos(225); // from left
		m_hzSplitSubRight.m_bFullDrag = false; // ghost bar enabled

		m_vSplitRight.SetSplitterPane(1, m_hzSplitSubRight);

		// split right bottom
		m_hzSplitBottomRight.Create(m_hzSplitSubRight.m_hWnd, rc, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_hzSplitBottomRight.m_cxyMin = 35; // minimum size
		m_hzSplitBottomRight.SetSplitterPos(225); // from left
		m_hzSplitBottomRight.m_bFullDrag = false; // ghost bar enabled

		m_hzSplitSubRight.SetSplitterPane(1, m_hzSplitBottomRight);

		// create center pane
		m_pPaneListView->Create(m_vSplitRight.m_hWnd);
		m_vSplitRight.SetSplitterPane(0, *m_pPaneListView);
		m_pPaneListView->SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
		m_pPaneListView->SetTitle(L"Folder");

		m_RightTopPane.Create(m_hzSplitSubRight.m_hWnd);
		m_hzSplitSubRight.SetSplitterPane(0, m_RightTopPane);
		m_RightTopPane.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
		m_RightTopPane.SetTitle(L"Preview");

		// create left pane
		m_pPaneTreeView->Create(m_vSplitLeft.m_hWnd);
		m_vSplitLeft.SetSplitterPane(0, *m_pPaneTreeView);
		m_pPaneTreeView->SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
		m_pPaneTreeView->SetTitle(L"Browse");

		// create right bottom pane
		m_pPropertyGridPane->Create(m_hzSplitBottomRight.m_hWnd);
		m_hzSplitBottomRight.SetSplitterPane(0, *m_pPropertyGridPane);
		m_pPropertyGridPane->SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
		m_pPropertyGridPane->SetTitle(L"Properties");

		// create right bottom pane
		m_pRightBottomPane->Create(m_hzSplitBottomRight.m_hWnd);
		m_hzSplitBottomRight.SetSplitterPane(1, *m_pRightBottomPane);
		m_pRightBottomPane->SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
		m_pRightBottomPane->SetTitle(L"Browse");
	}

	//----------------------------------------------------------------------------------------------
	void InitViewport()
	{
		m_pViewCtrl->InitViewport();
	}

	//----------------------------------------------------------------------------------------------
	void Render()
	{
		m_pViewCtrl->Render(); // Render & change buffer
	}

	//----------------------------------------------------------------------------------------------
	virtual void OnFolderSelected(SNode *pNodeParent)
	{
		assert(pNodeParent);

		if (TNodeMap<SNode, SNode> *pNode = m_TreeMapNodes.m_pFirstElement)
		{
			do
			{
				TNodeMap<SNode, SNode> *pRoot = pNode->GetRootNode();
				const SNode *pParentItem = pRoot ? pRoot->m_pValue : NULL;
				const SNode *pNodeItem = pNode->m_pValue;

				if (pNodeParent == pParentItem){
					m_pPaneListView->FillList(pNode);
					break;
				}

				pNode = m_TreeMapNodes.GetNext(pNode);
			} while (pNode);
		}
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnAssetImport(WORD, WORD, HWND, BOOL&)
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnAssetSave(WORD, WORD, HWND, BOOL&)
	{
		if (!m_strFilename.empty())
		{
			TCHAR strExePath[MAX_PATH];

			// Get the exe name, and exe path
			GetModuleFileName(NULL, strExePath, MAX_PATH);

			std::wstring strPath(strExePath);
			unsigned pos = strPath.find_last_of(L"/\\");

			if (pos == std::wstring::npos){
				return 0;
			}

			strPath = strPath.substr(0, pos) + std::wstring(L"//") + m_strFilename;

            if (!m_editor->Save(strPath))
            {
                // TODO: Error
            }
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnAssetSaveAs(WORD, WORD, HWND, BOOL&)
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnAssetClose(WORD, WORD, HWND, BOOL&)
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnButtonUndo(WORD, WORD, HWND, BOOL&)
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnButtonRedo(WORD, WORD, HWND, BOOL&)
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnModeMove(WORD, WORD, HWND, BOOL&)
	{
		if (m_pAppMain->GetEditControlMode() != ECM_Move)
		{
			m_pAppMain->SetEditControlMode(ECM_Move);
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnModeRotate(WORD, WORD, HWND, BOOL&)
	{
		if (m_pAppMain->GetEditControlMode() != ECM_Rotate)
		{
			m_pAppMain->SetEditControlMode(ECM_Rotate);
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnModeScale(WORD, WORD, HWND, BOOL&)
	{
		if (m_pAppMain->GetEditControlMode() != ECM_Scale)
		{
			m_pAppMain->SetEditControlMode(ECM_Scale);
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	void Update(const T_CLASS *pSender, ESystemEventID EventId)
	{
        //const T_CLASS *pModelViewer = m_editor->G

        //if (pModelViewer == pSender || CActor::IsChildOf(pModelViewer, pSender))
        //{

		m_pRightBottomPane->Update(pSender, EventId);
		m_pPropertyGridPane->Update(pSender, EventId);
	}

	//----------------------------------------------------------------------------------------------
	// TODO: move to fill strategy
	void BuildContentTree(const wchar_t *in_path)
	{
#ifdef WIN32
		HANDLE hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATA ffd;

		std::stack<SNode*>			nodes;
		std::stack<std::wstring>	directories;

		directories.push(in_path);		// add root
		nodes.push(new SNode(in_path, FILE_ATTR_DIRECTORY)); // node

		m_TreeMapNodes.PushBack(nodes.top(), nodes.top(), NULL); // fill root to tree

		while (!directories.empty())
		{
			SNode *pTop = nodes.top();

			std::wstring path = directories.top();
			std::wstring spath = path + L"//" + L"*";

			directories.pop();
			nodes.pop();

			hFind = FindFirstFile(spath.c_str(), &ffd);
			if (hFind == INVALID_HANDLE_VALUE){
				return;
			}

			do 
			{
				std::wstring buffer = path + L"\\" + ffd.cFileName;

				if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L"..") != 0) // to find msdn
				{
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						directories.push(buffer);

						SNode *pTmp = new SNode(buffer, FILE_ATTR_DIRECTORY);

						m_TreeMapNodes.PushBack(pTmp, pTmp, pTop);

						nodes.push(pTmp);
					}
					else
					{
						SNode *pTmp = new SNode(buffer);
						m_TreeMapNodes.PushBack(pTmp, pTmp, pTop);
					}
				}
			} while (FindNextFile(hFind, &ffd) != 0);

			if (GetLastError() != ERROR_NO_MORE_FILES)
			{
				FindClose(hFind);
				return;
			}

			FindClose(hFind);
			hFind = INVALID_HANDLE_VALUE;
		}
#else
#endif
	}

	//----------------------------------------------------------------------------------------------
	virtual void SetOpenFileName(const wchar_t *strName)
	{
		m_strFilename = strName;

		SetWindowText(strName);
	}

	//----------------------------------------------------------------------------------------------
	virtual HIMAGELIST GetImageList() const
	{
		return m_hImageList;
	}

private:
	TTreeMapNodes	m_TreeMapNodes;
	std::wstring	m_strFilename;

	HIMAGELIST		m_hImageList;
};

#endif//__assetbrowser_h__