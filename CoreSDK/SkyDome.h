#ifndef __skydome_h__
#define __skydome_h__

#ifdef _WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class CORESDK_API SkyDome : public ActorAllocator, public IDrawInterface
{
	DECLARE_CLASS_SIMPLE(SkyDome, ActorAllocator);

public:
	// Constructor/Destructor
	SkyDome(const CObjectAbstract *pParent);
	virtual ~SkyDome();

	virtual void			Initialize();
	virtual void			OnPropertyChanged(const char* PropertyName);

	virtual unsigned int	GetDetail() { return Detail; }
	virtual void			SetDetail(unsigned int detail) { Detail = detail; }

	virtual void			SetDiffuseTex(std::string& ResourceTag) { DiffuseTex = ResourceTag;	}
	virtual std::string		GetDiffuseTex() const { return DiffuseTex;}

	virtual void			SetColor(unsigned int color) { Color = color; }
	virtual unsigned int	GetColor() const { return Color; }

	virtual void			DoDraw();

protected:
	virtual void			DoRebuildMesh();

private:
	unsigned int			Detail;
	std::string				DiffuseTex;
	unsigned int			Color;

	class Comp_StaticMesh		*m_pMeshComponent;
	class Comp_MaterialEffect	*m_pMaterialEffect;
};

#endif//__skydome_h__