#ifndef __splineagregator_h__
#define __splineagregator_h__

#ifdef _WIN32
#pragma once
#endif

#include "UIScreenObject.h"

class SplineAgregator : public UIScreenObject
{	
	friend class Comp_StaticMesh;
	
	DECLARE_CLASS_SIMPLE(SplineAgregator, UIScreenObject);

public:
	/**constructor destructor*/
	SplineAgregator(const CObjectAbstract * Parent);
	SplineAgregator(const SplineAgregator & Source);
	virtual ~SplineAgregator();

	/** Init */
	virtual void				Initialize();
	virtual void				OnPropertyChanged(const char* PropertyName);

	virtual Vector2f			GetTransformedSize_() const;
	virtual bool				HitTest(const Vector2f &Position);
	virtual void				DoDraw();
	virtual void				DrawBounds() const;

	/** spline methods */
	virtual void				SetColor(unsigned int color) { Color = color; }
	virtual unsigned int		GetColor() const { return Color; }
	virtual unsigned int		GetDetail() const { return Detail; }
	virtual void				SetDetail(unsigned int detail) { Detail = detail; }
	virtual void				SetBorder(bool flag) { bBorder = flag; }
	virtual bool				GetBorder() const { return bBorder; }
	virtual void				SetBorderSize(float size) { BorderSize = size; }
	virtual float				GetBorderSize() const { return BorderSize; }
	virtual void				SetDiffuseTex(std::string& ResourceTag) { DiffuseTex = ResourceTag; }
	virtual std::string			GetDiffuseTex() const {	return DiffuseTex; }
	virtual void				SetBorderColor(unsigned int color) { BorderColor = color; }
	virtual unsigned int		GetBorderColor() const { return BorderColor; }
	virtual size_t				GetNumNodes() const { return SplineList.size(); }
	virtual bool				GetPointByIndex(size_t Index, Vector2f & OutValue) const;

	virtual bool				Rebuild();
	
	/** clear triangle lists */
	virtual void				ResetSpline();

	/** build closed spline */
	virtual void				BuildSplineList(TVecIndexedPoint2f& OutSpline);

protected:
	/**mesh components updte*/
	virtual void				DoRebuildMesh();
	virtual void				DoRebuildBorderMesh();

	/** vizualization */
	virtual void				RenderSpline(const Matrix &WTM);

	/** build border triangle list*/
	virtual bool				BuildBorder(TVecIndexedPoint2f &Spline);
	virtual void				AddTriangle(TVecIndexedPoint2f &VB, const Vector2f& P0, const Vector2f& P1, const Vector2f& P2);

public:
	/** update UV texture coords */
	virtual void				UpdateCoordsUV(const Matrix &WTM, const Vector2f& SourceSize);
	TVecIndexedPoint2f&			GetSplineList() { return SplineList; }

	virtual bool				InsertPoint(const Vector2f& MousePos, const UIScreenObject* Inserter);

private:
	virtual void				ConvertToEditableMesh();

private:
	/** spline point list */
	TVecIndexedPoint2f			SplineList;
	std::vector<size_t>			IndexList;

	TVecIndexedPoint2f			SplineBorderList;
	std::vector<size_t>			IndexBorderList;

	/** triangles */
	TTriangleList2f				TriangleBorderList;

	/** spline detail */
	bool						AdaptiveDetail; 
	unsigned int				Detail;
	unsigned int				Color;

	/** border representation flag*/
	bool						bBorder;
	float						BorderSize;
	unsigned int				BorderColor;
	bool						SplineClosed;
	Vector2f					InternalPreUpdateShiftTranslate;

	/** render texture */
	std::string					DiffuseTex;

	/** transient meshes */
	class Comp_StaticMesh		*MeshComponent;
	class Comp_StaticMesh		*BorderMeshComponent;

	class Comp_MaterialEffect	*MaterialEffect;
	class Comp_MaterialEffect	*MaterialEffectBorder;
};

#endif//__splineagregator_h__