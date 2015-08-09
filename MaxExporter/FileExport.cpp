#include "expstdafx.h"

extern bool	bExportSkeleton;
extern bool	bExportModel;
extern bool	bExportAnim;
extern bool bExportCollision;

//----------------------------------------------------------------------------------
bool CompleteExport(const char * filename)
{
	m_scene &scene = ExporterMAX::GetExporter()->GetSceneRef();
	scene.name = filename;

	// write number of mesh banks
	unsigned int nNumBanks = ExporterMAX::GetExporter()->m_VMeshBanks.size();

	for (size_t Index = 0; Index < nNumBanks; ++Index)
	{
		unsigned int nNumLods = ExporterMAX::GetExporter()->m_VMeshBanks[Index].VBankNodes.size();
		for (size_t Index2 = 0; Index2 < nNumLods; ++Index2)
		{
			std::vector<m_node*> &VRef = ExporterMAX::GetExporter()->m_VMeshBanks[Index].VBankNodes[Index2].VNodes; 
			for (size_t iNode = 0; iNode < VRef.size(); ++iNode)
			{
				m_node *node = VRef[iNode];

				if (VRef[iNode]->get_type() == m_node::GEOMETRY)
				{
					m_model *model = reinterpret_cast<m_model*>(node);

					if (model->meshes.size() == 0)
					{
						// aabb min...
						if (model->xform.x < model->aabb_min.x)
							model->aabb_min.x = model->xform.x;
						if (model->xform.y < model->aabb_min.y)
							model->aabb_min.y = model->xform.y;
						if (model->xform.z < model->aabb_min.z)
							model->aabb_min.z = model->xform.z;

						// aabb max...
						if (model->xform.x > model->aabb_max.x)
							model->aabb_max.x = model->xform.x;
						if (model->xform.y > model->aabb_max.y)
							model->aabb_max.y = model->xform.y;
						if (model->xform.z > model->aabb_max.z)
							model->aabb_max.z = model->xform.z;
					}

					// scene bounding box computation...
					// computated as the union of the bounding
					// boxes of all the models
					if (scene.aabb_min.x > model->aabb_min.x )
						scene.aabb_min.x = model->aabb_min.x;
					if (scene.aabb_min.y > model->aabb_min.y )
						scene.aabb_min.y = model->aabb_min.y;
					if (scene.aabb_min.z > model->aabb_min.z )
						scene.aabb_min.z = model->aabb_min.z;

					if (scene.aabb_max.x < model->aabb_max.x )
						scene.aabb_max.x = model->aabb_max.x;
					if (scene.aabb_max.y < model->aabb_max.y )
						scene.aabb_max.y = model->aabb_max.y;
					if (scene.aabb_max.z < model->aabb_max.z )
						scene.aabb_max.z = model->aabb_max.z;

					// update the model aabb bbox only...
					if (scene.models_aabb_min.x > model->aabb_min.x )
						scene.models_aabb_min.x = model->aabb_min.x;
					if (scene.models_aabb_min.y > model->aabb_min.y )
						scene.models_aabb_min.y = model->aabb_min.y;
					if (scene.models_aabb_min.z > model->aabb_min.z )
						scene.models_aabb_min.z = model->aabb_min.z;

					if (scene.models_aabb_max.x < model->aabb_max.x )
						scene.models_aabb_max.x = model->aabb_max.x;
					if (scene.models_aabb_max.y < model->aabb_max.y )
						scene.models_aabb_max.y = model->aabb_max.y;
					if (scene.models_aabb_max.z < model->aabb_max.z )
						scene.models_aabb_max.z = model->aabb_max.z;
				}
				else
				{
					// scene bounding box computation...
					// computated as the union of the bounding
					// boxes of all the dummies, lights, bones...
					if (scene.aabb_min.x > node->xform.x )
						scene.aabb_min.x = node->xform.x;
					if (scene.aabb_min.y > node->xform.y )
						scene.aabb_min.y = node->xform.y;
					if (scene.aabb_min.z > node->xform.z )
						scene.aabb_min.z = node->xform.z;
					if (scene.aabb_max.x < node->xform.x )
						scene.aabb_max.x = node->xform.x;
					if (scene.aabb_max.y < node->xform.y )
						scene.aabb_max.y = node->xform.y;
					if (scene.aabb_max.z < node->xform.z )
						scene.aabb_max.z = node->xform.z;
				}
			}
		}
	}

    // materials
	for (size_t Index = 0; Index < ExporterMAX::GetExporter()->GetNumMaterials(); ++Index){
		scene.materials.push_back(new m_material);
	}
    
    const MatIdxMap &MatMap = ExporterMAX::GetExporter()->GetMaterials();
   
	MatIdxMapCIt it = MatMap.begin();
    while (it != MatMap.end())
    {
	    scene.materials[(*it).second] = (*it).first;
	    ++it;
    }

	// textures
	for (size_t i = 0; i < ExporterMAX::GetExporter()->GetTexturesRef().size(); ++i)
	{
		m_texture * texture = new m_texture();
		texture = (ExporterMAX::GetExporter()->GetTexturesRef()[i]);
		scene.textures.push_back(texture);
	}

	// properties
	scene.ambient = Vector4f(0.f, 0.f, 0.f, 0.f); // ambient color

	// animation...
	TimeValue t0 = ExporterMAX::GetExporter()->GetIGame()->GetSceneStartTime();
	TimeValue t1 = ExporterMAX::GetExporter()->GetIGame()->GetSceneEndTime();
	TimeValue DeltaTime = ExporterMAX::GetExporter()->GetIGame()->GetSceneTicks();

	int SamplingRate = 1;

	int numkeys = (t1 - t0) / (DeltaTime * SamplingRate);
	scene.num_keys = numkeys;

	return true;
}

