#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
GameRealmInfo::GameRealmInfo()
: DefaultGameInfo(NULL)
, CurrentLevelId(INDEX_NONE)
, CurrentHP(100.f)
, CurrentEnergy(100.f)
{
	SAFE_ALLOC(DefaultGameInfo, GameStaticInfo);
}

//----------------------------------------------------------------------------------------------
GameRealmInfo::~GameRealmInfo()
{
	DELETESINGLE(DefaultGameInfo);
}

//----------------------------------------------------------------------------------------------
GameStaticInfo* GameRealmInfo::GetDefaultInfo()
{
	return DefaultGameInfo;
}

//----------------------------------------------------------------------------------------------
bool GameRealmInfo::Load(const std::string & filename)
{
	return DefaultGameInfo->Load(filename);
}