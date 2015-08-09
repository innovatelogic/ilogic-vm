#include "stdafxrender.h"

REGISTER_CLASS(Comp_RenderTarget, CRenderObject);

//----------------------------------------------------------------------------------------------
Comp_RenderTarget::Comp_RenderTarget(const CObjectAbstract * Parent)
: Super(Parent)
{
	RenderTarget = Alloc_D3DRenderTarget(GetRenderComponent()->GetRenderDriver());
}

//----------------------------------------------------------------------------------------------
Comp_RenderTarget::Comp_RenderTarget(const Comp_RenderTarget & Source)
: Super(Source)
{
	if (this != &Source)
	{
		//RenderTarget = new D3DRenderTarget(*Source.RenderTarget);
		assert(false);
	}
}

//----------------------------------------------------------------------------------------------
Comp_RenderTarget::~Comp_RenderTarget()
{
	Release_D3DRenderTarget(RenderTarget);
	delete RenderTarget;
	RenderTarget = NULL;
}

//----------------------------------------------------------------------------------------------
void Comp_RenderTarget::Initialize()
{
	Super::Initialize();
}

//----------------------------------------------------------------------------------------------
bool Comp_RenderTarget::CreateRenderTarget(unsigned int Width, unsigned int Height)
{
	return RenderTarget->CreateRenderTarget(Width, Height);
}

//----------------------------------------------------------------------------------------------
const D3DRenderTarget * Comp_RenderTarget::GetRenderTarget() const
{
	return RenderTarget;
}