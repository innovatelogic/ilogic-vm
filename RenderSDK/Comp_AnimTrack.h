#ifndef __comp_animtrack_h__
#define __comp_animtrack_h__

#ifdef _WIN32
#pragma once
#endif

#include "RenderObject.h"

class RENDERSDK_API Comp_AnimTrack : public CRenderObject
{
	DECLARE_CLASS_SIMPLE(Comp_AnimTrack, CRenderObject);

public:
	Comp_AnimTrack(const CObjectAbstract * Parent);
	Comp_AnimTrack(const Comp_AnimTrack & Source);
	virtual ~Comp_AnimTrack();

	virtual bool	LoadFromFile(const char* filename);

public:
	class D3DSkeleton* pSkeleton;
};

#endif//__comp_animtrack_h__