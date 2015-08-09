#ifndef _m_model_h_
#define _m_model_h_

#include "m_node.h"

#ifdef _WIN32
#pragma once
#endif

struct EXPORT m_model : public m_node
{
	typedef std::vector<struct m_mesh*> TVecMesh;
	typedef TVecMesh::iterator		    TVecMeshIterator;

    m_model();
    virtual ~m_model();

	static  node_type type;
	virtual node_type get_type() const;	

	virtual bool accept(const m_scene & scene, m_visitor & visitor) const;

	// meshes
	TVecMesh		meshes;

	// bounding box information...
	Vector			aabb_min;
	Vector			aabb_max;
};

EXPORT m_output_stream & operator << (m_output_stream & rOutputStream, const m_model & rModel);
EXPORT m_input_stream & operator >> (m_input_stream & rInputStream, m_model & rModel);

#endif //_m_model_h_