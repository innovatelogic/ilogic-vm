#include "expstdafx.h"

#pragma comment( lib, "../common/SDK/bullet/bullet-2.81-rev2613/lib/LinearMath_vs2010_debug.lib")

extern ClassDesc2* GetExporterMAXDesc();
extern ClassDesc2* GetUtilityMAXDesc();

HINSTANCE hInstance;
//int	controlsInit = FALSE;


bool	bExportSkeleton = true;
bool	bExportModel = true;
bool	bExportAnim = true;
bool	bExportCollision = true;

//----------------------------------------------------------------------------------
TCHAR* GetString(int id)
{
	static TCHAR buf[256];

	if (hInstance){
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	}
	return NULL;
}

//----------------------------------------------------------------------------------
__declspec( dllexport ) const TCHAR* LibDescription()
{
	return _T("OpenES exporter");//GetString(IDS_LIBDESCRIPTION);
}

//----------------------------------------------------------------------------------
__declspec( dllexport ) int LibNumberClasses()
{
	return 2;
}

//----------------------------------------------------------------------------------
__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
	switch(i) 
	{
		case 0: {
			return GetExporterMAXDesc(); 
				}break;
		case 1: {
				return GetUtilityMAXDesc(); 
				}break;
		default: return 0;
	}
	return 0;
}

//----------------------------------------------------------------------------------
__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

//----------------------------------------------------------------------------------
// Let the plug-in register itself for deferred loading
__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 0; // do not allow deferred load
}

//----------------------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved)
{
	hInstance = hinstDLL;				// Hang on to this DLL's instance handle.

	//if (!controlsInit) 
	{
		//controlsInit = TRUE;
		InitCommonControls();			// Initialize Win95 controls
	}

	return (TRUE);
}