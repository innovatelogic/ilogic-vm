#ifndef __polygonmesh_h__
#define __polygonmesh_h__

#ifdef _WIN32
#pragma once
#endif

#include "ActorAllocator.h"

struct STexMap
{
	std::vector<Vector>	 TexCoords;
};

struct SSelectInfo
{
	std::vector<int> PointIndexes;
	std::vector<Vector> PointOffsets; 	//local offsets to multiple selected points
};


//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class CPolygonMesh :
	public ActorAllocator, public IDrawInterface
{
	DECLARE_CLASS_SIMPLE(CPolygonMesh, ActorAllocator);
	
public:
	/**
	 * Constructor / Destructor
	 */
	CPolygonMesh(const CObjectAbstract *pParent);
	CPolygonMesh(const CPolygonMesh &Source);
	virtual ~CPolygonMesh();

	/** Init */
	virtual void	Initialize();
	virtual void	OnPropertyChanged(const char* PropertyName);

	virtual bool	Load(const char * FileName);
	virtual bool	Save(const char * FileName);

	virtual void	DoDraw();
	virtual void	DrawController() const;

	virtual bool	Rebuild();

	virtual bool	ProcessController(const MouseInputData &InputData);
	virtual bool	ProcessController(const MouseMoveInputData &InputData);

	virtual bool	ProcessRelease(const MouseInputData &InputData);

	size_t			GetNumberOfFaces() const { return m_Faces.size(); }

	const SPolygonFace*	GetFace(size_t Index) const { return &m_Faces[Index]; }

	/**
	 * Returns max actual face vertex index. 
	 *
	 * @return max index value, otherwise zero
	 */
	size_t			GetMaxFaceIndex() const;

	/**
	 * Returns number of affected texture mapping modificators for specified face. 
	 *
	 * @return number value, otherwise zero
	 */
	size_t			GetNumberOfTexVerts(size_t Index) const;

	size_t			GetNumberOfColorVerts(size_t Index) const;

protected:
	/**mesh components updte*/
	virtual void	DoRebuildMesh();

	void BuildFaceTopology(MatFaceMapType &OutMapMatface, TriMapType &OutMapTri, size_t MaxFaceIdx);

	void BuildMeshTopology(std::vector<m_mesh*> &OutVecMeshes, MatFaceMapType &InMapMatface, TriMapType &InMapTri);

	void ProcessNormals(MatFaceMapType &InMapMatface);

private:
	Vector GetSelectionPivot() const;

private:
	class Comp_StaticMesh		*m_pMeshComponent;
	class Comp_MaterialEffect	*m_pMaterialEffect;

	// geometry
	std::vector<Vector>			m_Vertices;			// array of vertices
	std::vector<Vector>			m_Normals;			// array of normals
	std::vector<Vector4f>		m_Colors;			// array of colors (vertex colors)
	std::vector<Vector>			m_TexCoords;		// base texture coords
	std::vector<STexMap>		m_TexMaps;			// texture mapping info

	// topology
	std::vector<SPolygonFace>		m_Faces;
	std::vector<SPolygonSubFace>	m_SubFaces;

	// material instances
	std::vector<class MaterialEffect*>	m_VecMatInstances;

	// uv modificators
	std::vector<class CUVModificator*> m_VecModificators;

	// Selection Info
	std::map<int, SSelectInfo> m_MapSelectInfo;
	Vector	m_PointGizmo;
};

#endif//__polygon_h__