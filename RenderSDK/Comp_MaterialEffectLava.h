#ifndef __comp_materialeffect_lava_h__
#define __comp_materialeffect_lava_h__


#ifdef WIN32
#pragma once
#endif

#include "Comp_MaterialEffect.h"

class RENDERSDK_API Comp_MaterialEffectLava : public Comp_MaterialBaseEffect
{
	DECLARE_CLASS_SIMPLE(Comp_MaterialEffectLava, Comp_MaterialBaseEffect);

public:
	Comp_MaterialEffectLava(const CObjectAbstract * Parent);
	Comp_MaterialEffectLava(const Comp_MaterialEffectLava & Source);
	virtual ~Comp_MaterialEffectLava();

	virtual void	Initialize();

	virtual bool	LoadDiffuseMap(const char *filename, bool ImmidiateLoad = true);
	virtual bool	LoadNormalMap(const char *filename, bool ImmidiateLoad = true);
	virtual bool	LoadHeightMap(const char *filename, bool ImmidiateLoad = true);

protected:
public:
    oes::d3d::MaterialEffectLava	*pMaterialEffectLava;
};

#endif//__comp_materialeffect_lava_h__