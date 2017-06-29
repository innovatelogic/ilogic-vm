#ifndef __comp_materialeffectui_h__
#define __comp_materialeffectui_h__

#ifdef WIN32
#pragma once
#endif

#include "Comp_MaterialBaseEffect.h"

class RENDERSDK_API Comp_MaterialEffectUI : public Comp_MaterialBaseEffect
{
	DECLARE_CLASS_SIMPLE(Comp_MaterialEffectUI, Comp_MaterialBaseEffect);

public:
	Comp_MaterialEffectUI(const CObjectAbstract * Parent);
	Comp_MaterialEffectUI(const Comp_MaterialEffectUI & Source);
	virtual ~Comp_MaterialEffectUI();

	virtual void		Initialize();
	virtual bool		LoadDiffuseMap(const char *filename, bool ImmidiateLoad = true);
	virtual bool		LoadDiffuseMapW(const wchar_t *filename);

	virtual Vector4f	GetUV() const;
	virtual void		SetUV(const Vector4f &UV);

	virtual void		SetDiffuseReference(const std::string &StrRef);
	virtual std::string	GetDiffuseReference() const;

	void				SetRenderTarget(class Comp_RenderTarget *pRT);

	bool	GetInfo(unsigned int &Width, unsigned int &Height) const;
	bool	GetDIB(BYTE** pData, bool bResample = false, unsigned int NewWidth = 0, unsigned int NewHeight = 0) const;

protected:
private:
public:
    oes::d3d::MaterialEffectUI	*pMaterialEffectUI;
};

#endif//__comp_materialeffectui_h__