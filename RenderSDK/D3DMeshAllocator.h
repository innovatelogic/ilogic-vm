#ifndef __d3dmeshallocator_h__
#define __d3dmeshallocator_h__

#ifdef _WIN32
#pragma once
#endif

#include "../D3DDrv/D3DMesh.h"

class RENDERSDK_API D3DMeshAllocator : public oes::d3d::D3DMesh
{
	DECL_CLASS_SIMPLE(D3DMeshAllocator, oes::d3d::D3DMesh);

public:
	D3DMeshAllocator(const oes::d3d::D3DDriver *pInterface);
	D3DMeshAllocator(const D3DMeshAllocator &Sender);
	virtual ~D3DMeshAllocator();

	virtual bool	LoadFromDesc(const oes::d3d::SMeshMaterialSet &Desc);

	virtual oes::d3d::BaseMaterial*	ApplyMeshMaterial(int materialID, const oes::d3d::SMeshMaterialSet &MaterialSet);

private:
	void DeserializeMesh(m_file &oFile, oes::d3d::SubMeshNode *pInMesh, unsigned int &numDistinctBones,	std::vector<int> &VecBoneRemapper);

	// file versions legacy
	void LoadFile_v1_0(m_file &oFile, const oes::d3d::SMeshMaterialSet &Desc);
	void LoadFile_v1_2(m_file &oFile, const oes::d3d::SMeshMaterialSet &Desc);
};

#endif//__d3dmeshallocator_h__