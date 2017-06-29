#ifndef __comp_materialeffectfont_h__
#define __comp_materialeffectfont_h__

#ifdef WIN32
#pragma once
#endif

#include "Comp_MaterialBaseEffect.h"

class RENDERSDK_API CComp_MaterialEffectFont : public Comp_MaterialBaseEffect
{
	DECLARE_CLASS_SIMPLE(CComp_MaterialEffectFont, Comp_MaterialBaseEffect);

public:
	CComp_MaterialEffectFont(const CObjectAbstract *pParent);
	CComp_MaterialEffectFont(const CComp_MaterialEffectFont &Source);
	virtual ~CComp_MaterialEffectFont();

	virtual void	Initialize();

private:
    oes::d3d::CMaterialEffectFont *m_pMaterialEffect;
};

#endif//__comp_materialeffectfont_h__
