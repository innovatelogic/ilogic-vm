#ifndef __gameinfomanager_h__
#define __gameinfomanager_h__

#ifdef WIN32
#pragma once
#endif

#include "../Foundation/Stdafxfoundation.h"

class GameInfoManager
{
public:
	GameInfoManager();
	virtual ~GameInfoManager();

	virtual void Update(float DeltaTime);

protected:
private:
};

#endif//__gameinfomanager_h__