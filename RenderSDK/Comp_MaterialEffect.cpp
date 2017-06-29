#include "StdafxRender.h"

REGISTER_CLASS(Comp_MaterialEffect, Comp_MaterialBaseEffect);

//----------------------------------------------------------------------------------------------
Comp_MaterialEffect::Comp_MaterialEffect(const CObjectAbstract * Parent)
: Super(Parent)
{
	pMaterialEffect = oes::d3d::Alloc_MaterialEffect(GetRenderComponent()->GetRenderDriver());
	SetBaseEffectPtr(pMaterialEffect);
}

//----------------------------------------------------------------------------------------------
Comp_MaterialEffect::~Comp_MaterialEffect()
{
    oes::d3d::Release_MaterialEffect(pMaterialEffect);
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
/*const*/ oes::d3d::MaterialEffect* Comp_MaterialEffect::GetMaterialEffect() const
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
	assert(pMaterialEffect);

    oes::d3d::Texture2D * Tex = pMaterialEffect->GetDiffuseTexture();
	return Vector4f(Tex->GetU(), Tex->GetV(), Tex->GetUL(), Tex->GetVL());
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialEffect::SetUV(const Vector4f &uv)
{
	assert(pMaterialEffect);

    oes::d3d::Texture2D * Tex = pMaterialEffect->GetDiffuseTexture();
	Tex->SetTexcoords(uv.x, uv.y, uv.z, uv.w);
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialEffect::SetDiffuseReference(const std::string &strRef)
{
	assert(pMaterialEffect);

    oes::d3d::Texture2D * Tex = pMaterialEffect->GetDiffuseTexture();
	Tex->SetReference(strRef);
}

//----------------------------------------------------------------------------------------------
std::string	Comp_MaterialEffect::GetDiffuseReference() const
{
	assert(pMaterialEffect);

    oes::d3d::Texture2D * Tex = pMaterialEffect->GetDiffuseTexture();
	return Tex->GetReference();
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialEffect::SetAlphaTestEnable(bool flag)
{
	assert(pMaterialEffect);
	pMaterialEffect->SetAlphaTestEnable(flag);
}

//----------------------------------------------------------------------------------------------
bool Comp_MaterialEffect::GetAlphaTestEnable()
{
	assert(pMaterialEffect);
	return pMaterialEffect->GetAlphaTestEnable();
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialEffect::SetTransparent(float value)
{
	assert(pMaterialEffect);
	pMaterialEffect->SetTransparent(value);
}

//----------------------------------------------------------------------------------------------
float Comp_MaterialEffect::GetTransparent()
{
	assert(pMaterialEffect);
	return pMaterialEffect->GetTransparent();
}