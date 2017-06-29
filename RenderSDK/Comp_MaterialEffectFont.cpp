#include "stdafxrender.h"

REGISTER_CLASS(CComp_MaterialEffectFont, Comp_MaterialBaseEffect);

//----------------------------------------------------------------------------------------------
CComp_MaterialEffectFont::CComp_MaterialEffectFont(const CObjectAbstract *pParent)
	: Super(pParent)
	, m_pMaterialEffect(NULL)
{
	m_pMaterialEffect = oes::d3d::Alloc_MaterialEffectFont(GetRenderComponent()->GetRenderDriver());
	SetBaseEffectPtr(m_pMaterialEffect);
}

//----------------------------------------------------------------------------------------------
CComp_MaterialEffectFont::CComp_MaterialEffectFont(const CComp_MaterialEffectFont &Source)
	: Super(Source)
	, m_pMaterialEffect(NULL)
{
	if (this != &Source)
	{
	}
}

//----------------------------------------------------------------------------------------------
CComp_MaterialEffectFont::~CComp_MaterialEffectFont()
{
    oes::d3d::Release_MaterialEffectFont(m_pMaterialEffect);
	m_pMaterialEffect = NULL;

	SetBaseEffectPtr(NULL);
}

//----------------------------------------------------------------------------------------------
void CComp_MaterialEffectFont::Initialize()
{
	Super::Initialize();

	ReloadEffect();
}
