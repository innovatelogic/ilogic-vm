#ifndef __comp_materialbutton_h__
#define __comp_materialbutton_h__

#ifdef WIN32
#pragma once
#endif

#include "Comp_MaterialBaseEffect.h"

class RENDERSDK_API Comp_MaterialButton : public Comp_MaterialBaseEffect
{
	DECLARE_CLASS_SIMPLE(Comp_MaterialButton, Comp_MaterialBaseEffect);

public:
	Comp_MaterialButton(const CObjectAbstract * Parent);
	Comp_MaterialButton(const Comp_MaterialButton & Source);
	virtual ~Comp_MaterialButton();

	virtual void			OnPropertyChanged(const char* PropertyName);

	virtual void			Initialize();

	virtual bool			LoadDiffuseMap(const char *filename, unsigned int IndexState);

	virtual Vector4f		GetUV(unsigned int StateIdx) const;
	virtual void			SetUV(const Vector4f & UV, unsigned int StateIdx);

	virtual void			SetDiffuseReference(const std::string & StrRef, unsigned int StateIdx);
	virtual std::string		GetDiffuseReference(unsigned int StateIdx) const;

	virtual void			SetActiveState(unsigned int StateIdx);

protected:
private:
public:
	class MaterialEffect_ButtonStates *m_pMaterialEffect;
};
#endif//__comp_materialbutton_h__