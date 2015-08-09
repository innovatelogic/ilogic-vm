#ifndef __envsceneinfo_h__
#define __envsceneinfo_h__

#ifdef _WIN32
#pragma once
#endif

#include "RenderObject.h"

class RENDERSDK_API CComp_EnvSceneInfo : public CRenderObject
{
	DECLARE_CLASS_SIMPLE(CComp_EnvSceneInfo, CRenderObject);

public:
	CComp_EnvSceneInfo(const CObjectAbstract* pParent);
	CComp_EnvSceneInfo(const CComp_EnvSceneInfo & Source);
	virtual ~CComp_EnvSceneInfo();

public:
	bool			m_bFog;
	EFogMode		m_FogMode;
	float			m_fFogMin;
	float			m_fFogMax;
	float			m_fFogDensity;
	unsigned int	m_FogColor;
};

#endif//__envsceneinfo_h__