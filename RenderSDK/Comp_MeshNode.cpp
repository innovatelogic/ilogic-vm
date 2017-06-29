#include "StdafxRender.h"

REGISTER_CLASS(Comp_MeshNode, CRenderObject);

//----------------------------------------------------------------------------------------------
Comp_MeshNode::Comp_MeshNode(const CObjectAbstract * Parent)
: Super(Parent)
, Effect(NULL)
{
	Mesh = new oes::d3d::D3DMesh(GetRenderComponent()->GetRenderDriver());
	assert(Mesh);
}

//----------------------------------------------------------------------------------------------
Comp_MeshNode::Comp_MeshNode(const Comp_MeshNode & Source)
: Super(Source)
, Effect(NULL)
{
	if (this != &Source)
	{
	}
}

//----------------------------------------------------------------------------------------------
Comp_MeshNode::~Comp_MeshNode()
{
	if (Mesh)
	{
		delete Mesh;
		Mesh = nullptr;
	}

	if(Mesh) 
	{
		delete Mesh;
		Mesh = nullptr; 
	}
}

//----------------------------------------------------------------------------------------------
void Comp_MeshNode::Clear()
{
	Mesh->Clear();
}

//----------------------------------------------------------------------------------------------
void Comp_MeshNode::Render() const
{
	Mesh->Render();
}