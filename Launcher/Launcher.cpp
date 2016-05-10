//-------------------------------------------------------------------------------------
// OpenES: Open Entertainment System
// Copyright (C) 2010  Yura Gunko email: yura.gunko@gmail.com
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//-------------------------------------------------------------------------------------

#include "LauncherStdAfx.h"
#include "scene_editor_main.h"

//#define _CRTDBG_MAP_ALLOC

std::wstring g_DDFilename;

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
namespace nmLauncher
{
	/** init process instance */
	typedef std::map<std::string, int> TIconRefs;
	
	/** root process */
	CCoreSDK		*GRootProcess = NULL;

    editors::SceneEditorMain *g_pSceneEditorMain = nullptr;

	CMainFrame<CActor>	*wndMain = NULL;

	Win32ObjectBrowserWidget<CActor> *pGTreeBrowser = NULL;

	CActor * SelectedActor = NULL;
	int NumBrushes = INDEX_NONE;

	TIconRefs		TreeIconRefs;
	HIMAGELIST		hTypeImageList;

	TIconRefs		OperationIconRefs;
	HIMAGELIST		TypeOperationImageList;

	int *localData = NULL;
	int threadRenderID = 0;
	int threadPhysID = 0;
	int threadTimerID = 0;
	int threadStreamingID = 0;

	CriticalSection timeUpdateCS;
	float AppTimePassed = 0.f;

	CriticalSection callsPerSecondCS;
	int MillisecondsPassed = 0;
	int eventCPS  = 0;
	int updateCPS = 0;
	int renderCPS = 0;
	int timerCPS  = 0;
	int streamCPS = 0;

	volatile LONG bFrameBackBufferWrite = FALSE;
	volatile LONG bFrameBufferPrepeared = FALSE;
	volatile LONG bFrameChanged = FALSE;
	volatile LONG bFramePrepared = FALSE;

	boost::shared_ptr<class Registry> pRegistryInstance; 

	CAppModule _Module;
}
using namespace nmLauncher;

//----------------------------------------------------------------------------------------------
// process App. data
//----------------------------------------------------------------------------------------------
int			InitThreads();
void		ReleaseThreads();

int			DialogInputBox(LPCTSTR szTitle, LPCTSTR szPrompt, LPTSTR szResult, DWORD nResultSize, bool bMultiLine = false);
int			CalculateTimeDeltaMilliseconds();
int			GetMilliseconds();
int			CALLBACK_ActorSubscribe(const CActor *pSender);
int			CALLBACK_ActorSubscribeEvents(const CActor * Sender);
void		OnActor_EventUpdated(const CActor *pSender, ESystemEventID EventId);

