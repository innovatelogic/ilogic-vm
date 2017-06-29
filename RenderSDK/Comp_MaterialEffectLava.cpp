#include "StdafxRender.h"

REGISTER_CLASS(Comp_MaterialEffectLava, Comp_MaterialBaseEffect);

//----------------------------------------------------------------------------------------------
Comp_MaterialEffectLava::Comp_MaterialEffectLava(const CObjectAbstract * Parent)
: Super(Parent)
{
	pMaterialEffectLava = new oes::d3d::MaterialEffectLava(GetRenderComponent()->GetRenderDriver());
	SetBaseEffectPtr(pMaterialEffectLava);
}

//----------------------------------------------------------------------------------------------
Comp_MaterialEffectLava::Comp_MaterialEffectLava(const Comp_MaterialEffectLava & Source)
: Super(Source)
{
	if (this != &Source)
	{

	}
}

//----------------------------------------------------------------------------------------------
Comp_MaterialEffectLava::~Comp_MaterialEffectLava()
{
	pMaterialEffectLava->Release();
	delete pMaterialEffectLava;
	pMaterialEffectLava = NULL;

	SetBaseEffectPtr(NULL);
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialEffectLava::Initialize()
{
	Super::Initialize();

	GetRenderComponent()->EnterCS();

	std::string sFilename;
	pMaterialEffectLava->Load(FindFile(GetReference(), &sFilename) ? sFilename.c_str() : "");

	GetRenderComponent()->LeaveCS();
}

//----------------------------------------------------------------------------------------------
bool Comp_MaterialEffectLava::LoadDiffuseMap(const char *filename, bool ImmidiateLoad /*= true*/)
{
	if (pMaterialEffectLava){
		return pMaterialEffectLava->LoadDiffuseMap(filename, ImmidiateLoad);
	}
	return false;
}

//----------------------------------------------------------------------------------------------
bool Comp_MaterialEffectLava::LoadNormalMap(const char *filename, bool ImmidiateLoad /*= true*/)
{
	if (pMaterialEffectLava){
		return pMaterialEffectLava->LoadNoiseMap(filename, ImmidiateLoad);
	}
	return false;
}

//----------------------------------------------------------------------------------------------
bool Comp_MaterialEffectLava::LoadHeightMap(const char *filename, bool ImmidiateLoad /*= true*/)
{
	if (pMaterialEffectLava){
		return pMaterialEffectLava->LoadHeightMap(filename, ImmidiateLoad);
	}
	return false;
}
