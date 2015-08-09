#ifndef __restrictedvolume_h__
#define __restrictedvolume_h__

#ifdef WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class CORESDK_API CRestrictedVolume : public ActorAllocator, IDrawInterface
{
	DECLARE_CLASS_SIMPLE(CRestrictedVolume, ActorAllocator);

public:
	CRestrictedVolume(const CObjectAbstract *pParent);
	virtual ~CRestrictedVolume();

	virtual void	OnPropertyChanged(const char *pPropertyName);

	virtual void	Initialize();
	virtual void	DoDraw();

private:
	bool	InitPhysics();

	class CRestrictedVolumeCollisionController	*m_pCollisionObject;
	bool	m_bSides[6];
};

#endif//__restrictedvolume_h__