//----------------------------------------------------------------------------------
bool CompleteExportSkeletonTree(const char * filename)
{
	bool bResult = false;
	/*
	std::stringstream ss;
	std::ofstream os;

	if (ExporterMAX::GetExporter()->Skeleton.GetNumBones() > 0)
	{
		ss << "<Comp_SkeletonObject>" << std::endl;

		ExporterMAX::GetExporter()->Skeleton.Serialize(ss);

		ss << "</Comp_SkeletonObject>" << std::endl;

		int IndexFind = std::string(filename).rfind('.');
		std::string nfilename = std::string(filename).substr(0, IndexFind) + ".hsk";

		os.open(nfilename.c_str(), std::ios::out);
		os << ss.str();
		os.flush();
		os.close();

		bResult = true;
	}*/
	return bResult;
}

//----------------------------------------------------------------------------------
bool CompleteExportAnim(const char * filename)
{
	bool bResult = false;
/*
	std::stringstream ss;
	std::ofstream os;

	if (ExporterMAX::GetExporter()->Track.GetNumFrames() > 0)
	{
		ExporterMAX::GetExporter()->Skeleton.SerializeXForms(ss);

		ExporterMAX::GetExporter()->Track.WriteToStream(ss);
		
		int IndexFind = std::string(filename).rfind('.');
		std::string nfilename = std::string(filename).substr(0, IndexFind) + ".trk";

		os.open(nfilename.c_str(), std::ios::out);
		
		os << ss.str();
		os.flush();
		os.close();

		bResult = true;
	}
	*/
	return bResult;
}

//----------------------------------------------------------------------------------
bool CompleteExportMat(const char *filename)
{
	bool bResult = false;

	std::stringstream ss;
	std::ofstream os;

	if (ExporterMAX::GetExporter()->m_MapMaterials.size() > 0)
	{
		const int IndexFind = std::string(filename).rfind('.');
		std::string nfilename = std::string(filename).substr(0, IndexFind);

		size_t idxSlash = nfilename.rfind('\\');

		if (idxSlash == std::string::npos){
			idxSlash = nfilename.rfind('/');
		}

		if (idxSlash != std::string::npos){
			nfilename = nfilename.substr(idxSlash + 1, INDEX_NONE);
		}

		ss << "<lactor>" << std::endl;
		{
			ss << "<mesh obj=\"" << (nfilename + ".obj") << "\" " <<
						"col=\"" << (bExportCollision ? ((nfilename + ".col")) : "") << "\">" << std::endl;
			{
				// texture
				{
					ss << "<textures>" << std::endl;

					for (size_t Index = 0; Index < ExporterMAX::GetExporter()->m_VTextures.size(); ++Index)	{
						ss << "<tex name=\"" << ExporterMAX::GetExporter()->m_VTextures[Index]->name << "\"" << "/>"<< std::endl;
					}
					ss << "</textures>" << std::endl;
				}

				ss << "<mtrls>" << std::endl;
				{
					GMatMapIt it = ExporterMAX::GetExporter()->ExporterMAX::GetExporter()->m_MapMaterials.begin();
					while (it != ExporterMAX::GetExporter()->m_MapMaterials.end())
					{
						ss << "<mat ";
						ss << "name=\"" << it->second->name << "\" ";
						ss << "fx=\"" << it->second->fx_file << "\" ";
						ss << "tehnique=\"" << it->second->tehnique << "\" ";
						ss << "id=\"" << it->second->id << "\" ";
						
						ss << "txids=\"";
							for (size_t iTexture = 0; iTexture < it->second->textures.size(); ++iTexture){
								ss << it->second->textures[iTexture] << ";";
							}
						ss << "\" ";

						ss << "txchs=\"";
							for (size_t iTexture = 0; iTexture < it->second->tex_channel.size(); ++iTexture){
								ss << it->second->tex_channel[iTexture] << ";";
							}
						ss << "\" ";

						ss << "diffuse=\"" << it->second->diffuse.x << ";" <<
												it->second->diffuse.y << ";" <<
												it->second->diffuse.z << ";" <<
												it->second->diffuse.w << "\" ";
						
						ss << "specular=\"" << it->second->specular.x << ";" <<
												it->second->specular.y << ";" <<
												it->second->specular.z << ";" <<
												it->second->specular.w << "\" ";

						ss << "shininess=\"" << it->second->shininess << "\" ";

						ss << "ambient=\"" << it->second->ambient.x << ";" <<
												it->second->ambient.y << ";" <<
												it->second->ambient.z << ";" <<
												it->second->ambient.w << "\" ";
						
						ss << "emission=\"" << it->second->emission.x << ";" <<
												it->second->emission.y << ";" <<
												it->second->emission.z << ";" <<
												it->second->emission.w << "\" ";

						ss << "transparent=\"" << it->second->transparent << "\" ";

						ss << "fog=\"" << it->second->fog << "\" ";
						
						ss << "fogcolor=\"" << it->second->fog_color.x << ";" <<
												it->second->fog_color.y << ";" <<
												it->second->fog_color.z << ";" <<
												it->second->fog_color.w << "\" ";

						ss << "/> " << std::endl;

						++it;
					}
				}
				ss << "</mtrls>" << std::endl;
			}
			ss << "</mesh>" << std::endl;
		}
		ss << "</lactor>" << std::endl;

		std::string nSaveFilename = std::string(filename).substr(0, IndexFind) + ".mat";

		os.open(nSaveFilename.c_str(), std::ios::out);

		if (os)
		{
			os << ss.str();
			os.flush();
			os.close();

			// export collision
			if (bExportCollision){
				CompleteExportCollision(filename);
			}
			
			bResult = true;
		}
	}

	return bResult;
}

