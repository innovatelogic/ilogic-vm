#ifndef __gamerealminfo_h__
#define __gamerealminfo_h__

#ifdef WIN32
#pragma once
#endif

#include "..\Foundation\StdafxFoundation.h"
#include "game_types.h"

class CORESDK_API GameRealmInfo
{
public:
	GameRealmInfo();
	virtual ~GameRealmInfo();

	class GameStaticInfo*	GetDefaultInfo();

	virtual bool			Load(const std::string &filename);

	virtual void			SetCurrentLevelId(int Value) { CurrentLevelId = Value; }
	virtual int				GetCurrentLevelId() const { return CurrentLevelId; }

	virtual void			SetCurrentHP(float Value) { CurrentHP = Value; }	
	virtual float			GetCurrentHP() const { return CurrentHP; }

	virtual void			SetCurrentEnergy(float Value) { CurrentEnergy = Value; }	
	virtual float			GetCurrentEnergy() const { return CurrentEnergy; }

protected:
private:
	class GameStaticInfo * DefaultGameInfo;

	int		CurrentLevelId;
	float	CurrentHP;
	float	CurrentEnergy;

private:
	GameRealmInfo(const GameRealmInfo&);
};

#endif//__gamerealminfo_h