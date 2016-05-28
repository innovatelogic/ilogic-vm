#include "MxsUnitResults.h"
#include "MxsUnitReporter.h"
#include <maxapi.h>

int MxsUnitResults::GetAssertFailCount()
{
	return (int)(MxsUnitReporter::GetInstance()->GetAssertFailures().length());
}

int MxsUnitResults::GetExceptionCount()
{
	return (int)(MxsUnitReporter::GetInstance()->GetExceptionFailures().length());
}

int MxsUnitResults::GetMessageCount()
{
	return (int)(MxsUnitReporter::GetInstance()->GetMessages().length());
}

MSTR MxsUnitResults::GetAssertFailure(int i)
{
	return MxsUnitReporter::GetInstance()->GetAssertFailures()[i];
}

MSTR MxsUnitResults::GetExceptionFailure(int i)
{
	return MxsUnitReporter::GetInstance()->GetExceptionFailures()[i];
}

MSTR MxsUnitResults::GetMessage(int i)
{
	return MxsUnitReporter::GetInstance()->GetMessages()[i];
}

MSTR MxsUnitResults::GetUserData()
{
	return MxsUnitReporter::GetInstance()->GetUserData();
}

void MxsUnitResults::Clear()
{
	MxsUnitReporter::GetInstance()->Clear();
}

static MxsUnitResults fp_MxsUnitResults_inst(
	I_MXS_UNIT_REPORTER,   // Interface_ID
	_T("MxsUnitResults"), // Internal fixed name
	0,                     // Localized string resource ID (doesn't have one)
	NULL,                  // owning class descriptor
	FP_CORE,

	// Functions ----------------------------
	IMxsUnitResults::e_GetAssertFailCount, _T("GetAssertFailCount"), 0, TYPE_INT, 0,0,
	IMxsUnitResults::e_GetExceptionCount,  _T("GetExceptionCount"),  0, TYPE_INT, 0,0,
	IMxsUnitResults::e_GetMessageCount,    _T("GetMessageCount"),    0, TYPE_INT, 0,0,

	IMxsUnitResults::e_GetAssertFailure,    _T("GetAssertFailure"),    0, TYPE_TSTR, 0,1,
		_T("index"), 0, TYPE_INDEX,
	IMxsUnitResults::e_GetExceptionFailure, _T("GetExceptionFailure"), 0, TYPE_TSTR, 0,1,
		_T("index"), 0, TYPE_INDEX,
	IMxsUnitResults::e_GetMessage,          _T("GetMessage"),          0, TYPE_TSTR, 0,1,
		_T("index"), 0, TYPE_INDEX,

	IMxsUnitResults::e_GetUserData, _T("GetUserData"), 0, TYPE_TSTR, 0, 0,
	IMxsUnitResults::e_Clear,       _T("Clear"),       0, TYPE_VOID, 0, 0,
	p_end
);



