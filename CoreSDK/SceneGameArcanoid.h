#ifndef __scenegamearcanoid_h__
#define __scenegamearcanoid_h__

#ifdef _WIN32
#pragma once
#endif

#include "Scene.h"

class CORESDK_API CSceneGameArcanoid : public CScene
{
	DECLARE_CLASS_SIMPLE(CSceneGameArcanoid, CScene);

public:
	CSceneGameArcanoid(const CObjectAbstract *pParent);
	virtual ~CSceneGameArcanoid();

	virtual void	Initialize();

	virtual void	DoDraw();

protected:
private:
	class CCamera 					 *m_pSceneCamera;
	class CPawnCueBall				 *m_pPlayerController;
};

#endif//__scenegamearcanoid_h__