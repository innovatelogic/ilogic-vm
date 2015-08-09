#include "stdafxrender.h"

REGISTER_CLASS_PURE(Comp_MaterialBaseEffect, CRenderObject)
	new PropertyString("XRef", (BYTE*)&((Comp_MaterialBaseEffect*)NULL)->XRef - (BYTE*)NULL, "Comp_MaterialBaseEffect",	"Value", READ_WRITE, CTRL_EDIT,	true, false),
	new PropertyString("NameTehnique", (BYTE*)&((Comp_MaterialBaseEffect*)NULL)->NameTehnique - (BYTE*)NULL, "Comp_MaterialBaseEffect", "Value", READ_WRITE, CTRL_EDIT,	true, false),
END_REGISTER_CLASS_PURE(Comp_MaterialBaseEffect, CRenderObject);

//----------------------------------------------------------------------------------------------
Comp_MaterialBaseEffect::Comp_MaterialBaseEffect(const CObjectAbstract * Parent)
: Super(Parent)
, pMaterialBaseEffect(NULL)
, XRef("ogl_diffuse.vs")
, NameTehnique("Technique_Diffuse")
{
}

//----------------------------------------------------------------------------------------------
Comp_MaterialBaseEffect::Comp_MaterialBaseEffect(const Comp_MaterialBaseEffect & Source)
: Super(Source)
, pMaterialBaseEffect(NULL)
, XRef("ogl_diffuse.vs")
, NameTehnique("Technique_Diffuse")
{
	if (this != &Source)
	{
		XRef = Source.XRef;
		NameTehnique = Source.NameTehnique;
	}
}

//----------------------------------------------------------------------------------------------
Comp_MaterialBaseEffect::~Comp_MaterialBaseEffect()
{

}

//----------------------------------------------------------------------------------------------
void Comp_MaterialBaseEffect::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);

	if (!strcmp(PropertyName, "XRef") || 
		!strcmp(PropertyName, "NameTehnique"))
	{
		ReloadEffect();
	}
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialBaseEffect::Initialize()
{
	Super::Initialize();
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialBaseEffect::SetTehnique(const char *tehnique)
{
	NameTehnique = tehnique; 

	assert(pMaterialBaseEffect);
	pMaterialBaseEffect->SetTechniqueName(tehnique);
}

//----------------------------------------------------------------------------------------------
std::string	Comp_MaterialBaseEffect::GetTehnique()
{ 
	return NameTehnique; 
}

//----------------------------------------------------------------------------------------------
unsigned int Comp_MaterialBaseEffect::BeginEffect() const
{
	assert(pMaterialBaseEffect);
	return pMaterialBaseEffect->BeginEffect(NameTehnique.c_str());
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialBaseEffect::EndEffect() const
{
	assert(pMaterialBaseEffect);
	pMaterialBaseEffect->EndEffect();
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialBaseEffect::SetMatrixParam(const char *ParamName, const Matrix& Value)
{
	assert(pMaterialBaseEffect);
	pMaterialBaseEffect->SetMatrixParam(ParamName, Value.GetPtr());
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialBaseEffect::SetVectorParam(const char *ParamName, const Vector& Value)
{
	assert(pMaterialBaseEffect);
	pMaterialBaseEffect->SetVector3Param(ParamName, Value.GetPtr());
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialBaseEffect::SetSampleTexture(const char *ParamName, const Texture2D * Value)
{
	assert(pMaterialBaseEffect);
	pMaterialBaseEffect->SetSampleTexture(ParamName, Value);
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialBaseEffect::SetScalarValue(const char *ParamName, float Value)
{
	assert(pMaterialBaseEffect);
	pMaterialBaseEffect->SetScalarValue(ParamName, Value);
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialBaseEffect::SetBaseEffectPtr(/*const*/ BaseMaterial * pEffect)
{
	pMaterialBaseEffect = pEffect;
}

//----------------------------------------------------------------------------------------------
BaseMaterial* Comp_MaterialBaseEffect::GetPrtEffect() const
{
	return pMaterialBaseEffect;
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialBaseEffect::ReloadEffect()
{
	//DWORD ThreadId = ::GetCurrentThreadId();

	//if (ThreadId != GetRenderComponent()->ThreadRenderID)
	//{
	//	GetRenderComponent()->PushToRenderStack( new Command<Comp_MaterialBaseEffect, Comp_MaterialBaseEffect>(&Comp_MaterialBaseEffect::DoReloadEffect, this), true);
	//}
	//else
	//{
		DoReloadEffect();
	//}
}

//----------------------------------------------------------------------------------------------
void Comp_MaterialBaseEffect::DoReloadEffect()
{
	std::string sFilename;
	if (FindFile(XRef.c_str(), &sFilename))
	{
		pMaterialBaseEffect->Load(sFilename.c_str());
	}
}