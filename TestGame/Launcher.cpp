// TestGame.cpp : Defines the entry point for the DLL application.
//

#include "gamestdafx.h"

namespace nmLauncher
{
	/** init process instance */
	typedef std::map<std::string, int> TIconRefs;

	/** root process */
	CCoreSDK		*pCoreSDK = NULL;
	CWindowManager	*pWindowManager = NULL;

	float GAppTimePassed = 0.f;
}
using namespace nmLauncher;

void	StopModule();

//----------------------------------------------------------------------------------------------
void TimerFunction()
{
	static LARGE_INTEGER previousTime;
	static LARGE_INTEGER freq;
	static bool init_timegettime = false;

	if (!init_timegettime)
	{
		::QueryPerformanceFrequency(&freq);
		::QueryPerformanceCounter(&previousTime);
		init_timegettime = true;
	}

	LARGE_INTEGER currentTime;
	::QueryPerformanceCounter(&currentTime);

	unsigned long long elapsedTime = currentTime.QuadPart - previousTime.QuadPart;
	previousTime = currentTime;

	GAppTimePassed += (float)(elapsedTime)/(freq.QuadPart);
}

//----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrInst, LPSTR sCmdStr, int nCmdShow)
{
	try
	{
		ValueParser CommandLineStr("");
		CommandLineStr.ParseCommandLine(sCmdStr);

		pCoreSDK = new CCoreSDK(CommandLineStr, StopModule);

		pWindowManager = new CWindowManager(pCoreSDK);
		pWindowManager->Initialize();

		//srand(CalculateTimeDeltaMilliseconds());

		pWindowManager->OutWindowMessageLog(std::string("Start Loading..."));

		//ShowWindow(pWindowManager->GetWindowManagerHWND(), SW_SHOWMAXIMIZED);

		// logging
		pWindowManager->OutWindowMessageLog(std::string("Initialize Core"));

		RECT rectWindows;
		GetClientRect(pWindowManager->GetWindowManagerHWND(), &rectWindows);

		unsigned int wndWidth = rectWindows.right - rectWindows.left;
		unsigned int wndHeight = rectWindows.bottom - rectWindows.top;

		pCoreSDK->Initialize(pWindowManager->GetWindowManagerHWND(), wndWidth, wndHeight);

		pCoreSDK->SetPause(false);

		// logging
		pWindowManager->OutWindowMessageLog(std::string("Initialization completed"));
		pWindowManager->CloseSplash();
	}
	catch (const std::exception &e)
	{
		_cputs("Core Initialization: Exception raised:\n");
		_cputs(e.what());
		return EXIT_FAILURE;
	}
	
	pCoreSDK->GetUISceneManager()->Command_LoadScene("SceneLogin01.xml");
	pCoreSDK->GetUISceneManager()->Command_InitializeScene("SceneLogin01.xml");

	// main loop
	while (pWindowManager->PeekMessageWindow())
	{
		static float fLastTimeUpdate = 0.f;

		TimerFunction();
		
		float fDeltaTime = GAppTimePassed - fLastTimeUpdate;

		fLastTimeUpdate = GAppTimePassed;
		
		pCoreSDK->ProcessUpdate(fDeltaTime);

		pCoreSDK->Draw(); // draw new frame to back buffer

		pCoreSDK->Render(0, 0); // Render & change buffer

		pCoreSDK->SimulatePhysics(fDeltaTime);
	}

	delete pWindowManager;
	delete pCoreSDK;

	NObjectFactory::TClassFactory * classFactory = GetClassFactoryA();

	classFactory->m_MapGenerator.clear();
	delete classFactory;

	return 0;
}

//----------------------------------------------------------------------------------------------
void StopModule()
{
	pWindowManager->QuitApplication();
}