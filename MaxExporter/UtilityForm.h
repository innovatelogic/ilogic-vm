#ifndef __UtilityForm_h__
#define __UtilityForm_h__

#ifdef _WIN32
#pragma once
#endif

#include "Max.h"

#define UTILITYMAX_CLASS_ID	Class_ID(0x3d3d2d84, 0x4e6e6224)

extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;

class UtilityForm : public UtilityObj
{
public:
	UtilityForm();
	virtual ~UtilityForm();

	void BeginEditParams(class Interface *_ip, IUtil *iu);
	void EndEditParams(class Interface *_ip, IUtil *iu);
	void DeleteThis();

	HWND hPanel;
	Interface * ip;

	static UtilityForm*			pUtility;                 // static pointer to self

protected:
private:
};


//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
class UtilityMAXClassDesc : public ClassDesc2 
{
public:
	int 			            IsPublic()                          { return TRUE; }
	void*			            Create(BOOL loading = FALSE)        { return new UtilityForm(); }
	const TCHAR*				ClassName()                         { return _T("UtilityForm"); } //GetString(IDS_CLASS_NAME);
	SClass_ID					SuperClassID()                      { return UTILITY_CLASS_ID; } //
	Class_ID					ClassID()                           { return UTILITYMAX_CLASS_ID; }
	const TCHAR*				Category()                          { return _T("OpenES"); }
	const TCHAR*				InternalName()                      { return _T("OpenES UtilityMAX"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE					HInstance()                         { return hInstance; }			// returns owning module handle
};

#endif//__UtilityForm_h__