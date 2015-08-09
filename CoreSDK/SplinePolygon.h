#ifndef __splinepolygon_h__
#define __splinepolygon_h__

#ifdef WIN32
#pragma once
#endif

class SplinePolygon :
	public ActorAllocator, public IDrawInterface
{
	DECLARE_CLASS_SIMPLE(SplinePolygon, ActorAllocator);

public:
	SplinePolygon(const CObjectAbstract * Parent);
	SplinePolygon(const SplinePolygon & Source);
	virtual ~SplinePolygon();

	/** Init */
	virtual void		Initialize();
	virtual void		OnPropertyChanged(const char* PropertyName);

	virtual bool		Rebuild();

protected:
	/**mesh components update*/
	virtual void		DoRebuildMesh();


private:
	/** spline point list */
	std::vector<size_t>			m_IndexList;

	/** transient meshes */
	class Comp_StaticMesh	*m_pMeshComponent;
};

#endif//__splinepolygon_h__