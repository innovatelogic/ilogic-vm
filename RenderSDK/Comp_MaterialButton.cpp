#include "stdafxrender.h"

REGISTER_CLASS(Comp_MaterialButton, Comp_MaterialBaseEffect);

//----------------------------------------------------------------------------------------------
Comp_MaterialButton::Comp_MaterialButton(const CObjectAbstract * Parent)
: Super(Parent)
, m_pMaterialEffect(NULL)
{
	m_pMaterialEffect = Alloc_MaterialEffect_ButtonStates(GetRenderComponent()->GetRenderDriver());

	SetBaseEffectPtr(m_pMaterialEffect);
}

//----------------------------------------------------------------------------------------------
Comp_MaterialButton::Comp_MaterialButton(const Comp_MaterialButton & Source)
: Super(Source)
, m_pMaterialEffect(NULL)
{
	if (this != &Source)
	{
		assert(false);
		//pMaterialEffect = new MaterialEffect_ButtonStates(*Source.pMaterialEffect);
		SetBaseEffectPtr(m_pMaterialEffect);
	}
}

//----------------------------------------------------------------------------------------------
Comp_MaterialButton::~Comp_MaterialButton()
{
	Release_MaterialEffect_ButtonStates(m_pMaterialEffect);
	m_pMaterialEffect = NULL;

// 	pMaterialEffect->Release();
// 	delete pMaterialEffect;

	SetBaseEffectPtr(NULL);
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialButton::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);

	if (!strcmp(PropertyName, "Ref"))
	{
		ReloadEffect();
	}
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialButton::Initialize()
{
	Super::Initialize();

	ReloadEffect();
}

//----------------------------------------------------------------------------------------------
bool Comp_MaterialButton::LoadDiffuseMap(const char *filename, unsigned int IndexState)
{
	bool bResult = false;

	if (m_pMaterialEffect)
	{
		std::string sFilename;
		if (FindFile(filename, &sFilename))
		{
			bResult = m_pMaterialEffect->LoadDiffuseMap(sFilename.c_str(), IndexState);
		}
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
Vector4f Comp_MaterialButton::GetUV(unsigned int StateIdx) const
{
	Texture2D * Tex = m_pMaterialEffect->GetDiffuseTexture(StateIdx);
	return Vector4f(Tex->GetU(), Tex->GetV(), Tex->GetUL(), Tex->GetVL());
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialButton::SetUV(const Vector4f & UV, unsigned int StateIdx)
{
	Texture2D * Tex = m_pMaterialEffect->GetDiffuseTexture(StateIdx);
	Tex->SetTexcoords(UV.x, UV.y, UV.z, UV.w);
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialButton::SetDiffuseReference(const std::string & StrRef, unsigned int StateIdx)
{
	Texture2D * Tex = m_pMaterialEffect->GetDiffuseTexture(StateIdx);
	Tex->SetReference(StrRef);
}

//----------------------------------------------------------------------------------------------
std::string	Comp_MaterialButton::GetDiffuseReference(unsigned int StateIdx) const
{
	Texture2D * Tex = m_pMaterialEffect->GetDiffuseTexture(StateIdx);
	return Tex->GetReference();
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialButton::SetActiveState(unsigned int StateIdx)
{
	m_pMaterialEffect->SetActiveIdx(StateIdx);
}