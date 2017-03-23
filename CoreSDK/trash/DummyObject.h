#ifndef __dummyobject_h__
#define __dummyobject_h__

#ifndef WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class CORESDK_API DummyObject : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(DummyObject, ActorAllocator);

public:
	DummyObject(const CObjectAbstract * Parent);
	~DummyObject();

	virtual void		DoDraw();

protected:
private:
};

#endif//__dummyobject_h__


