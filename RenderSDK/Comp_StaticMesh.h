#ifndef __staticmesh_h__
#define __staticmesh_h__

#ifdef _WIN32
#pragma once
#endif
#include "D3DMesh.h"
#include "RenderObject.h"

class RENDERSDK_API Comp_StaticMesh : public CRenderObject
{
	DECLARE_CLASS_SIMPLE(Comp_StaticMesh, CRenderObject);

public:
    Comp_StaticMesh(const CObjectAbstract *pParent);
    virtual ~Comp_StaticMesh();

	virtual void			OnPropertyChanged(const char* PropertyName);

	virtual void			LoadMesh(const SMeshMaterialSet &Desc);

	virtual void			Clear();

	virtual void			Render() const;

	void					SetShowDebug(bool Value);

	bool					GetShowNormals() const { return m_bShowNormals; }
	void					SetShowNormals(bool Value);

	bool					GetShowTangent() const { return m_bShowTangent; }
	void					SetShowTangent(bool Value);

	bool					GetShowBinormal() const { return m_bShowBinormal; }
	void					SetShowBinormal(bool Value);

	void					SetCastShadows(bool Value);
	void					SetReciveShadows(bool Value);

	class D3DMesh*			GetD3DMesh() const { return m_pMesh; }

	virtual void			SetForceLODLvl(unsigned int Value);
	virtual void			SetDestructLvl(unsigned int Value);

private:
	virtual void			ReloadMesh();

public:
	std::string				m_filename;
	
	bool					m_bShowNormals;
	bool					m_bShowBinormal;
	bool					m_bShowTangent;

private:
	class D3DMesh			*m_pMesh;
};

#endif//__staticmesh_h__



