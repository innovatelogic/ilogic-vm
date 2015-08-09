#ifndef __gamestaticinfo_h__
#define __gamestaticinfo_h__

#ifdef WIN32
#pragma once
#endif

#include "..\Foundation\StdafxFoundation.h"
#include "game_types.h"

class CORESDK_API GameStaticInfo
{
	typedef std::map<int, std::string> TMapIntString;

public:
	GameStaticInfo();
	virtual ~GameStaticInfo();

	virtual bool			Load(const std::string& filename);

	virtual std::string		GetLevelNameById(int Id);

protected:
private:
	TMapIntString	IdLevelMap;

private:
	GameStaticInfo(const GameStaticInfo & Source);
};

#endif//__gamestaticinfo_h__