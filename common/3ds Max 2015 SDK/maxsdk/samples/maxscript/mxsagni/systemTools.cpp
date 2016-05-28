#include <maxscript/maxscript.h>
#include <maxscript/foundation/numbers.h>
#include <maxscript/foundation/strings.h>
#include "MXSAgni.h"

#ifdef ScripterExport
#undef ScripterExport
#endif
#define ScripterExport __declspec( dllexport )

// ============================================================================

#include <maxscript\macros\define_external_functions.h>
#	include "namedefs.h"

#include <maxscript\macros\define_instantiation_functions.h>
#	include "systemTools_wraps.h"
// -----------------------------------------------------------------------------------------


Value* systemTools_IsDebugging_cf (Value** arg_list, int count)
{
	check_arg_count (IsDebugging, 0, count);
	return ( IsDebugging() ? &true_value : &false_value );
}

Value* systemTools_DebugPrint_cf(Value** arg_list, int count)
{
	check_arg_count(DebugPrint, 1, count);
	const MCHAR* message = arg_list[0]->to_string();
	DebugPrint(_T("%s\n"), message);
	return &ok;
}

Value* systemTools_NumberOfProcessors_cf (Value** arg_list, int count)
{
	check_arg_count (NumberOfProcessors, 0, count);
	return Integer::intern (NumberOfProcessors());
}

Value* systemTools_IsWindows9x_cf (Value** arg_list, int count)
{
	check_arg_count (IsWindows9x, 0, count);
	return ( IsWindows9x() ? &true_value : &false_value );
}

Value* systemTools_IsWindows98or2000_cf (Value** arg_list, int count)
{
	check_arg_count (IsWindows98or2000, 0, count);
	return ( IsWindows98or2000() ? &true_value : &false_value );
}

Value* systemTools_GetScreenWidth_cf (Value** arg_list, int count)
{
	check_arg_count (GetScreenWidth, 0, count);
	return Integer::intern (GetScreenWidth());
}

Value* systemTools_GetScreenHeight_cf (Value** arg_list, int count)
{
	check_arg_count (GetScreenHeight, 0, count);
	return Integer::intern (GetScreenHeight());
}

Value*
systemTools_getEnvVar_cf(Value** arg_list, int count)
{
	// systemTools.getEnvVariable <string>
	check_arg_count(systemTools.getEnvVariable, 1, count);
	const TCHAR *varName = arg_list[0]->to_string();
	DWORD bufSize = GetEnvironmentVariable(varName,NULL,0);
	if (bufSize == 0) return &undefined;
	TSTR varVal;
	varVal.Resize(bufSize+1);
	GetEnvironmentVariable(varName,varVal.dataForWrite(),bufSize+1);
	return new String(varVal);
}

Value*
systemTools_setEnvVar_cf(Value** arg_list, int count)
{
	// systemTools.setEnvVariable <string> <string>
	check_arg_count(systemTools.setEnvVariable, 2, count);
	const TCHAR *varName = arg_list[0]->to_string();
	const TCHAR *varVal = NULL;
	if (arg_list[1] != &undefined)
		varVal = arg_list[1]->to_string();
	BOOL res = SetEnvironmentVariable(varName,varVal);
	return bool_result(res);
}

Value*
	systemTools_getmaxstdio_cf(Value** arg_list, int count)
{
	// <int> systemTools.getmaxstdio()
	// Returns the number of simultaneously open files permitted at the stdio level (i.e. via fopen).
	check_arg_count(systemTools.getmaxstdio, 0, count);
	int maxstdio = _getmaxstdio();
	return_protected(Integer::intern(maxstdio));
}

Value*
	systemTools_setmaxstdio_cf(Value** arg_list, int count)
{
	// <int> systemTools.setmaxstdio <int newmax>
	// Sets a maximum for the number of simultaneously open files at the stdio level (i.e. via fopen).
	// The valid range for newmax is 512 to 2048.
	// Returns new maximum if successful; –1 otherwise. Fails if reducing max and have active file handle 
	// in one of the slots above newmax, or if memory allocation occurring when increasing maximum fails.
	// Note: due to a defect in the c runtime, it is currently unsafe to decrease the maximum. Attempts 
	// to decrease the maximum via this method will be a no-op, and -1 is returned.
	check_arg_count(systemTools.setmaxstdio, 1, count);
	int  new_maxstdio= arg_list[0]->to_int();
	range_check(new_maxstdio, 512, 2048, _T("systemTools.setmaxstdio arg"));
	// because of a c runtime bug, it is unsafe to decrease the max allowed handles.
	int old_maxstdio = _getmaxstdio();
	if (old_maxstdio > new_maxstdio)
		return_protected(Integer::intern(-1));
	int maxstdio = _setmaxstdio(new_maxstdio);
	return_protected(Integer::intern(maxstdio));
}
