#ifndef __Refcount_h__
#define __Refcount_h__

#ifdef _WIN32
#pragma once
#endif

#include "dxstdafx.h"

class D3DDRIVER_API Refcount
{
public:
	Refcount();
	virtual ~Refcount();

	long AddRef();
	long Release(bool Force = false);

	bool IsClear() { return (m_nCount == 0); }

	long GetRefCount() const { return m_nCount; }

protected:
	virtual void DoRelease();

public:
	volatile long m_nCount;
};

#endif //_m_refcount_h_