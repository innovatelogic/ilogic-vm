#include "m_scene.h"
#include "m_node.h"
#include "m_streams.h"
#include "m_light.h"
#include "m_model.h"
#include "m_mesh.h"
#include "m_camera.h"
#include "m_texture.h"
#include "m_material.h"

//-----------------------------------------------------------------------
m_scene::m_scene()
: num_keys(0)
, aabb_min(V_FLT_MAX, V_FLT_MAX, V_FLT_MAX)
, aabb_max(-V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX)
, models_aabb_min(V_FLT_MAX, V_FLT_MAX, V_FLT_MAX)
, models_aabb_max(-V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX)
, ambient(float(0.1f),float(0.1f),float(0.1f),1.f)
{
}

//-----------------------------------------------------------------------
m_scene::~m_scene()
{
	Release();
}

//-----------------------------------------------------------------------
void m_scene::Release()
{
	for (TVecNodesIterator Iter = nodes.begin(); Iter != nodes.end(); ++Iter){
		(*Iter)->release();
	}
	nodes.clear();

	for (TVecTexturesIterator Iter = textures.begin(); Iter != textures.end(); ++Iter){
		delete (*Iter);
	}
	textures.clear();

	for (TVecMaterialsIterator Iter = materials.begin(); Iter != materials.end(); ++Iter){
		delete (*Iter);
	}
	materials.clear();
}

