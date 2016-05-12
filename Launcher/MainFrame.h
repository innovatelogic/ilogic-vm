// MainFrm.h : interface of the CMainFrame class
//
// Copyright 2014 by Innovatelogic.
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__BBA5DFCA_6C1A_11D6_B657_0048548B09C5__INCLUDED_)
#define AFX_MAINFRM_H__BBA5DFCA_6C1A_11D6_B657_0048548B09C5__INCLUDED_

#pragma once

#include "AssetBrowser.h"
#include "ToolbarControl.h"
#include "scene_editor_main.h"

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template<class T_CLASS>
class CMainFrame : public CFrameWindowImpl<CMainFrame<T_CLASS> >
{
	typedef typename void (*pContextMenuFunction)(HWND hwnd, POINT pt, T_CLASS *pActor);
	typedef typename bool (*pContextMenuProcessor)(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	typedef typename int  (*pGetResourceIconIndex)(const std::string &TypeName);
	typedef typename int  (*CALLBACK_FN) (const T_CLASS *pSender);
	typedef typename void (*CALLBACK_EV)(const T_CLASS *pSender, ESystemEventID EventId);

public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CHorSplitterWindow		m_hzSplitTop;
	CHorSplitterWindow		m_hzSplitBottom;
	CSplitterWindow			m_vSplitRight;
	CSplitterWindow			m_vSplitLeft;
	CHorSplitterWindow		m_hzSplitSubRight;

	CPaneContainer					m_CentralPane;
	CPanePropertyContainer<T_CLASS>	*m_pRightTopPane;
	CTreePaneContainer<T_CLASS>		*m_pRightBottomPane;
	CPaneContainer					m_BottomPane;

	CViewContainer					m_ViewCtrl;
	CWTLPlacementWidget<T_CLASS>	*m_pPlacementCtrl;

	HWND m_hViewWnd;

	CToolbarControl *m_pToolbarControl;

    editors::SceneEditorMain *g_pSceneEditorMain = nullptr;
	class CCoreSDK *m_pAppMain;

	WNDCLASSEX  m_wcAppSplash;
	WCHAR       m_szAppSplashName[255];
	HWND		m_hWndSplash;

	HBITMAP		m_hSplashBitmap;
	HBITMAP		m_hSplashLogo;

	unsigned int m_SplWidth;
	unsigned int m_SplHeight;

	unsigned int m_LogoWidth;
	unsigned int m_LogoHeight;

	std::string	 m_SplashScreenMessage;

	pContextMenuFunction	m_pfnContextMenu;
	pContextMenuProcessor	m_pfnContextMenuProcessor;
	pGetResourceIconIndex	m_pfnGetResourceIconIndex;

	CALLBACK_FN				m_pfnInvokeObject;
	CALLBACK_FN				m_pfnDirectInvokeObject;
	CALLBACK_FN				m_pfnClearObject;
	CALLBACK_FN				m_pfnDirectClearObject;
	CALLBACK_EV				m_pfnOnEventUpdate;

	HIMAGELIST				m_hImageList;
	CImageList				m_ToolbarImagesDisabled;
	CImageList				m_ToolbarImagesHot;
	CImageList				m_ToolbarImages;

	Registry					*m_pRegistry;
	CAssetBrowserFrame<T_CLASS> *m_pAssetBrowserFrame;

	//----------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------
	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_USER_INSERTOBJECT, OnAppInsertObject)
		COMMAND_ID_HANDLER(ID_PANE_CLOSE, OnPaneClose)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)

		COMMAND_ID_HANDLER(ID_MAINMENU_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER(ID_MAINMENU_FILE_SAVE, OnFileSave)

		COMMAND_ID_HANDLER(ID_OBJECTS_WIREFRAME, OnWireframe)
		COMMAND_ID_HANDLER(ID_OBJECTS_BOUNDS, OnObjectBounds)
		COMMAND_ID_HANDLER(ID_OBJECTS_SPARITALSUB, OnSpatialSub)
		COMMAND_ID_HANDLER(ID_OBJECTS_COLLISION, OnObjectsCollision)
		COMMAND_ID_HANDLER(ID_OBJECTS_GRID, OnShowGrid)
		COMMAND_ID_HANDLER(ID_BUILD_PLAY, OnBuildPlay)

		COMMAND_ID_HANDLER(ID_MODE_MOVE, OnModeMove);
		COMMAND_ID_HANDLER(ID_MODE_ROTATE, OnModeRotate);
		COMMAND_ID_HANDLER(ID_MODE_SCALE, OnModeScale);

		COMMAND_ID_HANDLER(ID_BUTTON_UNDO, OnButtonUndo);
		COMMAND_ID_HANDLER(ID_BUTTON_REDO, OnButtonRedo);
		COMMAND_ID_HANDLER(ID_BUTTON_MOVE, OnModeMove);
		COMMAND_ID_HANDLER(ID_BUTTON_ROTATE, OnModeRotate);
		COMMAND_ID_HANDLER(ID_BUTTON_SCALE, OnModeScale);

		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	//----------------------------------------------------------------------------------------------
	CMainFrame(Registry *pRegistry,
				pContextMenuFunction pfMenu,
				pContextMenuProcessor pfnMenuProcessor,
				pGetResourceIconIndex pfnGetResourceIconIndex,
				CALLBACK_FN pfnInvokeObject,
				CALLBACK_FN pfnDirectInvokeObject,
				CALLBACK_FN pfnClearObject,
				CALLBACK_FN pfnDirectClearObject,
				CALLBACK_EV pfnOnEventUpdate,
				ValueParser &cmd,
				HIMAGELIST hImageList)
		: CFrameWindowImpl<CMainFrame<T_CLASS> >()
		, m_pRegistry(pRegistry)
		, m_pfnContextMenu(pfMenu)
		, m_pfnContextMenuProcessor(pfnMenuProcessor)
		, m_pfnGetResourceIconIndex(pfnGetResourceIconIndex)
		, m_pfnInvokeObject(pfnInvokeObject)
		, m_pfnDirectInvokeObject(pfnDirectInvokeObject)
		, m_pfnClearObject(pfnClearObject)
		, m_pfnDirectClearObject(pfnDirectClearObject)
		, m_pfnOnEventUpdate(pfnOnEventUpdate)
		, m_hImageList(hImageList)
		, m_hWndSplash(0)
		, m_hSplashBitmap(0)
		, m_hSplashLogo(0)
		, m_SplWidth(0)
		, m_SplHeight(0)
		, m_LogoWidth(0)
		, m_LogoHeight(0)
	{
		// start core root process
		m_pAppMain = new CCoreSDK(cmd, 0, m_pfnOnEventUpdate);

        g_pSceneEditorMain = new editors::SceneEditorMain(m_pAppMain, new editors::CommandBuffer);

		m_pPlacementCtrl = new CWTLPlacementWidget<T_CLASS>();

		m_pAssetBrowserFrame = new CAssetBrowserFrame<T_CLASS>(m_pAppMain,
			m_pfnContextMenu,
			m_pfnContextMenuProcessor,
			m_pfnGetResourceIconIndex,
			m_pfnInvokeObject,
			m_pfnDirectInvokeObject,
			m_pfnClearObject,
			m_pfnDirectClearObject,
			m_pfnOnEventUpdate,
			m_hImageList);

		m_pRightBottomPane = new CTreePaneContainer<T_CLASS>(m_pAppMain,
			m_pfnContextMenu,
			m_pfnContextMenuProcessor,
			m_pfnGetResourceIconIndex,
			m_pfnInvokeObject,
			m_pfnDirectInvokeObject,
			m_pfnClearObject,
			m_pfnDirectClearObject,
			m_pfnOnEventUpdate,
			m_hImageList);

		m_pRightTopPane = new CPanePropertyContainer<T_CLASS>(m_pAppMain);

		m_ViewCtrl.m_pApp = m_pAppMain;

		wcscpy(m_szAppSplashName, TEXT("Splash"));

        g_pSceneEditorMain->Initialize();
	}

	//----------------------------------------------------------------------------------------------
	virtual ~CMainFrame()
	{
		delete m_pPlacementCtrl;
		delete m_pRightBottomPane;
		delete m_pAssetBrowserFrame;
		delete m_pRightTopPane;
		delete m_pToolbarControl;
		delete m_pAppMain;
        delete g_pSceneEditorMain;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&)
	{
		CreateSimpleToolBar();

		// set flat toolbar style
		CToolBarCtrl tool = m_hWndToolBar;
		tool.SetStyle(tool.GetStyle() | TBSTYLE_FLAT);

		m_pToolbarControl = new CToolbarControl(m_hWndToolBar, 50, 40, 5);

		// attach the hicolor bitmaps to the toolbar
		m_pToolbarControl->AttachToolbarImages(IDB_BITMAP_TOOLBAR_MAIN, IDB_BITMAP_TOOLBAR_MAIN_DISABLED, IDB_BITMAP_TOOLBAR_MAIN_HOT);

		CreateSimpleStatusBar();

		m_hWndClient = CreateClient();

		InitViewport();

		// create asset browser window
		CRect rc = CRect(0, 0, 1024, 768);
		if (m_pAssetBrowserFrame->CreateEx(m_hWnd, rc, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN) == NULL)
		{
			ATLTRACE(_T("Asset window creation failed!\n"));
			return 0;
		}

		m_pAssetBrowserFrame->ShowWindow(1);
		m_pAssetBrowserFrame->InitViewport();

		m_pfnOnEventUpdate(m_pAppMain->GetExplorerInstance(), ESystemEventID::Event_ObjectGenerated);
		m_pfnOnEventUpdate(m_pAppMain->GetExplorerInstance()->GetExplorer2D(), ESystemEventID::Event_ObjectGenerated);
		m_pfnOnEventUpdate(m_pAppMain->GetExplorerInstance()->GetExplorer3D(), ESystemEventID::Event_ObjectGenerated);

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	HWND CreateClient()
	{
		CreatePaneGrid();

		if (m_ViewCtrl.Create(m_CentralPane.m_hWnd) == NULL)
		{
			ATLTRACE(_T("View window creation failed!\n"));
			return 0;
		}

		m_hViewWnd = m_ViewCtrl.m_hWnd;

		// assign the edit to the bottom container
		m_CentralPane.SetClient(m_hViewWnd);

		// create bottom pane
		if (m_pPlacementCtrl->Create(m_BottomPane.m_hWnd) == NULL)
		{
			ATLTRACE(_T("Placement window creation failed!\n"));
			return 0;
		}
		m_BottomPane.SetClient(m_pPlacementCtrl->m_hWnd);

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

		// create the horizontal splitter. Note that vSplit is parent of hzSplit
		m_hzSplitBottom.Create(m_hzSplitTop.m_hWnd, rc, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_hzSplitBottom.m_cxyMin = 35; // minimum size
		m_hzSplitBottom.SetSplitterPos(1000); // from top
		m_hzSplitBottom.m_bFullDrag = false; // ghost bar enabled

		m_hzSplitTop.SetSplitterPane(1, m_hzSplitBottom); // add bottom to top

		// create the vertical splitter
		m_vSplitLeft.Create(m_hzSplitBottom.m_hWnd, rc, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_vSplitLeft.m_cxyMin = 35; // minimum size
		m_vSplitLeft.SetSplitterPos(25); // from left
		m_vSplitLeft.m_bFullDrag = false; // ghost bar enabled

		m_hzSplitBottom.SetSplitterPane(0, m_vSplitLeft);

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

		// panes
		// add container to right pane (0) of vertical splitter
		m_CentralPane.Create(m_vSplitRight.m_hWnd);
		m_vSplitRight.SetSplitterPane(0, m_CentralPane);
		m_CentralPane.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
		m_CentralPane.SetTitle(L"View");

		// bottom pane
		m_BottomPane.Create(m_hzSplitBottom.m_hWnd);
		m_hzSplitBottom.SetSplitterPane(1, m_BottomPane);
		m_BottomPane.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
		m_BottomPane.SetTitle(L"Placement");

		// create right top pane
		m_pRightTopPane->Create(m_hzSplitSubRight.m_hWnd);
		m_hzSplitSubRight.SetSplitterPane(0, *m_pRightTopPane);
		m_pRightTopPane->SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
		m_pRightTopPane->SetTitle(L"Properties");

		// create right bottom pane
		m_pRightBottomPane->Create(m_hzSplitSubRight.m_hWnd);
		m_hzSplitSubRight.SetSplitterPane(1, *m_pRightBottomPane);
		m_pRightBottomPane->SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
		m_pRightBottomPane->SetTitle(L"Browse");
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnPaneClose(WORD, WORD, HWND hWndCtl, BOOL&)
	{
		// hide the container whose Close button was clicked. Use 
		// DestroyWindow(hWndCtl) instead if you want to totally 
		// remove the container instead of just hiding it
		::ShowWindow(hWndCtl, SW_HIDE);

		// find the container's parent splitter
		HWND hWnd = ::GetParent(hWndCtl);
		CSplitterWindow* pWnd;
		pWnd = (CSplitterWindow*)::GetWindowLong(hWnd, GWL_ID);

		// take the container that was Closed out of the splitter.
		// Use SetSplitterPane(nPane, NULL) if you want to stay in
		// multipane mode instead of changing to single pane mode
		int nCount = pWnd->m_nPanesCount;
		for(int nPane = 0; nPane < nCount; nPane++)
		{
			if (hWndCtl == pWnd->m_hWndPane[nPane])
			{
				pWnd->SetSinglePaneMode(nCount - nPane - 1);
				break;
			}
		}

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnFileExit(WORD, WORD, HWND, BOOL&)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnAppAbout(WORD, WORD, HWND, BOOL&)
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnAppInsertObject(UINT, WPARAM, LPARAM, BOOL&)
	{
		return 0;
	}
	//----------------------------------------------------------------------------------------------
	LRESULT OnFileOpen(WORD, WORD, HWND, BOOL&)
	{
		OPENFILENAME ofn;
		WCHAR szFileName[MAX_PATH] = L"";
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn); 
		ofn.hwndOwner = m_hWnd;
		ofn.lpstrFilter = L"XML Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = L"txt";

		if (GetOpenFileName(&ofn))
		{
            g_pSceneEditorMain->Open(szFileName);
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnFileSave(WORD, WORD, HWND, BOOL&)
	{
		OPENFILENAME ofn;
		WCHAR szFileName[MAX_PATH] = L"";
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn); 
		ofn.hwndOwner = m_hWnd;
		ofn.lpstrFilter = L"XML Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = L"txt";

		if (GetSaveFileName(&ofn))
		{
            g_pSceneEditorMain->Save(szFileName);
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnWireframe(WORD, WORD, HWND, BOOL&)
	{
        g_pSceneEditorMain->SetWireframeMode(!g_pSceneEditorMain->GetWireframeMode());

		UpdateFlagsState();

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnObjectBounds(WORD, WORD, HWND, BOOL&)
	{
		m_pAppMain->SetObjectBoundsVisible(!m_pAppMain->GetObjectBoundsVisible());
		UpdateFlagsState();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnSpatialSub(WORD, WORD, HWND, BOOL&)
	{
		m_pAppMain->SetSparitalSubdivisionVisible(!m_pAppMain->GetSparitalSubdivisionVisible());
		UpdateFlagsState();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnObjectsCollision(WORD, WORD, HWND, BOOL&)
	{
		m_pAppMain->SetCollisionDebugRender(!m_pAppMain->GetCollisionDebugRender());
		UpdateFlagsState();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnShowGrid(WORD, WORD, HWND, BOOL&)
	{
		m_pAppMain->SetShowGrid(!m_pAppMain->GetShowGrid());
		UpdateFlagsState();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnBuildPlay(WORD, WORD, HWND, BOOL&)
	{
		m_pAppMain->SetPause(!m_pAppMain->GetPause());
		UpdateFlagsState();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnButtonUndo(WORD, WORD, HWND, BOOL&)
	{
        g_pSceneEditorMain->Undo();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnButtonRedo(WORD, WORD, HWND, BOOL&)
	{
        g_pSceneEditorMain->Redo();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnModeMove(WORD, WORD, HWND, BOOL&)
	{
		if (m_pAppMain->GetEditControlMode() != ECM_Move)
		{
			m_pAppMain->SetEditControlMode(ECM_Move);
			m_pPlacementCtrl->SetMode(ECM_Move);

			m_pPlacementCtrl->UpdatePreview();

			if (T_CLASS* pSelected = m_pPlacementCtrl->GetSelected()){
				pSelected->BroadcastEvent(Event_ChangeEditCtrMode);
			}
			UpdateFlagsState();
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnModeRotate(WORD, WORD, HWND, BOOL&)
	{
		if (m_pAppMain->GetEditControlMode() != ECM_Rotate)
		{
			m_pAppMain->SetEditControlMode(ECM_Rotate);
			m_pPlacementCtrl->SetMode(ECM_Rotate);

			m_pPlacementCtrl->UpdatePreview();

			if (T_CLASS* pSelected = m_pPlacementCtrl->GetSelected()){
				pSelected->BroadcastEvent(Event_ChangeEditCtrMode);
			}
			UpdateFlagsState();
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnModeScale(WORD, WORD, HWND, BOOL&)
	{
		if (m_pAppMain->GetEditControlMode() != ECM_Scale)
		{
			m_pAppMain->SetEditControlMode(ECM_Scale);
			m_pPlacementCtrl->SetMode(ECM_Scale);

			m_pPlacementCtrl->UpdatePreview();

			if (T_CLASS* pSelected = m_pPlacementCtrl->GetSelected()){
				pSelected->BroadcastEvent(Event_ChangeEditCtrMode);
			}
			UpdateFlagsState();
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	void UpdateFlagsState() const
	{
		CheckMenuItem(GetMenu(), ID_OBJECTS_WIREFRAME, m_pAppMain->GetRenderSDK()->GetWireframeMode() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
//		CheckMenuItem(GetMenu(), ID_WINDOW_BROWSER, CWin32ObjectBrowser::IsVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
		//CheckMenuItem(GetMenu(m_hWnd), ID_WINDOW_TOOLBOX, CWin32ObjectToolbox::IsVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(GetMenu(), ID_WINDOW_THREADMONITOR, CWin32ThreadMonitor::IsVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
//		CheckMenuItem(GetMenu(), ID_WINDOWS_CAMERAMANAGER, CWin32CameraManager::IsVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
//		CheckMenuItem(GetMenu(), ID_WINDOWS_ACTIONPLAY, CWin32ActionPlayEditor::IsVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(GetMenu(), ID_OBJECTS_BOUNDS, m_pAppMain->GetObjectBoundsVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(GetMenu(), ID_OBJECTS_SPARITALSUB, m_pAppMain->GetSparitalSubdivisionVisible() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(GetMenu(), ID_OBJECTS_COLLISION, m_pAppMain->GetCollisionDebugRender() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(GetMenu(), ID_OBJECTS_GRID, m_pAppMain->GetShowGrid() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(GetMenu(), ID_EDITMODE_2D, m_pAppMain->GetEditorInputFlag() & EF_EDIT_2D ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(GetMenu(), ID_EDITMODE_3D, m_pAppMain->GetEditorInputFlag() & EF_EDIT_3D ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(GetMenu(), ID_OBJECTS_FREEZERENDER, m_pAppMain->GetRenderSDK()->GetFreezeRender() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(GetMenu(), ID_OBJECTS_DRAWHELPERS, m_pAppMain->GetRenderSDK()->GetDrawHelpers() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(GetMenu(), ID_BUILD_PLAY, m_pAppMain->GetPause() ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);

		// ECM State
		CheckMenuItem(GetMenu(), ID_MODE_MOVE, m_pAppMain->GetEditControlMode() == ECM_Move ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(GetMenu(), ID_MODE_ROTATE, m_pAppMain->GetEditControlMode() == ECM_Rotate ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(GetMenu(), ID_MODE_SCALE, m_pAppMain->GetEditControlMode() == ECM_Scale ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED);

		CToolBarCtrl	barCtrl = m_hWndToolBar;
		barCtrl.CheckButton(ID_BUTTON_MOVE, m_pAppMain->GetEditControlMode() == ECM_Move ? 1 : 0);
		barCtrl.CheckButton(ID_BUTTON_SCALE, m_pAppMain->GetEditControlMode() == ECM_Scale ? 1 : 0);
		barCtrl.CheckButton(ID_BUTTON_ROTATE, m_pAppMain->GetEditControlMode() == ECM_Rotate ? 1 : 0);
	}

	//----------------------------------------------------------------------------------------------
	void Update(const T_CLASS *pSender, ESystemEventID EventId)
	{
		// filter messages
		const T_CLASS *pExplorer2d = m_pAppMain->GetExplorerInstance()->GetExplorer2D();
		const T_CLASS *pExplorer3d = m_pAppMain->GetExplorerInstance()->GetExplorer3D();

		if (pSender == m_pAppMain->GetExplorerInstance() ||
			CActor::IsChildOf(pExplorer2d, pExplorer3d, pSender))
		{
			m_pRightBottomPane->Update(pSender, EventId);

			m_pRightTopPane->Update(pSender, EventId);
			m_pPlacementCtrl->Update(pSender, EventId);
		}

		m_pAssetBrowserFrame->Update(pSender, EventId);
	}

	//----------------------------------------------------------------------------------------------
	int InitclassexSplash()
	{	
		m_wcAppSplash.cbSize        = sizeof(m_wcAppSplash);
		m_wcAppSplash.style         = CS_OWNDC|CS_BYTEALIGNWINDOW|CS_BYTEALIGNCLIENT;
		m_wcAppSplash.lpfnWndProc   = CMainFrame::SplashWndProcMain;
		m_wcAppSplash.cbClsExtra    = 0;
		m_wcAppSplash.cbWndExtra    = 0;
		m_wcAppSplash.hInstance     = GetModuleHandle(NULL);
		m_wcAppSplash.hIcon         = NULL;
		m_wcAppSplash.hCursor       = LoadCursor(0, IDC_ARROW);
		m_wcAppSplash.hbrBackground = NULL;
		m_wcAppSplash.lpszMenuName  = NULL;
		m_wcAppSplash.lpszClassName = m_szAppSplashName;
		m_wcAppSplash.hIconSm       = NULL;

		if (!RegisterClassEx(&m_wcAppSplash)){
			return 0;
		}
		return 1;
	}

	//----------------------------------------------------------------------------------------------
	int CreateSplashWindow()
	{
		InitclassexSplash();

		m_hSplashBitmap = (HBITMAP)::LoadImage(NULL, L"resources/SplashScreen.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (m_hSplashBitmap)
		{
			BITMAP bitmap;
			::GetObject(m_hSplashBitmap, sizeof(BITMAP), &bitmap);

			m_SplWidth = bitmap.bmWidth;
			m_SplHeight = bitmap.bmHeight;
		}

		m_hSplashLogo = (HBITMAP)::LoadImage(NULL, L"resources/Logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (m_hSplashLogo)
		{
			BITMAP bitmap;
			::GetObject(m_hSplashLogo, sizeof(BITMAP),&bitmap);

			m_LogoWidth = bitmap.bmWidth;
			m_LogoHeight = bitmap.bmHeight;
		}

		int width  = GetSystemMetrics(SM_CXFULLSCREEN);
		int height = GetSystemMetrics(SM_CYFULLSCREEN);

		width  = width / 2 - m_SplWidth / 2;
		height = height / 2 - m_SplHeight / 2;

		m_hWndSplash = CreateWindow(m_szAppSplashName,
			APP_NAME,
			WS_POPUPWINDOW,
			width, height,
			m_SplWidth, m_SplHeight,
			NULL, NULL,
			GetModuleHandle(NULL),
			NULL);

		if(!m_hWndSplash) {
			return 0;
		}

		::SetWindowLong(m_hWndSplash, GWL_USERDATA, (LONG)this);

		ShowSplash();

		return 1;
	}

	//----------------------------------------------------------------------------------------------
	void ShowSplash()
	{
		::ShowWindow(m_hWndSplash, SW_SHOWNORMAL);
		::UpdateWindow(m_hWndSplash);
		Sleep(2000);
	}

	//----------------------------------------------------------------------------------------------
	void CloseSplash()
	{
		::DestroyWindow(m_hWndSplash);
		m_hWndSplash = NULL;
	}

	HBITMAP		 GetSplashBitmap() const { return m_hSplashBitmap; }
	unsigned int GetSplashWidth()  const { return m_SplWidth; }
	unsigned int GetSplashHeight() const { return m_SplHeight; }

	HBITMAP		 GetSplashLogo() const { return m_hSplashLogo; }
	unsigned int GetLogoWidth()  const { return m_LogoWidth; }
	unsigned int GetLogoHeight() const { return m_LogoHeight; }

	//----------------------------------------------------------------------------------------------
	void OutWindowMessageLog(char* str)
	{
		if (m_hWndSplash != NULL)
		{	
			m_SplashScreenMessage = str;
			::SendMessage(m_hWndSplash, WM_PAINT, 0, 0);
		}
	}

	//----------------------------------------------------------------------------------------------
	static LRESULT CALLBACK SplashWndProcMain(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
	{
		CMainFrame * WndMgr = reinterpret_cast<CMainFrame*>(::GetWindowLong(hWnd, GWL_USERDATA));

		if (WndMgr)
		{
			switch (messg) 
			{
			case WM_PAINT:
				{	
					HDC	  hdc;
					HFONT hCustomFont;
					HFONT hCustomFont2;

					hdc = ::GetDC(hWnd);

					//hdc = BeginPaint(hWnd, &PaintStruct);

					hCustomFont = CreateFont(
						16,7,
						0,0,
						500,0,
						0,0,
						ANSI_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH,
						L"Verdana");

					hCustomFont2 = CreateFont(
						12,5,
						0,0,
						500,0,
						0,0,
						ANSI_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH,
						L"Verdana");

					if (hdc)
					{
						HDC     hMemDC;
						HBITMAP hOldBitmap;

						hMemDC = CreateCompatibleDC(hdc);

						hOldBitmap = (HBITMAP)::SelectObject(hMemDC, WndMgr->GetSplashBitmap());
						BitBlt(hdc, 0, 0, WndMgr->GetSplashWidth(), WndMgr->GetSplashHeight(), hMemDC, 0, 0, SRCCOPY);

						hOldBitmap = (HBITMAP)::SelectObject(hMemDC, WndMgr->GetSplashLogo());
						BitBlt(hdc, WndMgr->GetSplashWidth() - WndMgr->GetLogoWidth() - 15, WndMgr->GetSplashHeight() - WndMgr->GetLogoHeight() * 2, WndMgr->GetLogoWidth(), WndMgr->GetLogoHeight(), hMemDC, 0, 0, SRCCOPY);

						SelectObject(hMemDC, hOldBitmap);
						DeleteDC(hMemDC);
					}

					SelectObject(hdc, hCustomFont);
					SetTextColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, TRANSPARENT);

					std::wstring str = ConvertStringToWideString(STR_COPYRIGHT);
					::TextOut(hdc, (int)(WndMgr->GetSplashWidth() - 160), (int)(WndMgr->GetSplashHeight() - 20), str.c_str(), (int)wcslen(str.c_str()));

					str = ConvertStringToWideString(STR_SOURCE_COPYRIGHT);
					::TextOut(hdc, (int)(WndMgr->GetSplashWidth() - 234), (int)(WndMgr->GetSplashHeight() - 45), str.c_str(), (int)wcslen(str.c_str()));

					SelectObject(hdc, hCustomFont2);
					std::wstring strlog = ConvertStringToWideString(WndMgr->m_SplashScreenMessage);
					::TextOut(hdc, 10, (int)(WndMgr->GetSplashHeight() - 35), strlog.c_str(), (int)wcslen(strlog.c_str()));

					DeleteObject(hCustomFont);
					DeleteObject(hCustomFont2);

					::ReleaseDC(hWnd, hdc);

					//EndPaint(hWnd, &PaintStruct);
				}break;
			}
		}
		return ::DefWindowProc(hWnd, messg, wParam, lParam);
	}

	//----------------------------------------------------------------------------------------------
	void InitViewport()
	{
		RECT rectWindows;
		::GetWindowRect(m_hViewWnd, &rectWindows);

        g_pSceneEditorMain->InitViewport((void*)m_hViewWnd, rectWindows.right - rectWindows.left, rectWindows.bottom - rectWindows.top);

		m_ViewCtrl.m_pRenderContext = m_pAppMain->GetRenderSDK()->GetRenderDriver()->GetDefaultContext();

        UpdateFlagsState();

        // center and show main window
        CenterWindow();
        ShowWindow(1);
	}

    //----------------------------------------------------------------------------------------------
    void Update(float deltaTime)
    {
        g_pSceneEditorMain->Update(deltaTime);
    }

	//----------------------------------------------------------------------------------------------
	void Render()
	{
		m_pAssetBrowserFrame->Render();

        g_pSceneEditorMain->Render();
	}
};

#endif // !defined(AFX_MAINFRM_H__BBA5DFCA_6C1A_11D6_B657_0048548B09C5__INCLUDED_)
