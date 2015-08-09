#ifndef __meshnode_h__
#define __meshnode_h__

#ifdef _WIN32
#pragma once
#endif

#include "RenderObject.h"

class RENDERSDK_API Comp_MeshNode : public CRenderObject
{
	DECLARE_CLASS_SIMPLE(Comp_MeshNode, CRenderObject);

public:
	Comp_MeshNode(const CObjectAbstract * Parent);
	Comp_MeshNode(const Comp_MeshNode & Source);
	virtual ~Comp_MeshNode();

	virtual void	Clear();
	virtual void	Render() const;
	
	class D3DMesh		*Mesh;
	class BaseMaterial	*Effect;
};

#endif//__meshnode_h__