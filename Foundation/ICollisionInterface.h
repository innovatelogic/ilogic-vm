#ifndef __icollisioninterface_h__
#define __icollisioninterface_h__

#ifdef _WIN32
#pragma once
#endif

#include "StdafxFoundation.h"

class EXPORT ICollisionInterface
{
public:
	virtual void Enable(bool bFlag) = 0;

	virtual void SetWorldPos(const Matrix &matrix) = 0;

protected:
private:
};

#endif//__icollisioninterface_h__