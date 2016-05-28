#include "MxsExport.h"
#include <strclass.h>
#include <ifnpub.h>

const Interface_ID I_MXS_UNIT_REPORTER = Interface_ID(0x67894321,0x91827364);

//! \brief A class that returns results of maxscript based unit tests. 
/*!	See the notes for MxsUnitResults more details. */
class IMxsUnitResults : public FPStaticInterface
{
	public:
	virtual int GetAssertFailCount() = 0;
	virtual int GetExceptionCount() = 0;
	virtual int GetMessageCount() = 0;
	
	virtual MSTR GetAssertFailure(int i) = 0;
	virtual MSTR GetExceptionFailure(int i) = 0;
	virtual MSTR GetMessage(int i) = 0;
	
	virtual MSTR GetUserData() = 0;
	
	virtual void Clear() = 0;
	// Note that the enum is local to this class, and not in the global scope. 
	enum
	{ 
		e_GetAssertFailCount, 
		e_GetExceptionCount, 
		e_GetMessageCount, 
		e_GetAssertFailure,
		e_GetExceptionFailure,
		e_GetMessage,
		e_GetUserData,
		e_Clear
	};
};






