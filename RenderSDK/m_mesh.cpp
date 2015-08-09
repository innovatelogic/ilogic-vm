#include "m_mesh.h"
#include "m_model.h"

//------------------------------------------------------------------------
m_mesh::m_mesh()
: material_id(BAD_IDX)
, num_vertices(0)
, normals(0)
, colors(0)
, skin(false)
, weights(0)        // vertex weights
, bone_idxs(0)      // 4 bones per vertex.
, num_texcoord_sets(0)
, texcoord_sets(0)
, num_faces(0)
, faces_idx(0)
, aabb_min(-_FLT_MAX, -_FLT_MAX, -_FLT_MAX) // bounding box information...
, aabb_max(_FLT_MAX, _FLT_MAX, _FLT_MAX)
{
}

//------------------------------------------------------------------------
m_mesh::~m_mesh()
{
	if (num_vertices)
	{
		delete [] vertices;
		num_vertices = 0;
		vertices = 0;
	}

	if (normals)
	{
		delete [] normals;
		normals = 0;
	}

	if (colors)
	{
		delete [] colors;
		colors = 0;
	}

	if (num_texcoord_sets)
	{
		delete [] texcoord_sets;
		texcoord_sets = 0;
		num_texcoord_sets = 0;
	}

	if (num_faces)
	{
		delete [] faces_idx;
		num_faces = 0;
	}

	if (weights)
	{
		delete [] weights;
		weights = 0;
	}

	if (bone_idxs)
	{
		delete [] bone_idxs;
		bone_idxs = 0;
	}
}

//------------------------------------------------------------------------
BOOL m_mesh::SplitTriangle(int iFace, const int& new_triangle_count,const Vector* new_triangles)
{
    BOOL bResult = FALSE;

    Vector       * new_vertices = NULL;      
    unsigned int * new_faces_idx = NULL;  

    unsigned int   new_num_faces_idx = (num_faces-1)*3 + new_triangle_count*3 ;   
    unsigned int   new_num_vert = num_vertices + new_triangle_count * 3; //new_triangle_count-1; 

    new_vertices  = new Vector[new_num_vert];
    new_faces_idx = new unsigned int[new_num_faces_idx];

    int shift = 0;

    // copy index buffer
    for (size_t iVertex = 0; iVertex < num_faces*3-shift; iVertex+=3)
    {
        if (iVertex == iFace){
            shift = 3; //new_triangle_count*3;
        }

        new_faces_idx[iVertex+ 0] = faces_idx[iVertex+ 0 + shift];
        new_faces_idx[iVertex+ 1] = faces_idx[iVertex+ 1 + shift];
        new_faces_idx[iVertex+ 2] = faces_idx[iVertex+ 2 + shift];
    }

    //copy vertex buffer
    for (unsigned int index = 0; index < num_vertices; ++index)
    {
        new_vertices[index] = vertices[index];
    } 

    //add new vertixes
    for (int iVert = 0; iVert < new_triangle_count*3; ++iVert)
    {
         new_vertices[num_vertices + iVert] = new_triangles[iVert];
    }
    
    if (new_triangle_count == 2)
    {
        new_faces_idx[(num_faces-1)*3 + 0] = num_vertices;
        new_faces_idx[(num_faces-1)*3 + 1] = num_vertices+1;
        new_faces_idx[(num_faces-1)*3 + 2] = num_vertices+2;

        new_faces_idx[(num_faces-1)*3 + 3] = num_vertices+3;
        new_faces_idx[(num_faces-1)*3 + 4] = num_vertices+4;
        new_faces_idx[(num_faces-1)*3 + 5] = num_vertices+5;
    }
    else if (new_triangle_count == 3)
    {
       new_faces_idx[(num_faces-1)*3 + 0] = num_vertices;
       new_faces_idx[(num_faces-1)*3 + 1] = num_vertices+1;
       new_faces_idx[(num_faces-1)*3 + 2] = num_vertices+2;

       new_faces_idx[(num_faces-1)*3 + 3] = num_vertices+3;
       new_faces_idx[(num_faces-1)*3 + 4] = num_vertices+4;
       new_faces_idx[(num_faces-1)*3 + 5] = num_vertices+5;

       new_faces_idx[(num_faces-1)*3 + 6] = num_vertices+6;
       new_faces_idx[(num_faces-1)*3 + 7] = num_vertices+7;
       new_faces_idx[(num_faces-1)*3 + 8] = num_vertices+8;
    }

	num_vertices = new_num_vert;
    num_faces = new_num_faces_idx/3;

    delete [] vertices;
    delete [] faces_idx;

    vertices = new_vertices;
    faces_idx = new_faces_idx;
    return bResult;
}


