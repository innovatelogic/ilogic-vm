#ifndef __comp_materialbaseeffect_h__
#define __comp_materialbaseeffect_h__

#ifdef WIN32
#pragma once
#endif

#include "RenderObject.h"

class RENDERSDK_API Comp_MaterialBaseEffect : public CRenderObject
{
	DECLARE_CLASS_SIMPLE(Comp_MaterialBaseEffect, CRenderObject);

public:
	Comp_MaterialBaseEffect(const CObjectAbstract * Parent);
	Comp_MaterialBaseEffect(const Comp_MaterialBaseEffect & Source);
	virtual ~Comp_MaterialBaseEffect();

	virtual void			OnPropertyChanged(const char* PropertyName);

	virtual void			Initialize();

	virtual void			SetReference(const char* Ref) { XRef = Ref; }
	virtual const char*		GetReference() const { return XRef.c_str(); }

	virtual void			SetTehnique(const char *tehnique);
	virtual std::string		GetTehnique();

	virtual unsigned int	BeginEffect() const;
	virtual void			EndEffect() const;

	virtual void			SetMatrixParam(const char *ParamName, const Matrix & Value);
	virtual void			SetVectorParam(const char *ParamName, const Vector & Value);
	virtual void			SetSampleTexture(const char *ParamName, const class Texture2D * Value);
	virtual void			SetScalarValue(const char *ParamName, float Value);

	class BaseMaterial*		GetPrtEffect() const;

	void					ReloadEffect();

protected:
	virtual	void			DoReloadEffect();
	void					SetBaseEffectPtr(/*const*/ class BaseMaterial * pEffect);
	
private:
	std::string				XRef;
	std::string				NameTehnique;
	class BaseMaterial		*pMaterialBaseEffect;
};

#endif//__comp_materialbaseeffect_h__