#ifndef __polygonimage_h__
#define __polygonimage_h__

#ifdef _WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class CORESDK_API CPolygonImage : public ActorAllocator, public IDrawInterface
{
	DECLARE_CLASS_SIMPLE(CPolygonImage, ActorAllocator);

public:
	// Constructor/Destructor
	CPolygonImage(const CObjectAbstract * Parent);
	virtual ~CPolygonImage();

	virtual void	Initialize();
	virtual void	OnPropertyChanged(const char* PropertyName);

	virtual void	DoDraw();

	virtual bool	ProcessController(const MouseMoveInputData &InputData);

protected:
	virtual void	DoRebuildMesh();

private:
	std::string					m_sDiffuseTex;
	class Comp_StaticMesh		*m_pMeshComponent;
	class Comp_MaterialEffect	*m_pMaterialEffect;
};

#endif//__polygonimage_h__