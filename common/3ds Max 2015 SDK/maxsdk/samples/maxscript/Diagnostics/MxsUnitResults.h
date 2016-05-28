#pragma once
#include "IMxsUnitReporter.h"

/*! Class that reports the summary of maxscript based assert messages
	There is a need for external code to access the results from
	the maxscript unit tests. This is an interface that can be used
	for external code to access that without having a library dependency or
	other strong dependencies. 
	Note that the same results for the maxscript based unit tests can be 
	access from maxscript by the AssertReporter global struct. That struct
	provides more diagnostic information that this interface. But it is only 
	accessible via maxscript. This interface is exposed via maxscript and also
	to C++ clients. */
class MxsUnitResults : public IMxsUnitResults
{
public:
	//! Gets the number of maxscript based assert failures. 
	virtual int GetAssertFailCount();
	//! Gets the number of exceptions that were caught.
	virtual int GetExceptionCount();
	//! Gets the number of messages that were logged with the manager.
	virtual int GetMessageCount();

	virtual MSTR GetAssertFailure(int i);
	virtual MSTR GetExceptionFailure(int i);
	virtual MSTR GetMessage(int i);

	virtual MSTR GetUserData();

	virtual void Clear();

	DECLARE_DESCRIPTOR(MxsUnitResults)
	BEGIN_FUNCTION_MAP
		FN_0(IMxsUnitResults::e_GetAssertFailCount, TYPE_INT, GetAssertFailCount)
		FN_0(IMxsUnitResults::e_GetExceptionCount, TYPE_INT, GetExceptionCount)
		FN_0(IMxsUnitResults::e_GetMessageCount, TYPE_INT, GetMessageCount)

		FN_1(IMxsUnitResults::e_GetAssertFailure, TYPE_TSTR, GetAssertFailure, TYPE_INDEX)
		FN_1(IMxsUnitResults::e_GetExceptionFailure, TYPE_TSTR, GetExceptionFailure, TYPE_INDEX)
		FN_1(IMxsUnitResults::e_GetMessage, TYPE_TSTR, GetMessage, TYPE_INDEX)

		FN_0(IMxsUnitResults::e_GetUserData, TYPE_TSTR, GetUserData)

		VFN_0(IMxsUnitResults::e_Clear, Clear)
	END_FUNCTION_MAP

};