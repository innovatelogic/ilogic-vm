#pragma once

#include "OEMBase.h"

class COMMON_BASE_EXPORT IBaseInterface
{
public:
	/** 
	 * Calls from Property class to signalize that it have changed.
	 * 
	 * @param PropertyName [in] current property name have changed.
	 */
	virtual bool PrePropertyChangeIntf(const char *PropertyName) = 0;

	/** 
	 * Calls from Property class to signalize that it have change.
	 * 
	 * @param PropertyName [in] current property name have change.
	 */
	virtual void OnPropertyChangedIntf(const char *PropertyName) = 0;
};