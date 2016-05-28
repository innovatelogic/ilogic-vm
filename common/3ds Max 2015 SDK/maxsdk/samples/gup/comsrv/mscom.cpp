//-----------------------------------------------------------------------------
// --------------------
// File ....: mscom.cpp
// --------------------
// Author...: Gus J Grubba
// Date ....: October 1998
// Descr....: MS COM/DCOM Server for 3D Studio MAX
//
// History .: Oct, 1 1998 - Started
//            
//-----------------------------------------------------------------------------

//-- Include files
#include "stdafx.h"
#include "mscom.h"
#include "comsrv.h" //need to create an instance
#include "MaxRenderer.h"
#include <assert.h>

CComPtr<IUnknown> GUP_MSCOM::spOMAppUnk = NULL;

//-- Globals ------------------------------------------------------------------

HINSTANCE hInst = NULL;

extern bool StartServer	( HINSTANCE hInstance, HINSTANCE MaxhInstance, int registerCOM );
extern void StopServer	( );
extern LPCTSTR FindToken(LPCTSTR p1);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-- DLL Declaration

BOOL WINAPI DllMain(HINSTANCE hDLLInst, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
		 case DLL_PROCESS_ATTACH:
				MaxSDK::Util::UseLanguagePackLocale();
				hInst = hDLLInst;
            DisableThreadLibraryCalls(hInst);
				break;
		 case DLL_PROCESS_DETACH:
				hInst  = NULL;
				break;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
//-- Resource String Helper

TCHAR *GetString(int id) {
	static TCHAR buf[256];
	if (hInst)
		return LoadString(hInst, id, buf, _countof(buf)) ? buf : NULL;
	return NULL;
}

int             mClassDesc::IsPublic     ( )		{ return 1; }
void           *mClassDesc::Create       ( BOOL )	{ return Instance(); }
const TCHAR    *mClassDesc::ClassName    ( )		{ return _T("MaxRenderer COM Server"); }
SClass_ID       mClassDesc::SuperClassID ( )		{ return GUP_CLASS_ID; }
Class_ID        mClassDesc::ClassID      ( )		{ return Class_ID(470000002,0); }
const TCHAR    *mClassDesc::Category     ( )		{ return _T("Global Utility PlugIn"); }

GUP_MSCOM* mClassDesc::Instance()
{
	if(_instance == NULL)
	{
		_instance = new GUP_MSCOM;
		mcRef = 0;
	}
	mcRef++;
	return _instance;
}

void mClassDesc::RevokeInstance()
{
	mcRef--;//should be interlocked?
	if(mcRef == 0)
	{
		delete _instance;
		_instance = NULL;
	}
}

GUP_MSCOM* mClassDesc::_instance = NULL;
ULONG mClassDesc::mcRef = 0L;
static mClassDesc cdesc;

//-----------------------------------------------------------------------------
// Interface

DLLEXPORT const TCHAR * LibDescription ( )  { 
	return GetString(IDS_LIBDESCRIPTION); 
}

DLLEXPORT int LibNumberClasses ( ) { 
	return 1; 
}

DLLEXPORT ClassDesc *LibClassDesc(int i) {
	switch(i) {
		case  0: return &cdesc;	break;
		default: return 0;		break;
	}
}

DLLEXPORT ULONG LibVersion ( )  { 
	return ( VERSION_3DSMAX ); 
}

//-----------------------------------------------------------------------------
// #> GUP_MSCOM::GUP_MSCOM()

GUP_MSCOM::GUP_MSCOM( ) { 

}

GUP_MSCOM::~GUP_MSCOM ( ) {

}

void GUP_MSCOM::DeleteThis()
{
	//delete this;  // RK: 06/28/00
	// JH 9/3/03 let the class descriptor manage the singleton.
	mClassDesc::RevokeInstance();
}

//-----------------------------------------------------------------------------
// #> GUP_MSCOM::Start()

DWORD GUP_MSCOM::Start( ) {
	
	bool toregister		= false;
	bool tounregister	= false;


	LPTSTR lpCmdLine;
	lpCmdLine = GetCommandLine();

	LPCTSTR lpszToken = FindToken(lpCmdLine);
	while (lpszToken != NULL) {
		if (lstrcmpi(lpszToken,_REGISTERCOM)==0) {
			toregister = true;
			break;
		}
		if (lstrcmpi(lpszToken,_UNREGISTERCOM)==0) {
			tounregister = true;
			break;
		}
		if (lstrcmpi(lpszToken,_T("Embedding"))==0) {
			_Module.m_bOLEStart = true;
			break;
		}
		lpszToken = FindToken(lpszToken);
	}

	//-----------------------------------------------------
	//-- If not registered

	if (!IsCOMRegistered()) {

		//-- We are registering ourselves
		
		if (toregister) {
			StartServer(hInst,MaxInst(),2);
			return GUPRESULT_ABORT;
		}
		
		//-- Not registered and not registering, no need to
		//   stay around.


		return GUPRESULT_NOKEEP;
	
	//-----------------------------------------------------
	//-- If registered
	
	} else {

		//-- Unregistering COM/DCOM server

		if (tounregister) {
			StartServer(hInst,MaxInst(),1);
			return GUPRESULT_ABORT;
		}
		else if(toregister)
			return GUPRESULT_ABORT;

	}

	//-----------------------------------------------------
	//-- Normal Operation
	StartServer(hInst,MaxInst(),0);	

	return GUPRESULT_KEEP;
}

//-----------------------------------------------------------------------------
// #> GUP_MSCOM::Stop()

void GUP_MSCOM::Stop( )
{
	if (IsCOMRegistered())
	{
		StopServer();
	}
}

//-----------------------------------------------------------------------------
// #> GUP_MSCOM::Control()

DWORD_PTR GUP_MSCOM::Control( DWORD parameter ) {
	switch (parameter) {
		case 0:	return (DWORD_PTR)IsCOMRegistered();
		case 1:	return (DWORD_PTR)RegisterCOM();
		case 2:	return (DWORD_PTR)UnRegisterCOM();
		case 4: return (DWORD_PTR)nullptr;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// #> GUP_MSCOM::IsCOMRegistered()

bool GUP_MSCOM::IsCOMRegistered( ) {
	TCHAR szKeyName[256];
	TCHAR clsidApp[] = {_T("{4AD72E6E-5A4B-11D2-91CB-0060081C257E}")};
	_stprintf(szKeyName,_T("CLSID\\%s"),clsidApp);
	HKEY	hKey;
	long retVal = RegOpenKeyEx(HKEY_CLASSES_ROOT,szKeyName,0,KEY_READ,&hKey);
	if (retVal == ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// #> GUP_MSCOM::RegisterCOM()

bool GUP_MSCOM::RegisterCOM( ) {
	if (IsCOMRegistered())
		return false;
	bool res = StartServer(hInst,MaxInst(),2);
	return res;
}

//-----------------------------------------------------------------------------
// #> GUP_MSCOM::UnRegisterCOM()

bool GUP_MSCOM::UnRegisterCOM( ) {
	bool res = StartServer(hInst,MaxInst(),1);
	return res;
}

IOResult GUP_MSCOM::Save( ISave *isave )
{
	return IO_OK;
}

IOResult GUP_MSCOM::Load( ILoad *iload )
{
	return IO_OK;
}


//-- EOF: mscom.cpp -----------------------------------------------------------