//------------------------------------------------------------------------
bool m_mesh::calculateTangentSpaces(unsigned int iUVSet)
{
	// Calculate neighbor information for every vertex,
	// consisting of an array with all adjacent vertices.

	std::vector< std::set<unsigned int> > aAdjacentVertices(num_vertices);

	for (unsigned int iTriangle = 0; iTriangle < num_faces; ++iTriangle)
	{
		aAdjacentVertices[faces_idx[3*iTriangle + 0]].insert(faces_idx[3*iTriangle + 1]);
		aAdjacentVertices[faces_idx[3*iTriangle + 0]].insert(faces_idx[3*iTriangle + 2]);

		aAdjacentVertices[faces_idx[3*iTriangle + 1]].insert(faces_idx[3*iTriangle + 2]);
		aAdjacentVertices[faces_idx[3*iTriangle + 1]].insert(faces_idx[3*iTriangle + 0]);

		aAdjacentVertices[faces_idx[3*iTriangle + 2]].insert(faces_idx[3*iTriangle + 0]);
		aAdjacentVertices[faces_idx[3*iTriangle + 2]].insert(faces_idx[3*iTriangle + 1]);
	}

	// Calculate a deltaU for each vertex and use it as weight
	// for each vector from the center vertex to the neighboring
	// vertex.
	for (unsigned int iVertex = 0; iVertex < num_vertices; ++iVertex)
	{
		for (unsigned int iUVSet = 0; iUVSet < num_texcoord_sets; ++iUVSet)
		{
			texcoord_sets[iUVSet].tangents[iVertex] = Vector(0.0f, 0.0f, 0.0f);

		/*	for (set<nv_idx>::const_iterator iAdjacent = aAdjacentVertices[iVertex].begin();
				iAdjacent != aAdjacentVertices[iVertex].end();
				++iAdjacent)
			{
				float nDeltaU = texcoord_sets[iUVSet].texcoords[2*(*iAdjacent)] 
				- texcoord_sets[iUVSet].texcoords[2*iVertex];
				vec3  vDeltaP = vertices[*iAdjacent] - vertices[iVertex];

				texcoord_sets[iUVSet].tangents[iVertex] += (nDeltaU / dot(vDeltaP, vDeltaP)) * vDeltaP;
			}
			// Calculate the binormal as the cross-product of the tangent
			// and the normal.
			cross(texcoord_sets[iUVSet].binormals[iVertex], 
				normals[iVertex], 
				texcoord_sets[iUVSet].tangents[iVertex]);
			// If degenerate case we try alternative tangents, 
			// until we're successful.
			float nLength = nv_norm(texcoord_sets[iUVSet].binormals[iVertex]);
			while (nLength == 0.0f)
			{
				texcoord_sets[iUVSet].tangents[iVertex] += vec3(0.1f, -0.2f, 0.3f);
				cross(texcoord_sets[iUVSet].binormals[iVertex], 
					normals[iVertex], 
					texcoord_sets[iUVSet].tangents[iVertex]);
				nLength = nv_norm(texcoord_sets[iUVSet].binormals[iVertex]);
			}
			// Normalize the binormal.
			scale(texcoord_sets[iUVSet].binormals[iVertex], 1.0f/nLength);
			// Recalculate the tangent as the cross-product of the binormal and
			// the normal.
			cross(texcoord_sets[iUVSet].tangents[iVertex], 
				texcoord_sets[iUVSet].binormals[iVertex],
				normals[iVertex]);*/
		}
	}

	return true;
}