/** context menu handler */
void		HandlePopupContextMenu(HWND hwnd, POINT pt, CActor * Actor);
bool		ContextMenuProcessor(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

bool		Copy();
bool		Paste();
bool		DeleteObject();
bool		RenameObject();

void		InitializeImageData(const class Registry * registry);
int			LoadImageResource(const std::string& TypeName, const std::string& TypeIcon, HIMAGELIST &ImageList, TIconRefs &Refs);

int			GetImageResource(const std::string& TypeName);
int			GetImageOperation(const std::string& TypeName);
HICON		GetImageOperationIcon(const std::string& TypeName);

void		Thread_RenderFunction();
void		Thread_PhysFunction();
void		Thread_TimerFunction();
void		Thread_ResourceStreaming();

float		UpdateDeltaTime();
void		UpdateCallsPerSecond(int milliseconds);
void		ProcessUpdate(float DeltaTime);
void		UpdateFrame();

//----------------------------------------------------------------------------------------------
void OnActor_EventUpdated(const CActor *pSender, ESystemEventID EventId)
{
	assert(pSender);

	if (EventId == Event_ObjectGenerated)
	{
		for (int EIndex = Event_ObjectGenerated; EIndex < Event_MAX; EIndex++) 
		{	
			REGISTER_EVENT_(const_cast<CActor*>(pSender), NULL, boost::bind<void>(&OnActor_EventUpdated, _1, _2), (ESystemEventID)EIndex);
		}
	}

	wndMain->Update(pSender, EventId);
}

//----------------------------------------------------------------------------------------------
int CALLBACK_ActorSubscribeEvents(const CActor *pSender)
{	
	assert(pSender);

	// subscribe user function to all to all actor events sender
	for (int EIndex = Event_ObjectGenerated; EIndex < Event_MAX; EIndex++) 
	{
		REGISTER_EVENT_(const_cast<CActor*>(pSender), NULL, boost::bind<void>(&OnActor_EventUpdated, _1, _2), (ESystemEventID)EIndex);
	}
	return 0;
}

//------------------------------------------------------------------------
int GCALLBACK_InvokeActor(const CActor *pSender)
{
	return pGTreeBrowser->InvokeActor(pSender);
}

//------------------------------------------------------------------------
int GCALLBACK_DirectInvokeActor(const CActor *pSender)
{       
	return pGTreeBrowser->DirectInvokeActor(pSender);
}

//------------------------------------------------------------------------
int GCALLBACK_ClearActor(const CActor *pSender)
{	
	return pGTreeBrowser->ClearActor(pSender);
}

//------------------------------------------------------------------------
int GCALLBACK_DirectClearActor(const CActor *pSender)
{       
	return pGTreeBrowser->DirectClearActor(pSender);
}

//----------------------------------------------------------------------------------------------
int Run(ValueParser &cmd, LPTSTR = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CGameLoop theLoop(UpdateFrame);

	_Module.AddMessageLoop(&theLoop);

	InitThreads();

	// allocate instances	
	pRegistryInstance = boost::shared_ptr<Registry>(new Registry());
	pRegistryInstance->Load("registry.xml");

	InitializeImageData(pRegistryInstance.get());

	wndMain = new CMainFrame<CActor>(pRegistryInstance.get(),
							HandlePopupContextMenu,
							ContextMenuProcessor,
							GetImageResource,
							GCALLBACK_InvokeActor,
							GCALLBACK_DirectInvokeActor,
							GCALLBACK_ClearActor,
							GCALLBACK_DirectClearActor,
							OnActor_EventUpdated,
							cmd,
							hTypeImageList);

	wndMain->CreateSplashWindow();

	GRootProcess = wndMain->m_pAppMain;

    g_pSceneEditorMain = new editors::SceneEditorMain(GRootProcess, new editors::CommandBuffer);

	// size of window to create
	CRect rc = CRect(0, 0, 1024, 768);
	if (wndMain->CreateEx(NULL, rc) == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	// logging
	wndMain->OutWindowMessageLog("Initialization completed");

	wndMain->CloseSplash();
	
	GRootProcess->Deserialize("3d_scene_controller.xml", NULL);

    //editors::TIEditor editor = GRootProcess->CreateEdtior("TestEditor");

	//GRootProcess->Deserialize("d:\\Project\\$cashe\\SceneLogin01.xml", NULL);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();

	return nRet;
}

//----------------------------------------------------------------------------------------------
void UpdateFrame()
{
	//while (InterlockedExchange(&bFrameChanged, TRUE) == TRUE){
	//	Sleep(0);
	//}

	float DeltaTime = UpdateDeltaTime();
	GRootProcess->ElapsedTime = DeltaTime;

	MillisecondsPassed += CalculateTimeDeltaMilliseconds();

	if (MillisecondsPassed > CPS_FREQUENCY_MILLISECONDS)
	{
		UpdateCallsPerSecond(MillisecondsPassed);
		MillisecondsPassed = 0;
	}

	int Mp1, Mp2;
	Mp1 = Mp2 = 0;

	CalculateTimeDeltaMilliseconds();
	Mp1 = CalculateTimeDeltaMilliseconds();

	GRootProcess->EnterDrawCS();

	CalculateTimeDeltaMilliseconds();

	if (MillisecondsPassed == 0){
		GRootProcess->GetRenderSDK()->SetDebugInfoCPS(true, eventCPS, updateCPS, renderCPS, timerCPS, streamCPS);
	}

	GRootProcess->GetRenderSDK()->SetDeltaTime(DeltaTime);

	GRootProcess->ProcessUpdate(DeltaTime);

	GRootProcess->Draw(); // draw new frame to back buffer

	InterlockedExchange(&bFramePrepared, TRUE);

	Mp2 = CalculateTimeDeltaMilliseconds();

	GRootProcess->LeaveDrawCS();

	//GRootProcess->Render(0, renderCPS); // Render & change buffer

	wndMain->Render();

	{
		static float LastTimeUpdate = 0.f;
		static float DeltaCall = 0.f;

		float DeltaTime = AppTimePassed - LastTimeUpdate;
		LastTimeUpdate  = AppTimePassed;

		DeltaCall += DeltaTime;

		//if (DeltaCall > 0.001f)
		{
			GRootProcess->SimulatePhysics(DeltaCall * 1.4f);
			DeltaCall = 0.f;
		}
	}

	//tm.doSerialWork();
}

//----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrInst, LPSTR sCmdStr, int nCmdShow)
{
 #if defined(DEBUG) | defined(_DEBUG) // Enable run-time memory check for debug builds.
 //	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(13);
// 	_CrtSetBreakAlloc(1);
// 	_crtBreakAlloc = 1;
 #endif

	OleInitialize(NULL);

	ValueParser CommandLineStr("");
	CommandLineStr.ParseCommandLine(sCmdStr);

#ifndef USE_WTL

	ThreadManagerSerial &tm = ThreadManagerSerial::getThreadManagerSerial();

 	try
 	{
		InitThreads();
		InitCommonControls();
		
		// start core root process
		GRootProcess = new CCoreSDK(CommandLineStr);

		GRootProcess->SetIsAEditor(true);
		GRootProcess->SetIsAGame(false);

		WindowManager = new CWindowManager(GRootProcess);
		WindowManager->Initialize(HandlePopupContextMenu, ContextMenuProcessor, GetImageOperationIcon);

		// logging
		WindowManager->OutWindowMessageLog(std::string("Start Loading..."));
		
		InitializeImageData(GRootProcess->GetRegistry());

		srand(CalculateTimeDeltaMilliseconds());

		HWND Wm_HWND = WindowManager->GetWindowManagerHWND();
/*
		CWin32ObjectBrowser::Initialize(WindowManager->m_pCtrlWindowRightBottom->GetHWND(),
										HandlePopupContextMenu,
										ContextMenuProcessor,
										GetImageResource,
										hTypeImageList,
										WindowManager,
										true);*/

		pGTreeBrowser = new Win32ObjectBrowserWidget<CActor>(0,
			WindowManager->m_pCtrlWindowRightBottom->GetHWND(),
			WindowManager, 
			HandlePopupContextMenu,
			ContextMenuProcessor,
			GetImageResource,
			GCALLBACK_InvokeActor,
			GCALLBACK_DirectInvokeActor,
			GCALLBACK_ClearActor,
			GCALLBACK_DirectClearActor,
			hTypeImageList);

		WindowManager->m_pCtrlWindowRightBottom->AddChild(pGTreeBrowser);
		WindowManager->m_pCtrlWindowRightBottom->pWidget = pGTreeBrowser;

		//CWin32ObjectToolbox::Initialize(Wm_HWND, GetImageResource, hTypeImageList, WindowManager);
		//CWin32PropertyWindow::Initialize(WindowManager->m_pCtrlWindowRightTop->GetHWND(), WindowManager, true);
		CWin32ThreadMonitor::Initialize(Wm_HWND, WindowManager, threadPhysID, threadRenderID, threadTimerID, threadStreamingID);
		CWin32CameraManager::Initialize(Wm_HWND, HandlePopupContextMenu, ContextMenuProcessor, GetImageResource, hTypeImageList, WindowManager);
		CWin32ActionPlayEditor::Initialize(Wm_HWND, HandlePopupContextMenu, ContextMenuProcessor, GetImageResource, hTypeImageList, WindowManager);

		ShowWindow(Wm_HWND, SW_SHOWMAXIMIZED);

		// logging
		WindowManager->OutWindowMessageLog(std::string("Initialize Core"));
		
		OnActor_EventUpdated(GRootProcess->GetExplorerInstance(), Event_ObjectGenerated); 

		RECT rectWindows;
		GetWindowRect(WindowManager->m_pCtrlWindow->GetHWND(), &rectWindows);

		unsigned int wndWidth = rectWindows.right - rectWindows.left;
		unsigned int wndHeight = rectWindows.bottom - rectWindows.top;
		
		GRootProcess->Initialize(WindowManager->m_pCtrlWindow->GetHWND(), wndWidth, wndHeight);

		// logging
		WindowManager->OutWindowMessageLog(std::string("Initialization completed"));
		WindowManager->CloseSplash();
		
		tm.startThread(threadRenderID);
		tm.startThread(threadPhysID);
		tm.startThread(threadTimerID);
		tm.startThread(threadStreamingID);

		ThreadManagerSerial::threadInfo *Info =  tm.GetThreadInfo(threadRenderID);
		DWORD Id = ::GetThreadId(Info->thread);
		GRootProcess->GetRenderSDK()->ThreadRenderID = Id;
		
		WindowManager->UpdateFlagsState();
 	}
 	catch (const std::exception &e)
 	{
 		_cputs("Core Initialization: Exception raised:\n");
 		_cputs(e.what());
         return EXIT_FAILURE;
 	}
	
 	// main loop
 	while (WindowManager->PeekMessageWindow())
	{
		while (InterlockedExchange(&bFrameChanged, TRUE) == TRUE){
			Sleep(0);
		}

		float DeltaTime = UpdateDeltaTime();
		GRootProcess->ElapsedTime = DeltaTime;

		MillisecondsPassed += CalculateTimeDeltaMilliseconds();

		if (MillisecondsPassed > CPS_FREQUENCY_MILLISECONDS)
		{
			UpdateCallsPerSecond(MillisecondsPassed);
			MillisecondsPassed = 0;
		}

		int Mp1, Mp2;
		Mp1 = Mp2 = 0;
		
		CalculateTimeDeltaMilliseconds();
		Mp1 = CalculateTimeDeltaMilliseconds();
				
		GRootProcess->EnterDrawCS();

		CalculateTimeDeltaMilliseconds();

		if (MillisecondsPassed == 0){
			GRootProcess->GetRenderSDK()->SetDebugInfoCPS(true, eventCPS, updateCPS, renderCPS, timerCPS, streamCPS);
		}

		GRootProcess->GetRenderSDK()->SetDeltaTime(DeltaTime);

		GRootProcess->ProcessUpdate(DeltaTime);
						
		GRootProcess->Draw(); // draw new frame to back buffer

		InterlockedExchange(&bFramePrepared, TRUE);
	
		Mp2 = CalculateTimeDeltaMilliseconds();

		GRootProcess->LeaveDrawCS();

		GRootProcess->Render(true, renderCPS); // Render & change buffer

		{
			static float LastTimeUpdate = 0.f;
			static float DeltaCall = 0.f;

			float DeltaTime = AppTimePassed - LastTimeUpdate;
			LastTimeUpdate  = AppTimePassed;

			DeltaCall += DeltaTime;

			//if (DeltaCall > 0.001f)
			{
				GRootProcess->SimulatePhysics(DeltaCall * 1.4f);
				DeltaCall = 0.f;
			}
		}

		tm.doSerialWork();
 	}

	ReleaseThreads();

	delete tm.sm_threadManager;
	delete WindowManager;
	delete GRootProcess;

	NObjectFactory::TClassFactory * classFactory = GetClassFactoryA();

	classFactory->m_MapGenerator.clear();
	delete classFactory;

#else

	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	::DefWindowProc(NULL, 0, 0, 0L);

	hRes = _Module.Init(NULL, hInst);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(CommandLineStr);

	_Module.Term();
	::CoUninitialize();

	OleUninitialize();

#endif//USE_WTL

	return 0;
}

//----------------------------------------------------------------------------------------------
int InitThreads()
{
	ThreadManagerSerial &tm = ThreadManagerSerial::getThreadManagerSerial();

	localData = new int[THREAD_SPAWN_COUNT + 1];
	for(int i = 0; i <= THREAD_SPAWN_COUNT; i++){
		localData[i] = 0;
	}

	// get a pointer to our ThreadManager
	if (!tm.init(THREAD_SPAWN_COUNT, &localData[0])){
		return EXIT_FAILURE;
	}

	// setup update thread
	threadPhysID = tm.AssignThreadToWorkFunction(Thread_PhysFunction, NULL,	&localData[1]);
	if (threadPhysID == INDEX_NONE){
		return EXIT_FAILURE;
	}

	// setup rendering thread
	threadRenderID = tm.AssignThreadToWorkFunction(Thread_RenderFunction, NULL, &localData[2]);
	if (threadRenderID == INDEX_NONE){
		return EXIT_FAILURE;
	}

	threadTimerID = tm.AssignThreadToWorkFunction(Thread_TimerFunction, NULL, &localData[3]);
	if (threadTimerID == INDEX_NONE){
		return EXIT_FAILURE;
	}

	threadStreamingID = tm.AssignThreadToWorkFunction(Thread_ResourceStreaming, NULL, &localData[4]);
	if (threadStreamingID == INDEX_NONE){
		return EXIT_FAILURE;
	}

// 	tm.SetThreadAffinity(threadTimerID, 0x00000001);
// 	tm.SetThreadAffinity(threadRenderID, 0x00000001);
// 	tm.SetThreadAffinity(threadPhysID, 0x00000001);
// 	tm.SetThreadAffinity(threadStreamingID, 0x00000001);

// 	tm.setThreadSerialized(threadPhysID, true);
// 	tm.setThreadSerialized(threadRenderID, true);
// 	tm.setThreadSerialized(threadTimerID, true);
// 	tm.setThreadSerialized(threadStreamingID, true);

	return 0;
}

//----------------------------------------------------------------------------------------------	
void Thread_RenderFunction()
{	
	if (bFramePrepared == TRUE)
	{
		//GRootProcess->Render(true, renderCPS); // change buffer

		InterlockedExchange(&bFramePrepared, FALSE);
		InterlockedExchange(&bFrameChanged, FALSE);
	}
}

//----------------------------------------------------------------------------------------------
void Thread_PhysFunction()
{
	/*static float LastTimeUpdate = 0.f;
	static float DeltaCall = 0.f;

	float DeltaTime = AppTimePassed - LastTimeUpdate;
	LastTimeUpdate  = AppTimePassed;
	
	DeltaCall += DeltaTime;

	if (DeltaCall > 0.01f)
	{
		GRootProcess->SimulatePhysics(DeltaCall * 1.4f);
		DeltaCall = 0.f;
	}*/
}

//----------------------------------------------------------------------------------------------
void Thread_TimerFunction()
{
	static LARGE_INTEGER previousTime;
	static LARGE_INTEGER freq;
	static bool init_timegettime = false;

	if (!init_timegettime)
	{
		::QueryPerformanceFrequency(&freq);
		::QueryPerformanceCounter(&previousTime);
		init_timegettime=true;
	}

	LARGE_INTEGER currentTime;
	::QueryPerformanceCounter(&currentTime);

	unsigned long long elapsedTime = currentTime.QuadPart - previousTime.QuadPart;
	previousTime = currentTime;

	AppTimePassed += (float)(elapsedTime)/(freq.QuadPart);
}

//----------------------------------------------------------------------------------------------
void Thread_ResourceStreaming()
{
	GRootProcess->GetRenderSDK()->ProcessStreaming();
}

//----------------------------------------------------------------------------------------------
void ReleaseThreads()
{
	ThreadManagerSerial &tm = ThreadManagerSerial::getThreadManagerSerial();
	tm.shutdown();
	delete [] localData;
}

//----------------------------------------------------------------------------------------------
int DialogInputBox(LPCTSTR szTitle, LPCTSTR szPrompt, LPTSTR szResult, DWORD nResultSize, bool bMultiLine)
{
	return (int)CWin32InputBox::InputBox(szTitle, szPrompt, szResult, nResultSize, bMultiLine);
}

//----------------------------------------------------------------------------------------------
int CalculateTimeDeltaMilliseconds()
{
	int output = 0;

#ifdef WIN32
	timeUpdateCS.enter();

	static ULARGE_INTEGER lastTime;

	FILETIME ft;
	ULARGE_INTEGER currentTime;
	GetSystemTimeAsFileTime(&ft);
	currentTime.HighPart = ft.dwHighDateTime;
	currentTime.LowPart = ft.dwLowDateTime;

	output = ((int)(currentTime.QuadPart - lastTime.QuadPart) / 10000);
	lastTime = currentTime;

	timeUpdateCS.leave();
#endif

	return output;
}

//----------------------------------------------------------------------------------------------
int GetMilliseconds()
{
	FILETIME ft;
	ULARGE_INTEGER currentTime;
	GetSystemTimeAsFileTime(&ft);
	currentTime.HighPart = ft.dwHighDateTime;
	currentTime.LowPart = ft.dwLowDateTime;

	return (int)currentTime.QuadPart / 10000;
}

//----------------------------------------------------------------------------------------------
void HandlePopupContextMenu(HWND hwnd, POINT pt, CActor * Selected)
{
	HMENU hmenu = LoadMenu((HINSTANCE)::GetModuleHandle(0), MAKEINTRESOURCE(IDR_BROWSER_MENU));
	if (hmenu == NULL) return;

	HICON hIcon = GetImageOperationIcon("Copy");
	if (hIcon)
	{
		ICONINFO iconinfo;
		GetIconInfo(hIcon, &iconinfo);
		SetMenuItemBitmaps(hmenu, ID_BROWSER_CONTEXT_COPY, MF_BYCOMMAND, iconinfo.hbmColor, NULL);
	}

	hIcon = GetImageOperationIcon("Paste");
	if (hIcon)
	{
		ICONINFO iconinfo;
		GetIconInfo(hIcon, &iconinfo);
		SetMenuItemBitmaps(hmenu, ID_BROWSER_CONTEXT_PASTE, MF_BYCOMMAND, iconinfo.hbmColor, NULL);
	}

	hIcon = GetImageOperationIcon("Rename");
	if (hIcon)
	{
		ICONINFO iconinfo;
		GetIconInfo(hIcon, &iconinfo);
		SetMenuItemBitmaps(hmenu, ID_BROWSER_CONTEXT_RENAME, MF_BYCOMMAND, iconinfo.hbmColor, NULL);
	}
	
	hIcon = GetImageOperationIcon("Delete");
	if (hIcon)
	{
		ICONINFO iconinfo;
		GetIconInfo(hIcon, &iconinfo);
		SetMenuItemBitmaps(hmenu, ID_BROWSER_CONTEXT_DELETE, MF_BYCOMMAND, iconinfo.hbmColor, NULL);
	}
				
	HMENU hmenuTrackPopup = GetSubMenu(hmenu, 0);
	hIcon = GetImageOperationIcon("Add");
	if (hIcon)
	{
		ICONINFO iconinfo;
		GetIconInfo(hIcon, &iconinfo);
		SetMenuItemBitmaps(hmenuTrackPopup, 0, MF_BYCOMMAND, iconinfo.hbmColor, NULL);
	}

	
/*
		bool bVisible = Selected->GetVisible();
		ModifyMenu(hmenuTrackPopup, ID_BROWSER_CONTEXT_HIDE_UNHIDE, MF_BYCOMMAND | MF_STRING, ID_BROWSER_CONTEXT_HIDE_UNHIDE, bVisible ? _T("Hide") : _T("Unhide"));

		hIcon = GetImageOperationIcon(bVisible ? "Hide" : "Unhide");
		if (hIcon)
		{
			ICONINFO iconinfo;
			GetIconInfo(hIcon, &iconinfo);
			SetMenuItemBitmaps(hmenuTrackPopup, 0, ID_BROWSER_CONTEXT_HIDE_UNHIDE, iconinfo.hbmColor, NULL);
		}

		bool bExternal = Selected->GetExternal();
		
		hIcon = GetImageOperationIcon("Open");
		if (hIcon)
		{
			ICONINFO iconinfo;
			GetIconInfo(hIcon, &iconinfo);
			BOOL RES = SetMenuItemBitmaps(GetSubMenu(hmenuTrackPopup, 8), 0, ID_BROWSER_CONTEXT_LOADEXTERNAL, iconinfo.hbmColor, NULL);
			RES = FALSE;
		}
		
		hIcon = GetImageOperationIcon("Save");
		if (hIcon)
		{
			ICONINFO iconinfo;
			GetIconInfo(hIcon, &iconinfo);
			BOOL RES = SetMenuItemBitmaps(GetSubMenu(hmenuTrackPopup, 8), 0, ID_BROWSER_CONTEXT_SAVEEXTERNAL, iconinfo.hbmColor, NULL);
			RES = FALSE;
		}

		// add command list
		int NumLines = 0;
		for (TVecPropertyIterator Iter = Selected->PropertyMap.begin(); Iter != Selected->PropertyMap.end(); ++Iter)
		{
			if (dynamic_cast<const CommandImpl*>(*Iter))
			{
				MENUITEMINFO iInfo;
				memset(&iInfo, 0, sizeof(MENUITEMINFO));
				iInfo.cbSize	= sizeof(MENUITEMINFO);
				iInfo.hSubMenu  = GetSubMenu(hmenuTrackPopup, 8);
				iInfo.wID		= (UINT)NumLines + 4000;
				iInfo.fMask		= MIIM_ID | MIIM_STRING;
				iInfo.fType		= MFT_STRING;

				wchar_t wbuf[100] = {0};
				MultiByteToWideChar(CP_ACP, 0, (*Iter)->GetName(), INDEX_NONE, wbuf, 100);

				iInfo.dwTypeData = wbuf;
				iInfo.cch = (UINT)strlen((*Iter)->GetName());

				InsertMenuItem(GetSubMenu(hmenuTrackPopup, 8), NumLines + 4000, TRUE, (LPCMENUITEMINFO) &iInfo);
				++NumLines;
			}
		}
*/
	
	std::vector<const ClassNode*> vecNodes;
	AppClassTree &classTree = NObjectFactory::GetClassTree();

	CEnumerateChildTypeStrategy<std::vector<const ClassNode*> > ObjEnumerate(IDrawInterface::ThisClassName(), classTree.GetTree(), vecNodes);

	NumBrushes = 0;
	for (size_t Index = 0; Index < vecNodes.size(); ++Index)
	{
		MENUITEMINFO iInfo;
		memset(&iInfo, 0, sizeof(MENUITEMINFO));
		iInfo.cbSize	= sizeof(MENUITEMINFO);
		iInfo.hSubMenu  = GetSubMenu(hmenuTrackPopup, 12);
		iInfo.wID		= (UINT)NumBrushes + 5000;
		iInfo.fMask		= MIIM_ID | MIIM_STRING;
 		iInfo.fType		= MFT_STRING;
	 
 		wchar_t wbuf[100] = {0};
 		MultiByteToWideChar(CP_ACP, 0, vecNodes[Index]->GetName(), INDEX_NONE, wbuf, 100);
	 
 		iInfo.dwTypeData = wbuf;
 		iInfo.cch = (UINT)strlen(vecNodes[Index]->GetName());

		TIconRefs::iterator IterFind = TreeIconRefs.find(vecNodes[Index]->GetName());
		if (IterFind != TreeIconRefs.end())
		{
			HICON hIcon = ImageList_GetIcon(hTypeImageList, IterFind->second, NULL);

			ICONINFO hIconInfo;
			if (hIcon && GetIconInfo(hIcon, &hIconInfo))
			{
				iInfo.fMask |= MIIM_BITMAP;
				iInfo.hbmpItem = hIconInfo.hbmColor;
			}
		}
 		InsertMenuItem(GetSubMenu(hmenuTrackPopup, 12), NumBrushes + 5000, TRUE, (LPCMENUITEMINFO) &iInfo);
		NumBrushes++;
	}

	TrackPopupMenuEx(hmenuTrackPopup, TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, hwnd, NULL);
	DestroyMenu(hmenu);

	SelectedActor = Selected;
}

//----------------------------------------------------------------------------------------------
bool ContextMenuProcessor(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	bool bResult = false;

	switch (messg) 
	{
	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{
		case ID_BROWSER_CONTEXT_COPY:
			bResult = Copy();
			break;
		case ID_BROWSER_CONTEXT_PASTE:
			bResult = Paste();
			break;
		case ID_BROWSER_CONTEXT_RENAME:
			bResult = RenameObject();
			bResult = true;
			break;
		case ID_BROWSER_CONTEXT_DELETE:
			bResult = DeleteObject();
			break;
		case ID_BROWSER_CONTEXT_MOVEUP:
			bResult = ActorAllocator::MoveObjectHierarchy(SelectedActor, GRootProcess->GetRegistry(), true, true);
			break;
		case ID_BROWSER_CONTEXT_MOVEDOWN:
			bResult = ActorAllocator::MoveObjectHierarchy(SelectedActor, GRootProcess->GetRegistry(), false, true);
			break;
		case ID_BROWSER_CONTEXT_MOVETOP:
			bResult = CActor::MoveObjectHierarchyBound(SelectedActor, true);
			break;
		case ID_BROWSER_CONTEXT_MOVEBOTTOM:
			bResult = CActor::MoveObjectHierarchyBound(SelectedActor, false);
			break;
		case ID_BROWSER_CONTEXT_HIDE_UNHIDE:
			{
				IDrawInterface *pIFocused = GRootProcess->GetViewportManager()->GetByActor(SelectedActor);
				if (pIFocused){
					pIFocused->SetVisible(!pIFocused->GetVisible());
				}
			}break;
		case ID_BROWSER_CONTEXT_FREEZE:
			// TODO
			break;

		case ID_BROWSER_CONTEXT_SAVEEXTERNAL:
			{
				OPENFILENAME ofn;
				WCHAR szFileName[MAX_PATH] = L"";
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn); 
				ofn.hwndOwner = wndMain->m_hWnd;
				ofn.lpstrFilter = L"XML Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
				ofn.lpstrFile = szFileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = L"txt";

				CHAR chFileName[MAX_PATH] = "";
				if (GetSaveFileName(&ofn) && ConvertWideStringToAnsiCch(chFileName, szFileName, MAX_PATH))
				{
					GRootProcess->SerializeActor(chFileName, SelectedActor, true);
				}
			}break;

		case ID_BROWSER_CONTEXT_LOADEXTERNAL:
			{
				OPENFILENAME ofn;
				WCHAR szFileName[MAX_PATH] = L"";
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn); 
				ofn.hwndOwner = wndMain->m_hWnd;
				ofn.lpstrFilter = L"XML Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
				ofn.lpstrFile = szFileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = L"txt";

				CHAR chFileName[MAX_PATH] = "";
				if (GetOpenFileName(&ofn) && ConvertWideStringToAnsiCch(chFileName, szFileName, MAX_PATH))
				{
 					GRootProcess->DeserializeActor(chFileName, SelectedActor, true);
				}
			}break;
		}

		// process commands
		if ((INT)(wParam) >= 4000 && (INT)(wParam) < 5000)
		{
			
			/*INT Index = 0;
			INT SelIndex = (INT)(wParam) - 4000;
			for (TVecPropertyIterator Iter = SelectedActor->PropertyMap.begin(); Iter != SelectedActor->PropertyMap.end(); ++Iter)
			{
				if (dynamic_cast<const CommandImpl*>(*Iter))
				{
					if (Index == SelIndex)
					{
						dynamic_cast<CommandImpl*>(*Iter)->ExecuteCommand();
						break;
					}
					Index++;
				}
			}*/
		}

		// process brushes
		if ((INT)(wParam) >= 5000 && (INT)(wParam) < 6000)
		{
			AppClassTree &classTree = NObjectFactory::GetClassTree();

			std::vector<const ClassNode*> vecNodes;

			CEnumerateChildTypeStrategy<std::vector<const ClassNode*> > ObjEnumerate(IDrawInterface::ThisClassName(), classTree.GetTree(), vecNodes);

			const ClassNode *pClass = vecNodes.at((INT)(wParam) - 5000);

			std::string strNewName = CActor::ResolveName(std::string("New"), SelectedActor);
			CActor *pNewObject = static_cast<CActor*>(GENERATE_OBJECT_SDK(pClass->GetName(), strNewName.c_str(), SelectedActor));

			assert(pNewObject);

			pNewObject->SetTransient(false);

			for (std::vector<IBaseInterface*>::iterator Iter = pNewObject->m_VecInterfacesImp.begin(); Iter != pNewObject->m_VecInterfacesImp.end(); ++Iter)
			{
				if (IDrawInterface *pInterface = dynamic_cast<IDrawInterface*>(*Iter))
				{
					pInterface->SetVisible(true);

					if (IDrawInterface *pIActorSelected = GRootProcess->GetViewportManager()->GetByActor(SelectedActor))
					{
						Matrix NewLTM;
						if (IDrawInterface::GetObjectAInLocalSpaceB(NewLTM, pInterface, pIActorSelected))
						{
							pInterface->SetLTM_(NewLTM);
						}
					}
					break;
				}
			}

			SelectedActor->AddChildNode(pNewObject);

			pNewObject->Initialize();

			GRootProcess->GetViewportManager()->RebuildTransform(pNewObject);

			/*ViewportInterface * VInterface = ActorAllocator::GetViewportInterface(SelectedActor);

			if (VInterface)
			{
				const CActor * VRoot = dynamic_cast<const CActor*>(VInterface);
				assert(VRoot);

				INT SelIndex = (INT)(wParam) - 5000;
				if (SelIndex >= 0 && SelIndex < NumBrushes)
				{
					int IndexBrush = 0;
					for (size_t Index = 0; Index < VRoot->m_ChildNodes.size(); ++Index)
					{
						if (dynamic_cast<const Brush_AbstractInterface*>(VRoot->m_ChildNodes[Index]))
						{
							if (SelIndex == IndexBrush)
							{
								//VRoot->m_ChildNodes[Index]->GainFocus();
								bResult = true;
								break;
							}
							IndexBrush++;
						}
					}
				}
			}*/
		}
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool Copy()
{
	assert(SelectedActor);
	CActor * RootNode = SelectedActor->GetRootNode();

	if (RootNode){
		RootNode->Copy(SelectedActor);
	}
	return true;
}

//----------------------------------------------------------------------------------------------
bool Paste()
{
	assert(SelectedActor);
	CActor *pRootNode = SelectedActor->GetRootNode();

	if (pRootNode){
		pRootNode->Paste(SelectedActor);
	}
	return true;
}

//----------------------------------------------------------------------------------------------
bool DeleteObject()
{
	assert(SelectedActor);
	SelectedActor->Release();
	return true;
}

//----------------------------------------------------------------------------------------------
bool RenameObject()
{
	assert(SelectedActor);

	wchar_t wbuf[100] = {0};
	char ascii[100] = {0};

	MultiByteToWideChar(CP_ACP, 0, SelectedActor->GetName(), -1, wbuf, 100);
	if (CWin32InputBox::InputBox(L"Rename", L"Please enter new name", wbuf, 100, false) == IDOK)
	{
		wcstombs(ascii, wbuf, wcslen(wbuf));
		SelectedActor->Rename(ascii);
	}
	return true;
}


//----------------------------------------------------------------------------------------------
void InitializeImageData(const Registry * registry)
{
	if (registry)
	{
		// load type icons
		hTypeImageList = ImageList_Create(16, 16, ILC_COLOR32, 255, 1024);

		std::map<std::string, std::string>::const_iterator Iter = registry->MapTypeIcons.begin();
		while (Iter != registry->MapTypeIcons.end())
		{
			LoadImageResource(Iter->first, Iter->second, hTypeImageList, TreeIconRefs);
			++Iter;
		}

		// load type icons
		TypeOperationImageList = ImageList_Create(16, 16, ILC_COLOR32, 255, 1024);

		Iter = registry->MapOprationsIcons.begin();
		while (Iter != registry->MapOprationsIcons.end())
		{
			LoadImageResource(Iter->first, Iter->second, TypeOperationImageList, OperationIconRefs);
			++Iter;
		}
	}
}

//----------------------------------------------------------------------------------------------
int LoadImageResource(const std::string& TypeName, const std::string& TypeIcon, HIMAGELIST &ImageList, TIconRefs &Refs)
{	
	int IndexBitmap = 0;

	// load bmp icon
	TIconRefs::iterator IterIcon = Refs.find(TypeName);

	if (IterIcon != Refs.end())
	{
		IndexBitmap = IterIcon->second;
	}
	else
	{
		HBITMAP hBitmap = NULL;
		std::wstring FileName = ConvertStringToWideString(TypeIcon);
		hBitmap = (HBITMAP)LoadImage(::GetModuleHandle(NULL), (LPWSTR)FileName.c_str(), IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);

		if (hBitmap)
		{
			IndexBitmap = ImageList_Add(ImageList, hBitmap, NULL); // Macro: Attach the image, to the image list
			Refs.insert(std::make_pair(TypeName, IndexBitmap));
		}
		DeleteObject(hBitmap);
	}
	return IndexBitmap;
}

//----------------------------------------------------------------------------------------------
int GetImageResource(const std::string& TypeName)
{
	// load bmp icon
	TIconRefs::iterator IterIcon = TreeIconRefs.find(TypeName);

	if (IterIcon != TreeIconRefs.end())
	{
		return IterIcon->second;
	}
	return INDEX_NONE;
}

//----------------------------------------------------------------------------------------------
int GetImageOperation(const std::string& TypeName)
{
	// load bmp icon
	TIconRefs::iterator IterIcon = OperationIconRefs.find(TypeName);

	if (IterIcon != OperationIconRefs.end())
	{
		return IterIcon->second;
	}
	return INDEX_NONE;
}

//----------------------------------------------------------------------------------------------
HICON GetImageOperationIcon(const std::string& TypeName)
{
	// load bmp icon
	TIconRefs::iterator IterIcon = OperationIconRefs.find(TypeName);

	if (IterIcon != OperationIconRefs.end())
	{
		return ImageList_GetIcon(TypeOperationImageList, IterIcon->second, NULL);
	}
	return NULL;
}

//----------------------------------------------------------------------------------------------
float UpdateDeltaTime()
{
	static float LastTimeUpdate = 0.f;
	static float StatTimePassed = 0.f;

	float DeltaTime = AppTimePassed - LastTimeUpdate;
	LastTimeUpdate  = AppTimePassed;

	StatTimePassed += DeltaTime;

	if (StatTimePassed > CPS_FREQUENCY_SECONDS)
	{
		// update number of calls for all our threads
		UpdateCallsPerSecond((int)StatTimePassed);
		StatTimePassed = 0.f;
	}
	return DeltaTime;
}

//----------------------------------------------------------------------------------------------
void UpdateCallsPerSecond(int milliseconds)
{
	ThreadManagerSerial &tm = ThreadManagerSerial::getThreadManagerSerial();

	unsigned int eventCalls  = tm.getAndResetThreadCalls(INDEX_NONE);

	unsigned int PhysCalls = 0;
	unsigned int renderCalls = 0;
	unsigned int timerCalls = 0;
	unsigned int streamCalls = 0;

#ifdef USE_THREADS
	PhysCalls	= tm.getAndResetThreadCalls(threadPhysID);
	renderCalls = tm.getAndResetThreadCalls(threadRenderID);
	timerCalls	= tm.getAndResetThreadCalls(threadTimerID);
	streamCalls	= tm.getAndResetThreadCalls(threadStreamingID);
#else
	PhysCalls	= eventCalls;
	renderCalls = eventCalls;
	timerCalls	= eventCalls;
	streamCalls	= eventCalls;
#endif

	callsPerSecondCS.enter();

	eventCPS   = (eventCalls * 1000) / milliseconds;
	updateCPS  = (PhysCalls * 1000) / milliseconds;
	renderCPS  = (renderCalls * 1000) / milliseconds;
	timerCPS   = (timerCalls * 1000) / milliseconds;
	streamCPS  = (streamCalls * 1000) / milliseconds;
	
	callsPerSecondCS.leave();

// 	CWin32ThreadMonitor::SetCPS_Main(eventCPS);
// 	CWin32ThreadMonitor::SetCPS_Render(renderCPS);
// 	CWin32ThreadMonitor::SetCPS_Update(updateCPS);
// 	CWin32ThreadMonitor::SetCPS_Timer(updateCPS);
}

//----------------------------------------------------------------------------------------------
void ProcessUpdate(float DeltaTime)
{
	GRootProcess->LoopWindow(DeltaTime);
}

