#ifndef _m_refcount_h_
#define _m_refcount_h_

#ifdef _WIN32
  #pragma once
#endif

#include "platform_specific.h"

class EXPORT m_refcount
{

public:
	m_refcount();
	virtual ~m_refcount();

	unsigned int add_ref();
	unsigned int release();

protected:
    unsigned int _count;
};

#endif //_m_refcount_h_