#ifndef __basesplineagregator_h__
#define __basesplineagregator_h__

#ifdef _WIN32
#pragma once
#endif

#include "coresdkafx.h"

class CORESDK_API BaseSplineAgregator : public UIScreenObject
{
	DECLARE_CLASS_SIMPLE(BaseSplineAgregator, UIScreenObject);

public:
	BaseSplineAgregator(const CObjectAbstract * Parent);
	BaseSplineAgregator(const BaseSplineAgregator &Source);
	virtual ~BaseSplineAgregator();
	
	virtual void	Initialize();
	virtual void	OnPropertyChanged(const char* PropertyName);
	//virtual void	DoUpdate(float dt);
	virtual bool	HitTest(const Vector2f &Position);
	virtual void	DoDraw();

	virtual void	Rebuild();

	virtual void	BuildSpline(TVectorList& OutSpline);
	virtual void	BuildHitVolume(TVectorList& InSpline, TTriangleList2f &OutVolume);

	virtual size_t	GetDetail() { return SplineDetail; }
	virtual void	SetDetail(size_t detail) { SplineDetail = detail; }
	virtual bool	GetClosed() { return SplineClosed; }
	virtual void	SetClosed(bool closed) { SplineClosed = closed; }

protected:
	//virtual void UpdateController(CActor* Actor, SubscriberData* Controller);

private:	
	/** spline point list */
	TVectorList SplineList;
	
	/** spline n-detail value */
	size_t SplineDetail;

	/** close spline */
	bool SplineClosed;
	
	/** spline hit test */
	TTriangleList2f HitTestTriangleList;
};

#endif//__basesplineagregator_h__