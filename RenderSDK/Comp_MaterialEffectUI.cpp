#include "stdafxrender.h"

REGISTER_CLASS(Comp_MaterialEffectUI, Comp_MaterialBaseEffect);

//----------------------------------------------------------------------------------------------
Comp_MaterialEffectUI::Comp_MaterialEffectUI(const CObjectAbstract * Parent)
: Super(Parent)
{
	pMaterialEffectUI = oes::d3d::Alloc_MaterialEffectUI(GetRenderComponent()->GetRenderDriver());
	SetBaseEffectPtr(pMaterialEffectUI);
}

//----------------------------------------------------------------------------------------------
Comp_MaterialEffectUI::Comp_MaterialEffectUI(const Comp_MaterialEffectUI & Source)
: Super(Source)
{
	if (this != &Source)
	{
		//pMaterialEffect = new MaterialEffect(*Source.pMaterialEffect);
		SetBaseEffectPtr(pMaterialEffectUI);
	}
}

//----------------------------------------------------------------------------------------------
Comp_MaterialEffectUI::~Comp_MaterialEffectUI()
{
    oes::d3d::Release_MaterialEffectUI(pMaterialEffectUI);
	pMaterialEffectUI = NULL;

	SetBaseEffectPtr(NULL);
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialEffectUI::Initialize()
{
	Super::Initialize();

	ReloadEffect();
}

//----------------------------------------------------------------------------------------------
bool Comp_MaterialEffectUI::LoadDiffuseMap(const char *filename, bool ImmidiateLoad/* = true*/)
{
	bool bResult = false;

	if (pMaterialEffectUI){
		bResult = pMaterialEffectUI->LoadDiffuseMap(filename, ImmidiateLoad);
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool Comp_MaterialEffectUI::LoadDiffuseMapW(const wchar_t *filename)
{
	bool bResult = false;

	if (pMaterialEffectUI){
		bResult = pMaterialEffectUI->LoadDiffuseMapW(filename);
	}
	return bResult;
}
//----------------------------------------------------------------------------------------------
Vector4f Comp_MaterialEffectUI::GetUV() const
{
    oes::d3d::Texture2D * Tex = pMaterialEffectUI->GetDiffuseTexture();
	return Vector4f(Tex->GetU(), Tex->GetV(), Tex->GetUL(), Tex->GetVL());
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialEffectUI::SetUV(const Vector4f & UV)
{
    oes::d3d::Texture2D * Tex = pMaterialEffectUI->GetDiffuseTexture();
	Tex->SetTexcoords(UV.x, UV.y, UV.z, UV.w);
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialEffectUI::SetDiffuseReference(const std::string & StrRef)
{
    oes::d3d::Texture2D * Tex = pMaterialEffectUI->GetDiffuseTexture();
	Tex->SetReference(StrRef);
}

//----------------------------------------------------------------------------------------------
std::string	Comp_MaterialEffectUI::GetDiffuseReference() const
{
    oes::d3d::Texture2D * Tex = pMaterialEffectUI->GetDiffuseTexture();
	return Tex->GetReference();
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialEffectUI::SetRenderTarget(Comp_RenderTarget *pRT)
{
	pMaterialEffectUI->SetRenderTarget(pRT->GetRenderTarget());
}

//----------------------------------------------------------------------------------------------
bool Comp_MaterialEffectUI::GetInfo(unsigned int &Width, unsigned int &Height) const
{
	return pMaterialEffectUI->GetInfo(Width, Height);
}

//----------------------------------------------------------------------------------------------
bool Comp_MaterialEffectUI::GetDIB(BYTE** pData, bool bResample /*= false*/, unsigned int NewWidth /*= 0*/, unsigned int NewHeight /*= 0*/) const
{
	return pMaterialEffectUI->GetDIB(pData, bResample, NewWidth, NewHeight);
}