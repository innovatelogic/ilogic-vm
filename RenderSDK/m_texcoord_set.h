#pragma once

#include "m_stdafx.h"

using namespace oes::common_base;

struct EXPORT m_texcoord_set
{
	m_texcoord_set();
	virtual ~m_texcoord_set();

	unsigned int    num_coords;
	unsigned int    dim;            // dimension of the set: 1D, 2D, 3D,...
	float         * texcoords;      // array of texture coords of dimension dim
	                                // example: for a 2D texture coordinate set:
	                                // dim = 2 and 
	                                // [s,t,s,t,s,t,..., nv_mesh->num_vertices * 2]
	// texture basis
	// note : use the normals from the geometry to complete the basis...
	Vector       * binormals;
	Vector       * tangents;
};

EXPORT  class m_output_stream & operator << (m_output_stream & rOutputStream, const m_texcoord_set & rTexCoordSet);
EXPORT  class m_input_stream & operator >> (m_input_stream & rInputStream, m_texcoord_set & rTexCoordSet);