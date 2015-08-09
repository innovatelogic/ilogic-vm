#ifndef __ProjectedLight_h__
#define __ProjectedLight_h__

#ifdef WIN32
#pragma once
#endif

#include "Camera.h"

class CORESDK_API ProjectedLight : public CCamera
{
	DECLARE_CLASS_SIMPLE(ProjectedLight, CCamera);

public:
	ProjectedLight(const CObjectAbstract *Parent);
	ProjectedLight(const ProjectedLight & Source);
	virtual ~ProjectedLight();

	virtual void	OnPropertyChanged(const char* PropertyName);
	virtual void	Initialize();

	virtual void	DoDraw();
	virtual void	PreDraw();

	virtual void			SetColor(unsigned int color) { Color = color; }
	virtual unsigned int	GetColor() const { return Color; }

	virtual void	SetAdapt(bool Value) { bTestAdapt = Value; }
	virtual bool	GetAdapt() const { return bTestAdapt; }

	virtual void			SetIndexTest(unsigned int Value) { IndexTest = Value; }
	virtual unsigned int	GetIndexTest() const { return IndexTest; }

	virtual void			SetDepthBias(float Value) { DepthBias = Value; }
	virtual float			GetDepthBias() const { return DepthBias; }

	virtual void			SetSlopeBias(float Value) { SlopeBias = Value; }
	virtual float			GetSlopeBias() const { return SlopeBias; }
 
protected:
private:
	virtual bool	BuildLightViewFrustrums(UniCullinfo &OutCullinfo, const class CCamera * BuildCamera, size_t Index) const;
	virtual Matrix	BuildCropMatrix(const CCamera::Subfrustra & InSubfrustra, const Matrix & SourceProjMatrix, const Bounds3f & CastersBB, const Bounds3f & RecieversBB);
	virtual void	CalcProjMatrix();

private:
	class Comp_RenderTarget	*CompRenderTarget;

	unsigned int		Color;
	Matrix				CropMatrixes[NUM_SPLITS];

	bool				bTestAdapt;
	unsigned int		IndexTest;

	float				DepthBias;
	float				SlopeBias;

};

#endif//__ProjectedLight_h__