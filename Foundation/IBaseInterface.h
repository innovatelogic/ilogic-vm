#ifndef __ibaseinterface_h__
#define __ibaseinterface_h__

#ifdef _WIN32
#pragma once
#endif

#include "StdafxFoundation.h"

class EXPORT IBaseInterface
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

#endif//__ibaseinterface_h__