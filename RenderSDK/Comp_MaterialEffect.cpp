#include "StdafxRender.h"

REGISTER_CLASS(Comp_MaterialEffect, Comp_MaterialBaseEffect);

//----------------------------------------------------------------------------------------------
Comp_MaterialEffect::Comp_MaterialEffect(const CObjectAbstract * Parent)
: Super(Parent)
{
	pMaterialEffect = Alloc_MaterialEffect(GetRenderComponent()->GetRenderDriver());
	SetBaseEffectPtr(pMaterialEffect);
}

//----------------------------------------------------------------------------------------------
Comp_MaterialEffect::Comp_MaterialEffect(const Comp_MaterialEffect & Source)
: Super(Source)
{
	if (this != &Source)
	{
		pMaterialEffect = new MaterialEffect(*Source.pMaterialEffect);
		SetBaseEffectPtr(pMaterialEffect);
	}
}

//----------------------------------------------------------------------------------------------
Comp_MaterialEffect::~Comp_MaterialEffect()
{
	Release_MaterialEffect(pMaterialEffect);
	pMaterialEffect = NULL;

	SetBaseEffectPtr(NULL);
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialEffect::Initialize()
{
	Super::Initialize();

	ReloadEffect();
}

//----------------------------------------------------------------------------------------------
/*const*/ MaterialEffect* Comp_MaterialEffect::GetMaterialEffect() const
{
	assert(pMaterialEffect);
	return pMaterialEffect; 
}

//----------------------------------------------------------------------------------------------
bool Comp_MaterialEffect::LoadDiffuseMap(const char *filename, bool ImmidiateLoad /*= true*/)
{
	if (pMaterialEffect){
		return pMaterialEffect->LoadDiffuseMap(filename, ImmidiateLoad);
	}
	return false;
}

//----------------------------------------------------------------------------------------------
bool Comp_MaterialEffect::LoadNormalMap(const char *filename, bool ImmidiateLoad /*= true*/)
{
	if (pMaterialEffect){
		return pMaterialEffect->LoadNormalMap(filename, ImmidiateLoad);
	}
	return false;
}

//----------------------------------------------------------------------------------------------
bool Comp_MaterialEffect::LoadHeightMap(const char *filename, bool ImmidiateLoad /*= true*/)
{
	if (pMaterialEffect){
		return pMaterialEffect->LoadHeightMap(filename, ImmidiateLoad);
	}
	return false;
}

//----------------------------------------------------------------------------------------------
Vector4f Comp_MaterialEffect::GetUV() const
{
	Texture2D * Tex = pMaterialEffect->GetDiffuseTexture();
	return Vector4f(Tex->GetU(), Tex->GetV(), Tex->GetUL(), Tex->GetVL());
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialEffect::SetUV(const Vector4f & UV)
{
	Texture2D * Tex = pMaterialEffect->GetDiffuseTexture();
	Tex->SetTexcoords(UV.x, UV.y, UV.z, UV.w);
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialEffect::SetDiffuseReference(const std::string & StrRef)
{
	Texture2D * Tex = pMaterialEffect->GetDiffuseTexture();
	Tex->SetReference(StrRef);
}

//----------------------------------------------------------------------------------------------
std::string	Comp_MaterialEffect::GetDiffuseReference() const
{
	Texture2D * Tex = pMaterialEffect->GetDiffuseTexture();
	return Tex->GetReference();
}