#ifndef __launcherstdafx_h__
#define __launcherstdafx_h__

#ifdef WIN32
#pragma once

#if defined(DEBUG) | defined(_DEBUG)
#pragma comment(lib, "../Lib/Foundation_d.lib")
#pragma comment(lib, "../Lib/CoreSDK_d.lib")
#pragma comment(lib, "../Lib/RenderSDK_d.lib")
#pragma comment(lib, "../Lib/D3DDrv_d.lib")
#else
#pragma comment(lib, "../Lib/Foundation.lib")
#pragma comment(lib, "../Lib/CoreSDK.lib")
#pragma comment(lib, "../Lib/RenderSDK.lib")
#pragma comment(lib, "../Lib/D3DDrv.lib")
#endif
#endif	//WIN32

#if defined(DEBUG) | defined(_DEBUG)
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


#include "..\..\CoreSDK\coresdkafx.h"
#include "WindowManager.h"

#include <Commdlg.h>
#include <conio.h>
#include <time.h>

#define APP_NAME TEXT("Game.Prototype-01 [v0.20]")
#define STR_COPYRIGHT "© Innovatelogic 2013"

#define USE_THREADS
#define THREAD_SPAWN_COUNT 4
#define CPS_FREQUENCY_SECONDS  2.f
#define CPS_FREQUENCY_MILLISECONDS  2000

#define IE(Key, State, Shift, Ctrl) EventInput(Key, State, Shift, Ctrl)

#define WM_USER_DELETE_ALL					WM_APP+0
#define WM_USER_FILL_PROPS					WM_APP+1
#define WM_USER_UPDATE_PROPS				WM_APP+2

#define IDM_FIRSTCHILD   50000 // used in structure when creating mdi client area for the main frame 

#endif//__launcherstdafx_h__