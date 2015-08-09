#ifndef __launcherstdafx_h__
#define __launcherstdafx_h__

#ifdef WIN32
#pragma once

#define USE_WTL

#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define NOMINMAX

//#define WINVER		0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100

#define APP_NAME TEXT("OpenES [v0.21]")
#define STR_COPYRIGHT "© Innovatelogic 2014"
#define STR_SOURCE_COPYRIGHT "Source Code licence: GNU GPLv2"

#define USE_THREADS
#define THREAD_SPAWN_COUNT 4
#define CPS_FREQUENCY_SECONDS  2.f
#define CPS_FREQUENCY_MILLISECONDS  2000

#define IE(Key, State, Shift, Ctrl) EventInput(Key, State, Shift, Ctrl)

#define WM_USER_DELETE_ALL					WM_APP+0
#define WM_USER_FILL_PROPS					WM_APP+1
#define WM_USER_UPDATE_PROPS				WM_APP+2
#define WM_USER_PROPS_FILLED				WM_APP+3

#define IDM_FIRSTCHILD   50000 // used in structure when creating mdi client area for the main frame 

#define ID_BUTTON_NEWPROJECT 1045
#define ID_BUTTON_ADDNEWITEM 1046
#define ID_BUTTON_OPENFILE	 1047	 
#define ID_BUTTON_SAVEFILE   1048

static const int MinWndSize = 40;

#if defined(DEBUG) | defined(_DEBUG)
//#pragma comment(lib, "../Lib/Foundation_d.lib")
//#pragma comment(lib, "../Lib/CoreSDK_d.lib")
//#pragma comment(lib, "../Lib/RenderSDK_d.lib")
//#pragma comment(lib, "../Lib/D3DDrv_d.lib")
#else
//#pragma comment(lib, "../Lib/Foundation.lib")
//#pragma comment(lib, "../Lib/CoreSDK.lib")
//#pragma comment(lib, "../Lib/RenderSDK.lib")
//#pragma comment(lib, "../Lib/D3DDrv.lib")
#endif
#endif	//WIN32

/*#if defined(DEBUG) | defined(_DEBUG)
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/BulletCollision_vs2010_debug.lib")
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/BulletDynamics_vs2010_debug.lib")
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/BulletSoftBody_vs2010_debug.lib")
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/ConvexDecomposition_vs2010_debug.lib")
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/HACD_vs2010_debug.lib")
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/BulletDynamics_vs2010_debug.lib")
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/LinearMath_vs2010_debug.lib")
#else
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/BulletCollision_vs2010.lib")
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/BulletDynamics_vs2010.lib")
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/BulletSoftBody_vs2010.lib")
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/ConvexDecomposition_vs2010.lib")
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/HACD_vs2010.lib")
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/BulletDynamics_vs2010.lib")
#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/LinearMath_vs2010.lib")
#endif
*/

#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>

#ifdef USE_WTL

#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlmisc.h>
#include <atlsplit.h>
#include <atlctrlx.h>
#include <atlctrlw.h>
#elif

#endif//USE_WTL


#include <mmsystem.h>
#include <commctrl.h>

#include <stdlib.h>
#include <crtdbg.h>

#include <Commdlg.h>
#include <conio.h>
#include <time.h>

#include "..\..\CoreSDK\coresdkafx.h"
#include "GameLoop.h"
#include "Win32ObjectBrowserWidget.h"
#include "Win32ObjectToolbox.h"
#include "Win32ThreadMonitor.h"
#include "Win32InputBox.h"
#include "Win32ImageList.h"
#include "ThreadManagerSerial.h"
#include "CriticalSection.h"
#include "ThreadManager.h"
#include "resource.h"
#include "ImageBitmap.h"

#ifdef USE_WTL
#include "WTLPropertyGrid.h"
#include "WTLPlacementWidget.h"
#include "WTLItemsTree.h"
#include "PanePropertyContainer.h"
#include "PaneTreeContainer.h"
#include "PaneViewContrainer.h"
#include "PaneTreeView.h"
#include "PaneListView.h"
#include "PaneViewAssetContainer.h"
#include "MainFrame.h"
#include "AssetBrowser.h"
#endif//USE_WTL

#endif//__launcherstdafx_h__