//------------------------------------------------------------------------
m_output_stream & operator << (m_output_stream & rOutputStream, const m_mesh & rMesh)
{
    rOutputStream << rMesh.material_id;

	rOutputStream << rMesh.num_vertices;

	unsigned int iVertex;

	for (iVertex = 0; iVertex < rMesh.num_vertices; ++iVertex)
	{
		rOutputStream << rMesh.vertices[iVertex];
		rOutputStream << rMesh.normals[iVertex];
		rOutputStream << rMesh.colors[iVertex];
	}

	rOutputStream << rMesh.skin;

	if (rMesh.skin)
	{
		for (iVertex = 0; iVertex < rMesh.num_vertices; ++iVertex )
		{
           rOutputStream << rMesh.weights[iVertex];
		   rOutputStream <<	rMesh.bone_idxs[4*iVertex + 0] << rMesh.bone_idxs[4*iVertex+1] 
		                 << rMesh.bone_idxs[4*iVertex + 2] << rMesh.bone_idxs[4*iVertex+3];

		}
	}

	// Texture coordinate sets
	rOutputStream << rMesh.num_texcoord_sets;
	for (unsigned int iTexCoordSet = 0; iTexCoordSet < rMesh.num_texcoord_sets; ++iTexCoordSet)
	{
		// texcoord_set doesn't know the length of it's arrays,
		// so we have to pass it here.
		rMesh.texcoord_sets[iTexCoordSet].num_coords = rMesh.num_vertices;
		rOutputStream << rMesh.texcoord_sets[iTexCoordSet];
	}

	// Face information
	rOutputStream << rMesh.num_faces;
	for (iVertex = 0; iVertex < rMesh.num_faces*3; ++iVertex)
		rOutputStream << rMesh.faces_idx[iVertex];

	// Attributes
	rOutputStream << rMesh.mesh_attr;

	// Bounding box 
	rOutputStream << rMesh.aabb_min << rMesh.aabb_max;

	return rOutputStream;
}

//------------------------------------------------------------------------
m_input_stream & operator >> (m_input_stream & rInputStream, m_mesh & rMesh)
{
	rInputStream >> rMesh.material_id;

	// Geometry
	rInputStream >> rMesh.num_vertices;

	if (0 == rMesh.num_vertices)
	{
		rMesh.vertices = 0;
		rMesh.normals  = 0;
		rMesh.colors   = 0;
	}
	else
	{
		rMesh.vertices = new Vector[rMesh.num_vertices];
		rMesh.normals  = new Vector[rMesh.num_vertices];
		rMesh.colors   = new Vector4f[rMesh.num_vertices];
	}

	unsigned int iVertex;
	for (iVertex = 0; iVertex < rMesh.num_vertices; ++iVertex)
	{
		rInputStream >> rMesh.vertices[iVertex];
		rInputStream >> rMesh.normals[iVertex];
		rInputStream >> rMesh.colors[iVertex];
	}

	// Skinning
	rInputStream >> rMesh.skin;

	if (rMesh.skin)
	{
		rMesh.weights   = new Vector4f[rMesh.num_vertices];
		rMesh.bone_idxs = new unsigned int[4 * rMesh.num_vertices];

		for (iVertex = 0; iVertex < rMesh.num_vertices; ++iVertex)
		{
			rInputStream >> rMesh.weights[iVertex];
			rInputStream >> rMesh.bone_idxs[4*iVertex + 0] >> rMesh.bone_idxs[4*iVertex + 1]
						 >> rMesh.bone_idxs[4*iVertex + 2] >> rMesh.bone_idxs[4*iVertex + 3]; 
		}
	}
	else
	{
		rMesh.weights   = 0;
		rMesh.bone_idxs = 0;
	}

	// Texture coordinate sets
	rInputStream >> rMesh.num_texcoord_sets;
	rMesh.texcoord_sets = (0 == rMesh.num_texcoord_sets) ? NULL : new m_texcoord_set[rMesh.num_texcoord_sets];

	for (unsigned int iTexCoordSet = 0; iTexCoordSet < rMesh.num_texcoord_sets; ++iTexCoordSet)
	{
		rInputStream >> rMesh.texcoord_sets[iTexCoordSet];
	}

	// Face information
	rInputStream >> rMesh.num_faces;
	rMesh.faces_idx = (0 == rMesh.num_faces) ? 0 : new unsigned int[3 * rMesh.num_faces];

	for (iVertex = 0; iVertex < rMesh.num_faces * 3; ++iVertex){
		rInputStream >> rMesh.faces_idx[iVertex];
	}

	// Attributes
	rInputStream >> rMesh.mesh_attr;

	// Bounding box 
	rInputStream >> rMesh.aabb_min >> rMesh.aabb_max;

    return rInputStream;
}