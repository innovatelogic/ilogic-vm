
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

    std::shared_ptr<editors::IEditor> m_editor;
	CCoreSDK *m_pAppMain;

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
        REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	CMainFrame(Registry *pRegistry,
            pContextMenuFunction pfMenu,
            pContextMenuProcessor pfnMenuProcessor,
            pGetResourceIconIndex pfnGetResourceIconIndex,
            CALLBACK_FN pfnInvokeObject,
            CALLBACK_FN pfnDirectInvokeObject,
            CALLBACK_FN pfnClearObject,
            CALLBACK_FN pfnDirectClearObject,
            ValueParser &cmd,
            HIMAGELIST hImageList);

	 ~CMainFrame();

	 LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);

	
     HWND CreateClient();

     void CreatePaneGrid();

    LRESULT OnPaneClose(WORD, WORD, HWND hWndCtl, BOOL&);

    LRESULT OnFileExit(WORD, WORD, HWND, BOOL&);

    LRESULT OnAppAbout(WORD, WORD, HWND, BOOL&);

    LRESULT OnAppInsertObject(UINT, WPARAM, LPARAM, BOOL&);

    LRESULT OnFileOpen(WORD, WORD, HWND, BOOL&);

    LRESULT OnFileSave(WORD, WORD, HWND, BOOL&);

    LRESULT OnWireframe(WORD, WORD, HWND, BOOL&);

    LRESULT OnObjectBounds(WORD, WORD, HWND, BOOL&);

    LRESULT OnSpatialSub(WORD, WORD, HWND, BOOL&);

    LRESULT OnObjectsCollision(WORD, WORD, HWND, BOOL&);

    LRESULT OnShowGrid(WORD, WORD, HWND, BOOL&);

    LRESULT OnBuildPlay(WORD, WORD, HWND, BOOL&);

    LRESULT OnButtonUndo(WORD, WORD, HWND, BOOL&);

    LRESULT OnButtonRedo(WORD, WORD, HWND, BOOL&);

    LRESULT OnModeMove(WORD, WORD, HWND, BOOL&);

    LRESULT OnModeRotate(WORD, WORD, HWND, BOOL&);

    LRESULT OnModeScale(WORD, WORD, HWND, BOOL&);

    void UpdateFlagsState() const;

    void Update(const T_CLASS *pSender, ESystemEventID EventId);

    int InitclassexSplash();

    int CreateSplashWindow();

     void ShowSplash();

     void CloseSplash();

	HBITMAP		 GetSplashBitmap() const { return m_hSplashBitmap; }
	unsigned int GetSplashWidth()  const { return m_SplWidth; }
	unsigned int GetSplashHeight() const { return m_SplHeight; }

	HBITMAP		 GetSplashLogo() const { return m_hSplashLogo; }
	unsigned int GetLogoWidth()  const { return m_LogoWidth; }
	unsigned int GetLogoHeight() const { return m_LogoHeight; }

   void OutWindowMessageLog(char* str);

   static LRESULT CALLBACK SplashWndProcMain(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

   void InitViewport();

   void Update(float deltaTime);

    void Render();

    void InitCallbacks();
};

#include "MainFrame.ipp"
