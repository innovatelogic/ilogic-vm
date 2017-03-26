#pragma once

#include "ActorAllocator.h"
#include "IDrawInterface.h"

class EXPORT UIViewPivotControl : public ActorAllocator, public IDrawInterface
{
	DECLARE_CLASS_SIMPLE(UIViewPivotControl, ActorAllocator);

public:
	UIViewPivotControl(const CObjectAbstract * Parent);
	virtual ~UIViewPivotControl();

	virtual void	OnPropertyChanged(const char* PropertyName);
	virtual void	Initialize();

	virtual void	DoDraw();

	class Comp_RenderTarget* GetRenderTarget() const { return CompRenderTarget; }

	bool			ProcessClick(const MouseInputData& InputData, const Vector2f& ViewportSize);

protected:
	void			RebuildMesh();

private:
	class Comp_RenderTarget		*CompRenderTarget;
	class Comp_StaticMesh		*MeshComponent;
	class Comp_MaterialEffect	*m_pMaterialEffect;
};