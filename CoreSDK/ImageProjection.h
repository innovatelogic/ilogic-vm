#ifndef __imageprojection_h__
#define __imageprojection_h__

#ifdef WIN32
#pragma once
#endif

#include "..\Foundation\StdafxFoundation.h"

class CORESDK_API CImageProjection : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(CImageProjection, ActorAllocator);

public:
	CImageProjection(const CObjectAbstract *Parent);
	CImageProjection(const CImageProjection &Source);
	virtual ~CImageProjection();

protected:
private:
	class CCamera *m_pCamera;
	class UIImage *m_pImage;
};

#endif//__imageprojection_h__