//----------------------------------------------------------------------------------
bool CompleteExportObjects(const char *filename)
{
	bool bResult = false;

	ExporterMAX *pExporter = ExporterMAX::GetExporter();

	int IndexFind = std::string(filename).rfind('.');

	std::string nfilename = std::string(filename).substr(0, IndexFind) + ".obj";

	m_scene &scene = pExporter->GetSceneRef();

	scene.name = nfilename;

	// write out to disk
	m_file oFile;

	m_file::teError error = oFile.open(nfilename.c_str(), m_file::WRITE_MODE);

	if (m_file::NO_FILE_ERROR){
		return bResult;
	}

	// Write to output file.
	oFile << 'O' << 'E' << 'S'		  // File identification string 
		<< 1                          // major version number
		<< 1;                         // minor version number

	const m_scene &out_scene = pExporter->GetSceneRef();

	//SaveObjects(oFile, out_scene);

	// Write the scene's name
	unsigned int nNameLength = (unsigned int)out_scene.name.size(); // 0

	oFile << nNameLength;
	
	oFile.write(out_scene.name.c_str(), nNameLength);

	// write number of mesh banks
	unsigned int nNumBanks = pExporter->m_VMeshBanks.size();

	oFile << nNumBanks;

	for (size_t Index = 0; Index < nNumBanks; ++Index)
	{
		unsigned int nNumLods = pExporter->m_VMeshBanks[Index].VBankNodes.size();
	
		oFile << nNumLods;

		for (size_t Index2 = 0; Index2 < nNumLods; ++Index2)
		{
			std::vector<m_node*> &VRef = pExporter->m_VMeshBanks[Index].VBankNodes[Index2].VNodes; 

			// write base meshes
			unsigned int num_meshes = 0;
			for (size_t iNode = 0; iNode < VRef.size(); ++iNode)
			{
				if (VRef[iNode]->get_type() == m_node::GEOMETRY)
				{
					num_meshes += VRef[iNode]->asModel()->meshes.size();
				}
			}

			oFile << num_meshes;

			for (size_t iNode = 0; iNode < VRef.size(); ++iNode)
			{
				if (VRef[iNode]->get_type() == m_node::GEOMETRY)
				{
					for (size_t iMesh = 0; iMesh < VRef[iNode]->asModel()->meshes.size(); ++iMesh)
					{
						oFile << *(VRef[iNode]->asModel()->meshes[iMesh]);
					}
				}
			}
		}
	}

	oFile << out_scene.models_aabb_min << out_scene.models_aabb_max;

	// Bounding boxes.
	oFile << out_scene.aabb_min        << out_scene.aabb_max;
	oFile << out_scene.models_aabb_min << out_scene.models_aabb_max;

	oFile.close();

	bResult = true;

	return bResult;
}

//----------------------------------------------------------------------------------
bool CompleteExportCollision(const char *filename)
{
	bool bResult = false;

	std::ofstream os;
	
	int IndexFind = std::string(filename).rfind('.');
	std::string nfilename = std::string(filename).substr(0, IndexFind) + ".col";

	os.open(nfilename.c_str(), std::ios::out);
	
	os << "<col>" << std::endl;

	for (size_t Index = 0; Index < ExporterMAX::GetExporter()->m_VCollisionObjects.size(); ++Index)
	{
		ExporterMAX::GetExporter()->m_VCollisionObjects[Index]->Serialize(os);
	}

	os << "</col>" << std::endl;

	os.flush();
	os.close();

	bResult = true;

	return bResult;
}