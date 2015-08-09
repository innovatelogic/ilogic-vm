#ifndef __d3dmeshallocator_h__
#define __d3dmeshallocator_h__

#ifdef _WIN32
#pragma once
#endif

#include "../D3DDrv/D3DMesh.h"

class RENDERSDK_API D3DMeshAllocator : public D3DMesh
{
	DECL_CLASS_SIMPLE(D3DMeshAllocator, D3DMesh);

public:
	D3DMeshAllocator(const class D3DDriver *pInterface);
	D3DMeshAllocator(const D3DMeshAllocator &Sender);
	virtual ~D3DMeshAllocator();

	virtual bool	LoadFromDesc(const SMeshMaterialSet &Desc);

	virtual BaseMaterial*	ApplyMeshMaterial(int materialID, const SMeshMaterialSet &MaterialSet);

private:
	void DeserializeMesh(m_file &oFile, class SubMeshNode *pInMesh, unsigned int &numDistinctBones,	std::vector<int> &VecBoneRemapper);

	// file versions legacy
	void LoadFile_v1_0(m_file &oFile, const SMeshMaterialSet &Desc);
	void LoadFile_v1_2(m_file &oFile, const SMeshMaterialSet &Desc);
};

#endif//__d3dmeshallocator_h__