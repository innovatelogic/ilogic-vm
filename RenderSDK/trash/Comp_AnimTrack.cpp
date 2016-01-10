#include "StdafxRender.h"

REGISTER_CLASS(Comp_AnimTrack, CRenderObject);

//------------------------------------------------------------------------
Comp_AnimTrack::Comp_AnimTrack(const CObjectAbstract * Parent)
: Super(Parent)
{
	pSkeleton = Alloc_D3DSkeleton(GetRenderComponent()->GetRenderDriver());
}

//------------------------------------------------------------------------
Comp_AnimTrack::Comp_AnimTrack(const Comp_AnimTrack & Source)
: Super(Source)
{
	if (this != &Source)
	{
		assert(false);
		//pSkeleton = new D3DSkeleton(*Source.pSkeleton);
	}
}

//------------------------------------------------------------------------
Comp_AnimTrack::~Comp_AnimTrack()
{
	if (pSkeleton)
	{
		Release_D3DSkeleton(pSkeleton);
		delete pSkeleton;
		pSkeleton = NULL;
	}
}

//------------------------------------------------------------------------
bool Comp_AnimTrack::LoadFromFile(const char *Filename)
{
	bool bResult = false;

	std::string sFilename;  
	if (!FindFile(Filename, &sFilename)){
		return false;
	}

	bResult = pSkeleton->Load(sFilename);

	return bResult;
}