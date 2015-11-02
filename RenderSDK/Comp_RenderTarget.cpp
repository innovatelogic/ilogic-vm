#include "stdafxrender.h"

REGISTER_CLASS(Comp_RenderTarget, CRenderObject);

//----------------------------------------------------------------------------------------------
Comp_RenderTarget::Comp_RenderTarget(const CObjectAbstract *parent)
: Super(parent)
, m_pRenderTarget(nullptr)
{
	m_pRenderTarget = Alloc_D3DRenderTarget(GetRenderComponent()->GetRenderDriver());
}

//----------------------------------------------------------------------------------------------
Comp_RenderTarget::Comp_RenderTarget(const Comp_RenderTarget &source)
: Super(source)
{
}

//----------------------------------------------------------------------------------------------
Comp_RenderTarget::~Comp_RenderTarget()
{
	Release_D3DRenderTarget(m_pRenderTarget);
	delete m_pRenderTarget;
	m_pRenderTarget = NULL;
}

//----------------------------------------------------------------------------------------------
void Comp_RenderTarget::Initialize()
{
	Super::Initialize();
}

//----------------------------------------------------------------------------------------------
bool Comp_RenderTarget::CreateRenderTarget(unsigned int width, unsigned int height)
{
	return m_pRenderTarget->CreateRenderTarget(width, height);
}

//----------------------------------------------------------------------------------------------
const D3DRenderTarget* Comp_RenderTarget::GetRenderTarget() const
{
	return m_pRenderTarget;
}