//-----------------------------------------------------------------------
bool m_scene::accept(m_visitor & visitor) const
{
	for (size_t i = 0; i < nodes.size(); ++i)
	{
		// find root nodes
		if (nodes[i]->Parent == NULL)
		{
			if (nodes[i]->accept(*this, visitor) == false)
				return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------
unsigned int m_scene::find_node_idx(const m_node *node)
{
	for (size_t i = 0; i < nodes.size(); i++)
	{
		if(nodes[i] == node)
		{
			return (unsigned int)i;
		}
	}
	return BAD_IDX;
}

//-----------------------------------------------------------------------
m_output_stream & operator << (m_output_stream & rOutputStream, const m_scene & rScene)
{
	// Write the scene's name
	unsigned int nNameLength = (unsigned int)rScene.name.size(); // 0
	
	rOutputStream << nNameLength;
	rOutputStream.write(rScene.name.c_str(), nNameLength);

	// Write out all the nodes
	rOutputStream << (unsigned int)rScene.nodes.size();

	for (size_t iNode = 0; iNode < rScene.nodes.size(); ++iNode)
	{
		rOutputStream << rScene.nodes[iNode]->get_type();

		switch (rScene.nodes[iNode]->get_type())
		{
		case m_node::CAMERA:
			{
				rOutputStream << *(rScene.nodes[iNode]->asCamera());
			}
			break;
		case m_node::GEOMETRY:
			{
				rOutputStream << *(rScene.nodes[iNode]->asModel());
			}
			break;
		case m_node::LIGHT:
			{
				rOutputStream << *(rScene.nodes[iNode]->asLight());
			}   
			break;
		case m_node::ANONYMOUS:
			{
				rOutputStream << *(rScene.nodes[iNode]);
			}
			break;
		default:
			assert(false); // We came across a node we don't know how to serialize
		}
	}

	rOutputStream << (unsigned int)rScene.textures.size();
	for (size_t iTexture = 0; iTexture < rScene.textures.size(); ++iTexture)
		rOutputStream << *rScene.textures[iTexture];

	rOutputStream << (unsigned int)rScene.materials.size();
	for (size_t iMaterial = 0; iMaterial < rScene.materials.size(); ++iMaterial)
		rOutputStream << *rScene.materials[iMaterial];

	rOutputStream << rScene.ambient;

	// Bounding boxes.
	rOutputStream << rScene.aabb_min        << rScene.aabb_max;
	rOutputStream << rScene.models_aabb_min << rScene.models_aabb_max;

	rOutputStream << rScene.num_keys;

	return rOutputStream;
}

//-----------------------------------------------------------------------
m_input_stream & operator >> (m_input_stream & rInputStream, m_scene & rScene)
{
	// Write the scene's name
	unsigned int nNameLength;
	
	rInputStream >> nNameLength;

	char * _name = new char[nNameLength + 1];
	assert(_name);
	rInputStream.read(_name, nNameLength);
	_name[nNameLength] = '\0';
	rScene.name = _name;
	delete _name;

	// Write out all the nodes
	unsigned int num_nodes;
	rInputStream >> num_nodes;
	rScene.nodes.reserve(num_nodes);

	for (unsigned int iNode = 0; iNode < num_nodes; ++iNode)
	{
		m_node::node_type eNodeType;
		
		rInputStream >> reinterpret_cast<unsigned int &>(eNodeType);

		switch (eNodeType)
		{
		  case m_node::CAMERA:
			{
				m_camera * pCamera = new m_camera;
				rInputStream >> *pCamera;
				rScene.nodes.push_back(pCamera);
			}
			break;

		  case m_node::GEOMETRY:
			{
				m_model * pModel = new m_model;
				rInputStream >> *pModel;
				rScene.nodes.push_back(pModel);
			}
			break;

		  case m_node::LIGHT:
			{
				m_light * pLight = new m_light;
				rInputStream >> *pLight;
				rScene.nodes.push_back(pLight);
			}   
			break;

		  case m_node::ANONYMOUS:
			{
				m_node * pNode = new m_node;
				rInputStream >> *pNode;
				rScene.nodes.push_back(pNode);
			}
			break;

		default:
			assert(false); // We came across a node we don't know how to serialize
		}
	}

	unsigned int num_textures;
	rInputStream >> num_textures;

	rScene.textures.reserve(num_textures);

	for (size_t Index = 0; Index < num_textures; ++Index)
	{
		rScene.textures.push_back(new m_texture);
		assert(rScene.textures[Index]);
		rInputStream >> *rScene.textures[Index];
	}

	unsigned int num_materials;
	rInputStream >> num_materials;

	rScene.materials.reserve(num_materials);

	for (unsigned int iMaterial = 0; iMaterial < num_materials; ++iMaterial)
	{
		rScene.materials.push_back(new m_material);
		assert(rScene.materials[iMaterial]);
		rInputStream >> *rScene.materials[iMaterial];
	}

	rInputStream >> rScene.ambient;

	// Bounding boxes.
	rInputStream >> rScene.aabb_min        >> rScene.aabb_max;
	rInputStream >> rScene.models_aabb_min >> rScene.models_aabb_max;

	rInputStream >> rScene.num_keys;
	return rInputStream;
}

//-----------------------------------------------------------------------
m_output_stream& SaveObjects(class m_output_stream & rOutputStream, const m_scene & rScene)
{
	// Write the scene's name
	unsigned int nNameLength = (unsigned int)rScene.name.size(); // 0

	rOutputStream << nNameLength;
	rOutputStream.write(rScene.name.c_str(), nNameLength);

	unsigned int num_meshes = 0;

	for (size_t iNode = 0; iNode < rScene.nodes.size(); ++iNode)
	{
		if (rScene.nodes[iNode]->get_type() == m_node::GEOMETRY)
		{
			num_meshes += rScene.nodes[iNode]->asModel()->meshes.size();
		}
	}

	rOutputStream << num_meshes;

	// write down mapping attributes
	for (size_t iNode = 0; iNode < rScene.nodes.size(); ++iNode)
	{
		if (rScene.nodes[iNode]->get_type() == m_node::GEOMETRY)
		{
			for (size_t iMesh = 0; iMesh < rScene.nodes[iNode]->asModel()->meshes.size(); ++iMesh)
			{
				rOutputStream << *(rScene.nodes[iNode]->asModel()->meshes[iMesh]);
			}
		}
	}
	
	rOutputStream << rScene.models_aabb_min << rScene.models_aabb_max;

	return rOutputStream;
}