//
// Copyright 2014 Autodesk, Inc.  All rights reserved. 
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc. and
// are protected under applicable copyright and trade secret law.  They may
// not be disclosed to, copied or used by any third party without the prior
// written consent of Autodesk, Inc.
//
//

#include "activeshadefragmentMain.h"
#include "Resource.h"


HINSTANCE hInstance;
/** public functions **/
BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) {
	if( fdwReason == DLL_PROCESS_ATTACH )
	{
		MaxSDK::Util::UseLanguagePackLocale();
		hInstance = hinstDLL;
		DisableThreadLibraryCalls(hInstance);
	}
	return(TRUE);
}

//------------------------------------------------------
// This is the interface to Max:
//------------------------------------------------------

// Return version so can detect obsolete DLLs
__declspec( dllexport ) ULONG LibVersion() { return VERSION_3DSMAX; }

TCHAR *GetString(int id)
{
	static TCHAR buf[256];
	if(hInstance)
		return LoadString(hInstance, id, buf, _countof(buf)) ? buf : NULL;

	return NULL;
}

__declspec( dllexport ) const TCHAR * LibDescription() { return GetString(IDS_LIBDESCRIPTION); }

// MUST CHANGE THIS NUMBER WHEN ADD NEW CLASS
__declspec( dllexport ) int LibNumberClasses() 
{
	return 1;
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i) 
{
	switch(i) {
	case 0: return GetActiveShadeFragmentDesc();
	default: return 0;
	}
}





