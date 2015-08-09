#ifndef __basemodificator_h__
#define __basemodificator_h__

#ifdef _WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class BaseModificator : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(BaseModificator, ActorAllocator);

public:
	BaseModificator(const CObjectAbstract * Parent);
	BaseModificator(const BaseModificator& Source);
	virtual ~BaseModificator();

protected:
private:
};

#endif//__basemodificator_h__