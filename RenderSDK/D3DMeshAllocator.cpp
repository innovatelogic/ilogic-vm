#include "StdafxRender.h"

//----------------------------------------------------------------------------------------------
D3DMeshAllocator::D3DMeshAllocator(const class D3DDriver * Interface)
: D3DMesh(Interface)
{

}

//----------------------------------------------------------------------------------------------
D3DMeshAllocator::D3DMeshAllocator(const D3DMeshAllocator &Sender)
: D3DMesh(Sender)
{

}

//----------------------------------------------------------------------------------------------
D3DMeshAllocator::~D3DMeshAllocator()
{

}

//----------------------------------------------------------------------------------------------
bool D3DMeshAllocator::LoadFromDesc(const SMeshMaterialSet &Desc)
{
	bool bResult = false;

	// read object file
	std::string sFilename;

	if (FindFile(Desc.ObjectFile.c_str(), &sFilename))
	{
		// try to find already initialized scenes
		// recreate full mesh tree replica with binded materials
		if (CSceneMeshNode *pScene = m_pD3DInterface->GetSceneMeshNode(sFilename.c_str()))
		{
			std::vector<SMeshNodesBank> &VBank = pScene->GetMeshBank();

			for (std::vector<SMeshNodesBank>::iterator Iter = VBank.begin(); Iter != VBank.end(); ++Iter)
			{
				m_VMeshEffectBank.push_back(SMeshEffectBank());

				for (TSVMeshNodes::iterator IterLod = (*Iter).VBankNodes.begin();
						IterLod != (*Iter).VBankNodes.end(); ++IterLod)
				{
					const size_t nLastIdx = m_VMeshEffectBank.size() - 1;
					m_VMeshEffectBank[nLastIdx].VBankNodes.push_back(SVecSubMeshes());

					TVecMeshes &VNodes = (*IterLod).MeshNodes;

					for (TVecMeshes::iterator Iter = VNodes.begin(); Iter != VNodes.end(); ++Iter)
					{
						SMeshEffectPair MeshEffect;
						MeshEffect.SubMesh = *Iter;
						MeshEffect.Effect = ApplyMeshMaterial(MeshEffect.SubMesh->m_materialID, Desc);
						
						m_VMeshEffectBank[nLastIdx].VBankNodes[
							m_VMeshEffectBank[nLastIdx].VBankNodes.size() - 1
						].VSubMeshes.push_back(MeshEffect);
					}
				}
			}

			m_pSceneMesh = pScene;
			m_pSceneMesh->AddRef();
		}
		else
		{
			Clear(); // clear already

			m_pSceneMesh = m_pD3DInterface->AllocSceneMeshNode(sFilename.c_str());

			m_file oFile;
			if (oFile.open(sFilename.c_str()) == m_file::NO_FILE_ERROR)
			{
				char * sFileHeader = new char[4];
				sFileHeader[3] = '\0';
				oFile.read(sFileHeader, 3);

				if (!strcmp(sFileHeader, "OES"))
				{
					unsigned int nMajorVersion;
					unsigned int nMinorVersion;

					oFile >> nMajorVersion >> nMinorVersion;

					if (nMajorVersion == 1 && nMinorVersion == 0){
						LoadFile_v1_0(oFile, Desc);
					}
					else{
						LoadFile_v1_2(oFile, Desc);
					}
					
					delete [] sFileHeader;
				}
				oFile.close();
			}
		}
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
BaseMaterial* D3DMeshAllocator::ApplyMeshMaterial(int materialID, const SMeshMaterialSet &MaterialSet)
{
	BaseMaterial *pOutMateral = 0;

	// init material
	if (materialID != INDEX_NONE)
	{
		for (size_t IndexMaterial = 0; IndexMaterial < MaterialSet.Materials.size(); ++IndexMaterial)
		{
			if (MaterialSet.Materials[IndexMaterial].id == materialID)
			{
				MaterialEffect *pMatEffect = new MaterialEffect(m_pD3DInterface);

				for (size_t TextureIdx = 0; TextureIdx < MaterialSet.Materials[IndexMaterial].textures.size(); ++TextureIdx)
				{
					// diffuse
					if (TextureIdx == 0 && MaterialSet.Materials[IndexMaterial].textures[TextureIdx] < MaterialSet.Textures.size())
					{
						std::string sFilename;
						if (FindFile(MaterialSet.Textures[MaterialSet.Materials[IndexMaterial].textures[TextureIdx]].c_str(), &sFilename))
						{
							pMatEffect->LoadDiffuseMap(sFilename.c_str());
						}
					}

					// normal
					if (TextureIdx == 1 && MaterialSet.Materials[IndexMaterial].textures[TextureIdx] < MaterialSet.Textures.size())
					{
						std::string sFilename;
						if (FindFile(MaterialSet.Textures[MaterialSet.Materials[IndexMaterial].textures[TextureIdx]].c_str(), &sFilename))
						{
							pMatEffect->LoadNormalMap(sFilename.c_str());
						}
					}
				}

				pMatEffect->SetReference(MaterialSet.Materials[IndexMaterial].FX.c_str());
				pMatEffect->SetTechniqueName(MaterialSet.Materials[IndexMaterial].tehnique.c_str());
				pMatEffect->transperent = MaterialSet.Materials[IndexMaterial].transparent;

				std::string sFilenameFx;
				pMatEffect->Load(FindFile(MaterialSet.Materials[IndexMaterial].FX.c_str(), &sFilenameFx) ? sFilenameFx.c_str() : "");

				pOutMateral = pMatEffect;
				break;
			}
		}
	}
	return pOutMateral;
}

//----------------------------------------------------------------------------------------------
void D3DMeshAllocator::DeserializeMesh(m_file &oFile, SubMeshNode *pInMesh, unsigned int &numDistinctBones,	std::vector<int> &VecBoneRemapper)
{
	// temp variables similar to serialization
	// geometry...
	unsigned int    num_vertices;		// number of vertices
	Vector			*vertices;			// array of vertices
	Vector			*normals;			// array of normals
	Vector4f		*colors;			// array of colors (vertex colors)
	bool            skin;				// flag to tell if the vertices are to be skinned
	Vector4f		*weights;			// vertex weights
	unsigned int	*bone_idxs;			// 4 bones per vertex.

	// array of texcoord sets...
	unsigned int    num_texcoord_sets;  // number of texture coordinate sets
	m_texcoord_set	*texcoord_sets;     // array of texcoord_sets - they all contain 

	// topology...
	unsigned int    num_faces;			// numbers of triangle face
	unsigned int	*faces_idx;			// array of vertex indices - faces

	m_attribute     mesh_attr; 

	// bounding box information...
	Vector          aabb_min;
	Vector          aabb_max;

	// read
	oFile >> pInMesh->m_materialID;

	//pInMesh->m_materialID = 0;

	// Geometry
	oFile >> num_vertices;

	// allocator
	if (0 == num_vertices)
	{
		vertices = 0;
		normals  = 0;
		colors   = 0;
	}
	else
	{
		vertices = new Vector[num_vertices];
		normals  = new Vector[num_vertices];
		colors   = new Vector4f[num_vertices];
	}

	unsigned int iVertex;
	for (iVertex = 0; iVertex < num_vertices; ++iVertex)
	{
		oFile >> vertices[iVertex];
		oFile >> normals[iVertex];
		oFile >> colors[iVertex];
	}

	// Skinning
	oFile >> skin;

	if (skin)
	{
		weights   = new Vector4f[num_vertices];
		bone_idxs = new unsigned int[4 * num_vertices];

		for (iVertex = 0; iVertex < num_vertices; ++iVertex)
		{
			oFile >> weights[iVertex];
			oFile >> bone_idxs[4 * iVertex + 0] >> 
				bone_idxs[4 * iVertex + 1] >> 
				bone_idxs[4 * iVertex + 2] >> 
				bone_idxs[4 * iVertex + 3]; 
		}
	}
	else
	{
		weights   = 0;
		bone_idxs = 0;
	}

	// Texture coordinate sets
	oFile >> num_texcoord_sets;
	texcoord_sets = (0 == num_texcoord_sets) ? NULL : new m_texcoord_set[num_texcoord_sets];

	for (unsigned int iTexCoordSet = 0; iTexCoordSet < num_texcoord_sets; ++iTexCoordSet)
	{
		oFile >> texcoord_sets[iTexCoordSet];
	}

	// Face information
	oFile >> num_faces;
	faces_idx = (0 == num_faces) ? 0 : new unsigned int[3 * num_faces];

	for (iVertex = 0; iVertex < num_faces * 3; ++iVertex){
		oFile >> faces_idx[iVertex];
	}

	// Attributes
	oFile >> mesh_attr;

	// Bounding box 
	oFile >> aabb_min >> aabb_max;

	pInMesh->InitializeVertexData(m_pD3DInterface,
		(float*)vertices,
		(float*)normals,
		(float*)colors,
		(float*)texcoord_sets[0].tangents,
		(float*)texcoord_sets[0].binormals,
		faces_idx,
		num_vertices,
		num_faces,
		(float*)texcoord_sets[0].texcoords,
		num_texcoord_sets,
		skin,
		(float*)weights,
		bone_idxs,
		VecBoneRemapper,
		numDistinctBones);

	delete [] vertices;			// array of vertices
	delete [] normals;			// array of normals
	delete [] colors;			// array of colors (vertex colors)

	delete [] weights;			// vertex weights
	delete [] bone_idxs;		// 4 bones per vertex.

	// array of texcoord sets...
	delete [] texcoord_sets;    // array of texcoord_sets - they all contain 
	delete [] faces_idx;		// array of vertex indices - faces
}

//----------------------------------------------------------------------------------------------
void D3DMeshAllocator::LoadFile_v1_0(m_file &oFile, const SMeshMaterialSet &Desc)
{
	unsigned int nNameLength;
	oFile >> nNameLength;

	char *_name = new char[nNameLength + 1];

	oFile.read(_name, nNameLength);
	_name[nNameLength] = '\0';

	std::string FileObject = _name;
	delete [] _name;

	unsigned int nNumMeshes = 0;
	oFile >> nNumMeshes;

	std::vector<SMeshNodesBank> &VBank = m_pSceneMesh->GetMeshBank();

	//initialize bone remapper
	unsigned int numDistinctBones = 0;
	std::vector<int> VecBoneRemapper;

	VecBoneRemapper.reserve(512);
	for (size_t IndexRemap = 0; IndexRemap < 512; ++IndexRemap){
		VecBoneRemapper.push_back(INDEX_NONE);
	}

	VBank.push_back(SMeshNodesBank());
	m_VMeshEffectBank.push_back(SMeshEffectBank());

	VBank[0].VBankNodes.push_back(SVMeshNodes());
	m_VMeshEffectBank[0].VBankNodes.push_back(SVecSubMeshes());

	for (unsigned int Index = 0; Index < nNumMeshes; ++Index)
	{
		SubMeshNode *pSubMeshNode = new SubMeshNode(m_pD3DInterface);

		DeserializeMesh(oFile, pSubMeshNode, numDistinctBones, VecBoneRemapper);

		VBank[0].VBankNodes[0].MeshNodes.push_back(pSubMeshNode);

		// init material pair
		SMeshEffectPair MeshEffect;		
		MeshEffect.SubMesh = pSubMeshNode;
		MeshEffect.Effect = ApplyMeshMaterial(MeshEffect.SubMesh->m_materialID, Desc);

		m_VMeshEffectBank[0].VBankNodes[0].VSubMeshes.push_back(MeshEffect);
	}

	Vector scene_aabb_min, scene_aabb_max;
	Vector models_aabb_min, models_aabb_max;

	// Bounding box 
	oFile >> scene_aabb_min >> scene_aabb_max;
	oFile >> models_aabb_min >> models_aabb_max;

	m_pSceneMesh->SetSceneBBox(scene_aabb_min.x, scene_aabb_min.y, scene_aabb_min.z, scene_aabb_max.x, scene_aabb_max.y, scene_aabb_max.z);
	m_pSceneMesh->SetModelsBBox(models_aabb_min.x, models_aabb_min.y, models_aabb_min.z, models_aabb_min.x, models_aabb_min.y, models_aabb_min.z);
}

//----------------------------------------------------------------------------------------------
void D3DMeshAllocator::LoadFile_v1_2(m_file &oFile, const SMeshMaterialSet &Desc)
{	
	std::vector<SMeshNodesBank> &VBank = m_pSceneMesh->GetMeshBank();

	unsigned int nNameLength;
	oFile >> nNameLength;

	char *_name = new char[nNameLength + 1];

	oFile.read(_name, nNameLength);
	_name[nNameLength] = '\0';

	std::string FileObject = _name;
	delete [] _name;

	unsigned int nNumBanks = 0;
	oFile >> nNumBanks;
	
	VBank.reserve(nNumBanks);
	m_VMeshEffectBank.reserve(nNumBanks);

	//initialize bone remapper
	unsigned int numDistinctBones = 0;
	std::vector<int> VecBoneRemapper;

	VecBoneRemapper.reserve(512);

	for (unsigned int Index = 0; Index < nNumBanks; ++Index)
	{
		VBank.push_back(SMeshNodesBank());
		m_VMeshEffectBank.push_back(SMeshEffectBank());

		unsigned int nNumLods = 0;
		oFile >> nNumLods;

		VBank[Index].VBankNodes.reserve(nNumLods);
		m_VMeshEffectBank[Index].VBankNodes.reserve(nNumLods);

		for (unsigned int Index2 = 0; Index2 < nNumLods; ++Index2)
		{
			VBank[Index].VBankNodes.push_back(SVMeshNodes());
			m_VMeshEffectBank[Index].VBankNodes.push_back(SVecSubMeshes());

			unsigned int nNumMeshes = 0;
			oFile >> nNumMeshes;

			for (unsigned int IndexMesh = 0; IndexMesh < nNumMeshes; ++IndexMesh)
			{
				SubMeshNode *pSubMeshNode = new SubMeshNode(m_pD3DInterface);

				for (size_t IndexRemap = 0; IndexRemap < 512; ++IndexRemap){
					VecBoneRemapper.push_back(INDEX_NONE);
				}

				DeserializeMesh(oFile, pSubMeshNode, numDistinctBones, VecBoneRemapper);

				VBank[Index].VBankNodes[Index2].MeshNodes.push_back(pSubMeshNode);
				
				// init material pair
				SMeshEffectPair MeshEffect;		
				MeshEffect.SubMesh = pSubMeshNode;
				MeshEffect.Effect = ApplyMeshMaterial(MeshEffect.SubMesh->m_materialID, Desc);

				m_VMeshEffectBank[Index].VBankNodes[Index2].VSubMeshes.push_back(MeshEffect);
			}
		}
	}

	Vector scene_aabb_min, scene_aabb_max;
	Vector models_aabb_min, models_aabb_max;

	// Bounding box 
	oFile >> scene_aabb_min >> scene_aabb_max;
	oFile >> models_aabb_min >> models_aabb_max;

	m_pSceneMesh->SetSceneBBox(models_aabb_min.x, models_aabb_min.y, models_aabb_min.z,
							   models_aabb_max.x, models_aabb_max.y, models_aabb_max.z);
	m_pSceneMesh->SetModelsBBox(models_aabb_min.x, models_aabb_min.y, models_aabb_min.z,
								models_aabb_max.x, models_aabb_max.y, models_aabb_max.z);
}