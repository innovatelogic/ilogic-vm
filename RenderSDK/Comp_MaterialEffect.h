#ifndef __comp_materialeffect_h__
#define __comp_materialeffect_h__

#ifdef _WIN32
#pragma once
#endif

#include "Comp_MaterialBaseEffect.h"

class RENDERSDK_API Comp_MaterialEffect : public Comp_MaterialBaseEffect
{
	DECLARE_CLASS_SIMPLE(Comp_MaterialEffect, Comp_MaterialBaseEffect);

public:
	Comp_MaterialEffect(const CObjectAbstract * Parent);
	virtual ~Comp_MaterialEffect();

	virtual void			Initialize();

	/*const*/ oes::d3d::MaterialEffect*	GetMaterialEffect() const;

	virtual bool			LoadDiffuseMap(const char *filename, bool ImmidiateLoad = true);
	virtual bool			LoadNormalMap(const char *filename, bool ImmidiateLoad = true);
	virtual bool			LoadHeightMap(const char *filename, bool ImmidiateLoad = true);

	virtual Vector4f		GetUV() const;
	virtual void			SetUV(const Vector4f & UV);

	virtual void			SetDiffuseReference(const std::string & StrRef);
	virtual std::string		GetDiffuseReference() const;

	void					SetAlphaTestEnable(bool flag);
	bool					GetAlphaTestEnable();

	void					SetTransparent(float value);
	float					GetTransparent();

protected:
private:
public:
    oes::d3d::MaterialEffect	*pMaterialEffect;
};

#endif//__comp_materialeffect_h__