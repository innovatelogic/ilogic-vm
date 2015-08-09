#ifndef __scene_h__
#define __scene_h__

#ifdef WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class CORESDK_API CScene : 
	public ActorAllocator, public IDrawInterface
{
	DECLARE_CLASS_SIMPLE(CScene, ActorAllocator);

public:
	CScene(const CObjectAbstract *Parent);
	CScene(const CScene &Source);
	virtual ~CScene();

	/** 
	 *  Initialization function. calls once per object create or deserialize
	 *	ATTENTION! Do not call it twice 
	 */
	virtual void	Initialize();

	virtual void	DoDraw();

protected:
private:
	class CComp_EnvSceneInfo *m_pEnvSceneInfo;
};

#endif//__scene_h__