#ifndef _m_mesh_h_ 
#define _m_mesh_h_

#ifdef _WIN32
#pragma once
#endif

#include "m_node.h"
#include "m_texcoord_set.h"

enum EMeshUsageFlag
{
	EMeshGeom_Base = 0,
	EMeshGeom_LOD,
	EMeshGeom_Destr,
};

struct EXPORT m_mesh : public m_node
{
	m_mesh();
	virtual ~m_mesh();

	unsigned int    material_id;		// material id - reference in the 
										// scene material array
	// geometry...
	unsigned int    num_vertices;		// number of vertices
	
	Vector			*vertices;			// array of vertices
	Vector			*normals;			// array of normals
	Vector4f		*colors;			// array of colors (vertex colors)
	                                 
	// skinning...
	bool            skin;				// flag to tell if the vertices are to be skinned
	Vector4f		*weights;			// vertex weights
	unsigned int	*bone_idxs;			// 4 bones per vertex.

	// array of texcoord sets...
	unsigned int    num_texcoord_sets;  // number of texture coordinate sets
	m_texcoord_set	*texcoord_sets;     // array of texcoord_sets - they all contain 
	                                    // num_vertices texture coordinates per set
	// topology...
	unsigned int    num_faces;			// numbers of triangle face
	unsigned int	*faces_idx;			// array of vertex indices - faces
	
	m_attribute     mesh_attr; 

	// bounding box information...
	Vector          aabb_min;
	Vector          aabb_max;

    // split triangle
    BOOL SplitTriangle(int triangle_index, const int& out_triangle_count,const Vector* new_triangles);

	// calculateTangentSpaces
	//
	// Description:
	//      Calculates tangent-space information for a UV set.
	//          The tangen-space information consists of the
	//      per-vertex tangent and binormal vectors stored with
	//      each UV set.
	//
	// Parameters:
	//      iUVSet - UV set index.
	//
	// Returns:
	//      true - on success,
	//      flase - otherwise.
	//
	bool calculateTangentSpaces(unsigned int iUVSet);
};

EXPORT m_output_stream & operator << (m_output_stream & rOutputStream, const m_mesh & rMesh);
EXPORT m_input_stream & operator >> (m_input_stream & rInputStream, m_mesh & rMesh);

#endif   