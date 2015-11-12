#include "m_model.h"
#include "m_streams.h"
#include "m_mesh.h"

//-----------------------------------------------------------------------
m_node::node_type m_model::type = GEOMETRY;

//-----------------------------------------------------------------------
m_model::m_model()
: aabb_min(V_FLT_MAX, V_FLT_MAX, V_FLT_MAX)
, aabb_max(-V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX)
{

}

//-----------------------------------------------------------------------
m_model::~m_model()
{
	for (TVecMeshIterator Iter = meshes.begin(); Iter != meshes.end(); ++Iter){
		delete *Iter;
	}
	meshes.clear();
}

//-----------------------------------------------------------------------
m_node::node_type m_model::get_type() const
{
   return type;
}

//-----------------------------------------------------------------------
bool m_model::accept(const m_scene & scene, m_visitor & visitor) const
{
	bool ret = m_node::accept(scene,visitor);
	visitor.visit_model(this);
	return ret;
}

//-----------------------------------------------------------------------
/// Write a model to an nv_output_stream.
m_output_stream & operator << (m_output_stream & rOutputStream, const m_model & rModel)
{
	rOutputStream << dynamic_cast<const m_node &>(rModel);

	// Meshes
	rOutputStream << (unsigned int)rModel.meshes.size();

	for (size_t iMesh = 0; iMesh < rModel.meshes.size(); ++iMesh)
	{
		rOutputStream << *rModel.meshes[iMesh];
	}
	// Bounding box 
	rOutputStream << rModel.aabb_min << rModel.aabb_max;

	return rOutputStream;
}

//-----------------------------------------------------------------------
/// Read a model from an nv_input_stream.
m_input_stream & operator >> (m_input_stream & rInputStream, m_model & rModel)
{
	// First write the node part of model
	rInputStream >> dynamic_cast<m_node &>(rModel);

	// Meshes
	unsigned int num_meshes;
	rInputStream >> num_meshes;

	for (unsigned int Index = 0; Index < num_meshes; ++Index)
	{
		rModel.meshes.push_back(new m_mesh);
		assert(rModel.meshes[Index]);
		rInputStream >> *rModel.meshes[Index];
	}

	// Bounding box 
	rInputStream >> rModel.aabb_min >> rModel.aabb_max;

	return rInputStream;
}
