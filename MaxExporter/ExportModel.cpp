#include "expstdafx.h"

//----------------------------------------------------------------------------------
bool heavier(const w_offset & a, const w_offset & b)
{
	if (a.weight > b.weight )
		return true;
	return false;
}

//----------------------------------------------------------------------------------
unsigned int Helper_GetNumberOfFaces(IGameMesh * gM, unsigned int & NumFaces, unsigned int & MaxFaceIdx)
{
	MaxFaceIdx = 0;
	NumFaces = gM->GetNumberOfFaces();

	for (size_t i = 0; i < NumFaces; ++i)
	{
		FaceEx * pFace = gM->GetFace(i);
		for (size_t j = 0; j < 3; ++j)
		{
			unsigned int face_idx = pFace->vert[j]; // index into the vertex array

			if (MaxFaceIdx < face_idx){ // find max face index
				MaxFaceIdx = face_idx;             
			}
		}
	}
	return NumFaces;
}

//----------------------------------------------------------------------------------
bool Helper_ProcessFace(IGameMesh * gM,
						unsigned int Index,
						const AffineParts & PRS,
						const Matrix3 & world_to_obj,
						MatFaceMapType & matface_map,
						TriMapType     & tri_map,
						unsigned int & MaxFaceIdx)
{
	FaceEx *pFace		= gM->GetFace(Index);
	IGameMaterial *pIGMat = gM->GetMaterialFromFace(Index);
	bool HasTexVerts	= gM->GetNumberOfTexVerts() ? true : false;
	bool HasCVerts		= gM->GetNumberOfColorVerts() ? true : false;
	unsigned int smg_id = pFace->smGrp; // smooth group
	unsigned int mat_id = ExporterMAX::GetExporter()->FindMaterialIdx(pIGMat);
	IGameSkin *skin = NULL;

	const int numMod = gM->GetNumModifiers();

	if (numMod > 0)
	{
		for (int i = 0; i < numMod; i++)     // check for skin modifier
		{
			IGameModifier *pM = gM->GetIGameModifier(i);
			if (pM->IsSkin()){
				skin = (IGameSkin*)pM; // skin modifier
			}
		}
	}

	mesh_opt * m_opt = NULL;

	// lets sort by material and find the corresponding mesh_opt
	MatFaceMapIt it_matfacemap = matface_map.find(mat_id);

	if (it_matfacemap == matface_map.end()) // no corresponding mesh, allocate new face holder
	{
		m_opt = new mesh_opt();
		matface_map.insert(MatFaceMapPair(mat_id, m_opt));
	}
	else
	{
		m_opt = (*it_matfacemap).second;
	}

	for (int j = 0; j < 3; ++j) // build the face 
	{
		vert_opt face;
		unsigned int idx;
		unsigned int ori_face_idx = pFace->vert[j]; // get index into the vertex array 
		unsigned int face_idx = ori_face_idx;

		bool create_face = false;

		// build the face as expected
		face.smg_id = smg_id; // smooth group

		if (HasTexVerts)
		{
			idx = pFace->texCoord[j]; // get index into the standard mapping channel

			if (idx != BAD_IDX)
			{
				face.t.x = gM->GetTexVertex(idx).x;
				face.t.y = gM->GetTexVertex(idx).y;
			}
		}

		if (HasCVerts)
		{
			idx = pFace->color[j];   
			face.c.x = gM->GetColorVertex(idx).x;  // get vertex color 
			face.c.y = gM->GetColorVertex(idx).y;
			face.c.z = gM->GetColorVertex(idx).z;
			face.c.w = 1.f;

		}else{
			face.c = Vector4f(1.f, 1.f, 1.f, 1.f);
		}

		Tab<int> mapNums = gM->GetActiveMapChannelNum();
		face.num_tmaps = mapNums.Count();

		if (face.num_tmaps)
		{
			face.tmaps = new Vector[face.num_tmaps];
			for (size_t k = 0; k < face.num_tmaps; ++k)
			{
				unsigned long mapfaceidx[3];
				gM->GetMapFaceIndex(mapNums[k], Index, &mapfaceidx[0]);
				idx = mapfaceidx[j];
				face.tmaps[k].x = gM->GetMapVertex(mapNums[k], idx).x;
				face.tmaps[k].y = gM->GetMapVertex(mapNums[k], idx).y;
				face.tmaps[k].z = gM->GetMapVertex(mapNums[k], idx).z;
			}
		}

		// try to find in origin array
		FaceMapIt it_face_map = m_opt->face_map.find(face_idx);  // get face map iter

		if (it_face_map == m_opt->face_map.end()) // if not find such create anyway
		{
			create_face = true;
		}
		else
		{
			if (is_matching((*it_face_map).second, face) == false) // check find vertex not matching 
			{
				bool found = false;

				// process vertex multi map
				std::pair<FaceMMapIt,FaceMMapIt> pair_mmap = m_opt->face_mmap.equal_range(ori_face_idx);

				FaceMMapIt it_face_mmap = pair_mmap.first;

				while (it_face_mmap != pair_mmap.second && found == false)
				{
					idxvert_opt & idxface = (*it_face_mmap).second;

					if (is_matching(idxface.face, face))
					{
						face_idx = idxface.new_idx;
						found = true;
					}
					++it_face_mmap;
				}

				if (found == false)
				{
					create_face = true;
					++MaxFaceIdx;			// increment max index and
					face_idx = MaxFaceIdx;	// set index is out of bounds of origin 3DMax's index range
				}
			}
		}

		if (create_face)
		{
			if (skin)
			{
				std::vector<w_offset>   w_offsets;
				for (int k = 0; k < skin->GetNumberOfBones(ori_face_idx); ++k)
				{
				/*	if (skin->GetWeight(ori_face_idx, k) > m_eps)
					{
						w_offset w;
						w.weight = skin->GetWeight(ori_face_idx, k);

						_BoneObject * Bone = ExporterMAX::GetExporter()->Skeleton.AddMaxBone(skin->GetIGameBone(ori_face_idx, k), skin->GetIGameBone(ori_face_idx, k)->GetNodeID());
						assert(Bone != NULL);

						w.bone_id = Bone->GetID();
						w_offsets.push_back(w);
					}*/
				}

//				std::sort(w_offsets.begin(), w_offsets.end(), heavier);

				int ILeft = 0;
				for (size_t l = 0; l < w_offsets.size() && l < 4; ++l, ++ILeft)
				{
					w_offset & w = w_offsets[l];
					face.weights[l] = w.weight;
					face.bones[l] = w.bone_id;
				}

				for (; ILeft < 4; ++ILeft)
				{
					face.weights[ILeft] = 0.f;
					face.bones[ILeft] = BAD_IDX;
				}

				// check for valid weights...
				float w_sum = 0.f;

				for (int l = 0; l < 4; ++l){
					w_sum += face.weights[l];
				}

				if ((w_sum < m_one - m_eps) || (w_sum > m_one + m_eps))
				{
					for (int l = 0; l < 4; ++l) // renormalizing... 
						face.weights[l] /= w_sum;
				}
			}

			Point3 v_world = gM->GetVertex(ori_face_idx);
			Point3 v_obj = v_world; // * world_to_obj;

			face.v.x = v_obj.x; // * PRS.k.x * PRS.f;
			face.v.y = v_obj.z; // * PRS.k.z * PRS.f;
			face.v.z = v_obj.y; // * PRS.k.y * PRS.f; 

			// add the vertex and store its new idx
			face.face_idx = m_opt->count;

			m_opt->face_map.insert(FaceMapPair(face_idx, face));

			if (ori_face_idx != face_idx)  // store the newly created and duplicated independently
			{
				idxvert_opt idxface(face_idx, face);// store new face
				m_opt->face_mmap.insert(FaceMMapPair(ori_face_idx, idxface)); // but store key as a original
			}
			m_opt->count++;
		}
		
		// add the face indices...
		TriMapIt it = tri_map.find(mat_id);
		if (it != tri_map.end())
		{
			(*it).second->push_back(face_idx);
		}
		else
		{
			IdxType * idx_type = new IdxType;
			idx_type->push_back(face_idx);
			tri_map.insert(TriMapPair(mat_id, idx_type));
		}
	}
	return true;
}

//----------------------------------------------------------------------------------
bool Helper_ComputeNormals(IGameMesh * gM, MatFaceMapType & matface_map)
{
	//compute normals...
	Vector face_n;
	Vector edge0;
	Vector edge1;

	for (int i = 0; i < gM->GetNumberOfFaces(); ++i)
	{
		FaceEx * pFace = gM->GetFace(i);

		unsigned int smg_id = pFace->smGrp;
		IGameMaterial *pIGMat = gM->GetMaterialFromFace(i);
		unsigned int mat_id = ExporterMAX::GetExporter()->FindMaterialIdx(pIGMat);

		// lets sort by material and find the corresponding mesh_opt
		MatFaceMapIt it_matfacemap = matface_map.find(mat_id);
		assert(it_matfacemap != matface_map.end());

		mesh_opt * m_opt = (*it_matfacemap).second;

		unsigned int face0_idx = pFace->vert[0];
		unsigned int face1_idx = pFace->vert[1];
		unsigned int face2_idx = pFace->vert[2];

		FaceMapIt it_face0_map = m_opt->face_map.find(face0_idx);
		assert(it_face0_map != m_opt->face_map.end());

		FaceMapIt it_face1_map = m_opt->face_map.find(face1_idx);
		assert(it_face1_map != m_opt->face_map.end());

		FaceMapIt it_face2_map = m_opt->face_map.find(face2_idx);
		assert(it_face2_map != m_opt->face_map.end());

		vert_opt & face0 = (*it_face0_map).second;
		vert_opt & face1 = (*it_face1_map).second;
		vert_opt & face2 = (*it_face2_map).second;

		sub(edge0, face2.v, face0.v);
		sub(edge1, face1.v, face0.v);
		cross(face_n, edge0, edge1);

		float weight = length(face_n);

		if (weight > m_eps){
			scale(face_n, m_one / weight);
		}
		else{
			scale(face_n, float(0.001f));
		}

		if (face0.smg_id == smg_id){
			face0.n += face_n;
		}
		if (face1.smg_id == smg_id){
			face1.n += face_n;
		}
		if (face2.smg_id == smg_id){
			face2.n += face_n;
		}

		std::pair<FaceMMapIt,FaceMMapIt> pair_mmap = m_opt->face_mmap.equal_range(face0_idx);
		FaceMMapIt it_face_mmap = pair_mmap.first;
		while (it_face_mmap != pair_mmap.second)
		{
			idxvert_opt & idxface = (*it_face_mmap).second;
			if (idxface.face.smg_id == smg_id)
			{
				FaceMapIt it_face_map = m_opt->face_map.find(idxface.new_idx);
				vert_opt & face = (*it_face_map).second;
				face.n += face_n;
			}
			++it_face_mmap;
		}

		pair_mmap = m_opt->face_mmap.equal_range(face1_idx);
		it_face_mmap = pair_mmap.first;
		while (it_face_mmap != pair_mmap.second)
		{
			idxvert_opt & idxface = (*it_face_mmap).second;
			if (idxface.face.smg_id == smg_id)
			{
				FaceMapIt it_face_map = m_opt->face_map.find(idxface.new_idx);
				vert_opt & face = (*it_face_map).second;
				face.n += face_n;
			}
			++it_face_mmap;
		}

		pair_mmap = m_opt->face_mmap.equal_range(face2_idx);
		it_face_mmap = pair_mmap.first;
		while (it_face_mmap != pair_mmap.second)
		{
			idxvert_opt & idxface = (*it_face_mmap).second;
			if (idxface.face.smg_id == smg_id)
			{
				FaceMapIt it_face_map = m_opt->face_map.find(idxface.new_idx);
				vert_opt & face = (*it_face_map).second;
				face.n += face_n;
			}
			++it_face_mmap;
		}
	}
	return true;
}

//----------------------------------------------------------------------------------
bool Helper_ComputeUV(m_mesh & msh)
{
	// Compute final texture coordinates based on texture uv offsets and scaling factors...
	coord_type coord_list;
	m_material * pMat = ExporterMAX::GetExporter()->GetMaterial(msh.material_id);

	if (pMat && msh.num_texcoord_sets)
	{
		// count the number of textures coords set to compute...
		coord_it it;
		unsigned int num_texchannel = 0;

		for (size_t k = 0; k < pMat->textures.size(); ++k)
		{
			unsigned int tex = pMat->textures[k];
			it = std::find(coord_list.begin(), coord_list.end(), bk_texs[tex]);
			if (it == coord_list.end())
			{
				bk_texs[tex].new_channel = num_texchannel;
				pMat->tex_channel[k] = num_texchannel;
				coord_list.push_back(bk_texs[tex]);
				++num_texchannel;
			}
			else{
				pMat->tex_channel[k] = (*it).new_channel;
			}
		}

		m_texcoord_set * texcoord_sets = new m_texcoord_set[num_texchannel];
		for (size_t k = 0; k < num_texchannel; ++k)
		{
			texcoord_sets[k].dim = 2;
			texcoord_sets[k].texcoords = new float[2 * msh.num_vertices];
		}

		for (size_t k = 0; k < msh.num_vertices; ++k)
		{
			unsigned int idx = k * 2;
			Vector uv = Vector(0.f,0.f,0.f);
			Vector tmp;
			it = coord_list.begin();
			for (size_t l = 0; l < num_texchannel; ++l)
			{
				MatTextureInfo & bk_tex = *it;
				uv.x = msh.texcoord_sets[bk_tex.base_channel].texcoords[idx];
				uv.y = msh.texcoord_sets[bk_tex.base_channel].texcoords[idx + 1];

				mult_pos( tmp, bk_tex.tex_mat, uv );
				texcoord_sets[l].texcoords[idx] = bk_tex.uv_scale.x * tmp.x + bk_tex.uv_offset.x;
				texcoord_sets[l].texcoords[idx + 1] = bk_tex.uv_scale.y * tmp.y + bk_tex.uv_offset.y;
				++it;
			}
		}
		if (num_texchannel)
		{
			delete [] msh.texcoord_sets;
			msh.texcoord_sets = texcoord_sets;
			msh.num_texcoord_sets = num_texchannel;
		}
	}
	return true;
}

//----------------------------------------------------------------------------------
bool Helper_ComputeTBN(m_mesh & msh)
{
	NVMeshMender aMender;

	// compute tangents and binormals
	std::vector<float> vpos;
	std::vector<int>   triIndices;
	std::vector<float> vnor; 
	std::vector<float> texCoords; 

	for (size_t k = 0; k < msh.num_vertices; ++k)
	{
		Vector v(msh.vertices[k].x,msh.vertices[k].y,msh.vertices[k].z);
		Vector n(msh.normals[k].x,msh.normals[k].y,msh.normals[k].z);

		vpos.push_back(v.x);
		vpos.push_back(v.y);
		vpos.push_back(v.z);

		vnor.push_back(n.x);
		vnor.push_back(n.y);
		vnor.push_back(n.z);
	}

	for (size_t k = 0; k < msh.num_faces * 3; ++k)
	{
		triIndices.push_back(msh.faces_idx[k]);
	}

	for (size_t k = 0; k < msh.num_texcoord_sets; ++k)
	{
		texCoords.clear();

		for (size_t l = 0; l < msh.num_vertices; ++l)
		{
			texCoords.push_back(msh.texcoord_sets[k].texcoords[l*2]);
			texCoords.push_back(msh.texcoord_sets[k].texcoords[l*2+1]);
			texCoords.push_back(m_zero);
		}

		std::vector<NVMeshMender::VertexAttribute> inputAtts;// What you have
		std::vector<NVMeshMender::VertexAttribute> outputAtts;// What you want.

		NVMeshMender::VertexAttribute posAtt;
		posAtt.Name_ = "position";
		posAtt.floatVector_ = vpos;

		NVMeshMender::VertexAttribute triIndAtt;
		triIndAtt.Name_ = "indices";
		triIndAtt.intVector_ = triIndices;

		NVMeshMender::VertexAttribute norAtt;
		norAtt.Name_ = "normal";
		norAtt.floatVector_ = vnor;

		NVMeshMender::VertexAttribute texCoordAtt;
		texCoordAtt.Name_ = "tex0";
		texCoordAtt.floatVector_ = texCoords;

		NVMeshMender::VertexAttribute tgtSpaceAtt;
		tgtSpaceAtt.Name_ = "tangent";

		NVMeshMender::VertexAttribute binormalAtt;
		binormalAtt.Name_ = "binormal";

		inputAtts.push_back(posAtt);
		inputAtts.push_back(triIndAtt);
		inputAtts.push_back(norAtt);
		inputAtts.push_back(texCoordAtt);

		outputAtts.push_back(posAtt);
		outputAtts.push_back(triIndAtt);
		outputAtts.push_back(norAtt);
		outputAtts.push_back(texCoordAtt);
		outputAtts.push_back(tgtSpaceAtt);
		outputAtts.push_back(binormalAtt);

		bool bSuccess = aMender.Munge( 
			inputAtts,                                // these are my positions & indices
			outputAtts,                               // these are the outputs I requested, plus extra stuff generated on my behalf
			m_two_pi / (6.0f),						  // tangent space smooth angle
			NULL,                                     // no Texture matrix applied to my tex0 coords
			NVMeshMender::DontFixTangents,            // fix degenerate bases & texture mirroring
			NVMeshMender::DontFixCylindricalTexGen,   // handle cylidrically mapped textures via vertex duplication
			NVMeshMender::DontWeightNormalsByFaceSize // weight vertex normals by the triangle's size
			);

		if (bSuccess)
		{
			msh.texcoord_sets[k].binormals = new Vector[msh.num_vertices];
			msh.texcoord_sets[k].tangents = new Vector[msh.num_vertices];

			for (size_t l = 0; l < msh.num_vertices; ++l)
			{
				msh.texcoord_sets[k].tangents[l].x = outputAtts[4].floatVector_[l * 3];
				msh.texcoord_sets[k].tangents[l].y = outputAtts[4].floatVector_[l * 3 + 1];
				msh.texcoord_sets[k].tangents[l].z = outputAtts[4].floatVector_[l * 3 + 2];

				msh.texcoord_sets[k].binormals[l].x = outputAtts[5].floatVector_[l * 3];
				msh.texcoord_sets[k].binormals[l].y = outputAtts[5].floatVector_[l * 3 + 1];
				msh.texcoord_sets[k].binormals[l].z = outputAtts[5].floatVector_[l * 3 + 2];
			}
		}
	}

	return true;
}

//----------------------------------------------------------------------------------
bool DumpModel(IGameMesh *gM, m_model *pModel, IGameNode *pGameNode)
{
	IGameSkin *skin = NULL;

	if (gM->InitializeData()) // prepare game data
	{
		GMatrix ObjectTM = pGameNode->GetObjectTM(ExporterMAX::GetExporter()->GetStaticFrame());
		
		Matrix3 world_to_obj = Inverse(ObjectTM.ExtractMatrix3());

		AffineParts	PRS;
		decomp_affine(ObjectTM.ExtractMatrix3(), &PRS);

 		//Matrix xform;
		//
 		//xform.set_rot(Quaternion(-PRS.q.x, -PRS.q.z, PRS.q.y, PRS.q.w));
 		//xform.set_translation(Vector(-PRS.t.x, PRS.t.z, PRS.t.y));
		
		const int numMod = gM->GetNumModifiers();

		if (numMod > 0)
		{
			for (int i = 0; i < numMod; i++)     // check for skin modifier
			{
				IGameModifier * pM = gM->GetIGameModifier(i);
				if (pM->IsSkin()){
					skin = (IGameSkin*)pM; // skin modifier
				}
			}
		}
		
		mesh_opt       *m_opt;
		TriMapType      tri_map;
		MatFaceMapType  matface_map; // int <-> material 
	
		unsigned int max_face_idx = 0;
		unsigned int FaceNum = Helper_GetNumberOfFaces(gM, FaceNum, max_face_idx);

		for (size_t i = 0; i < FaceNum; ++i)
		{
			Helper_ProcessFace(gM, i, PRS, world_to_obj, matface_map, tri_map, max_face_idx);
		}

		Helper_ComputeNormals(gM, matface_map);

		for (size_t IndexAdd = 0; IndexAdd < tri_map.size(); ++IndexAdd)
		{
			pModel->meshes.push_back(new m_mesh());
		}

		int count = 0;
		
		TriMapIt it = tri_map.begin();

		while (it != tri_map.end())
		{
			m_mesh &msh = *pModel->meshes[count];

			msh.num_faces = (*it).second->size() / 3;

			msh.material_id = (*it).first;

			msh.faces_idx = new unsigned int[msh.num_faces * 3];

			for (size_t i = 0; i < msh.num_faces * 3; i+=3)
			{
				int Idx0 = (*it).second->front();
				(*it).second->pop_front();

				int Idx1 = (*it).second->front();
				(*it).second->pop_front();

				int Idx2 = (*it).second->front();
				(*it).second->pop_front();

				msh.faces_idx[i+0] = Idx2;
				msh.faces_idx[i+1] = Idx1;
				msh.faces_idx[i+2] = Idx0;
			}		

			MatFaceMapIt it_mapfacemap = matface_map.find((*it).first);
			assert(it_mapfacemap != matface_map.end());
			m_opt = (*it_mapfacemap).second;

			msh.skin = skin ? true : false;
			msh.num_vertices = m_opt->face_map.size();
			msh.vertices = new Vector[msh.num_vertices];
			msh.normals = new Vector[msh.num_vertices];
			msh.colors = new Vector4f[msh.num_vertices];

			msh.weights = skin ? new Vector4f[msh.num_vertices] : NULL;
			msh.bone_idxs = skin ? new unsigned int[msh.num_vertices * 4] : NULL;

			unsigned int texdim = 0;
			bool * faceidx_cache = new bool[msh.num_vertices];
			memset(faceidx_cache, 0, msh.num_vertices * sizeof(bool));
			bool alloc_texture = false;
			
			for (size_t i = 0; i < msh.num_faces * 3; ++i)
			{
				unsigned int face_idx = msh.faces_idx[i];
				FaceMapIt it_face_map = m_opt->face_map.find(face_idx);
				assert(it_face_map != m_opt->face_map.end());

				vert_opt face = (*it_face_map).second;

				msh.faces_idx[i] = face.face_idx;

				if (faceidx_cache[face.face_idx] == false)
				{
					faceidx_cache[face.face_idx] = true;

					msh.vertices[face.face_idx] = Vector(face.v.x, face.v.y, face.v.z);

					msh.colors[face.face_idx].x = face.c.x;
                    msh.colors[face.face_idx].y = face.c.y;
                    msh.colors[face.face_idx].z = face.c.z;

					Vector V(face.n.x, face.n.y, face.n.z);
					V.normalize();
					msh.normals[face.face_idx] = V;

					if (msh.skin)
					{
						msh.weights[face.face_idx].x = face.weights.x;
						msh.weights[face.face_idx].y = face.weights.y;
						msh.weights[face.face_idx].z = face.weights.z;

						msh.bone_idxs[face.face_idx * 4 + 0] = face.bones[0]; // already remapped idxs
						msh.bone_idxs[face.face_idx * 4 + 1] = face.bones[1];
						msh.bone_idxs[face.face_idx * 4 + 2] = face.bones[2];
						msh.bone_idxs[face.face_idx * 4 + 3] = face.bones[3];
					}

					if (face.num_tmaps && alloc_texture == false)
					{
						alloc_texture = true;
						texdim = 2;
						msh.num_texcoord_sets = face.num_tmaps;
						msh.texcoord_sets = new m_texcoord_set[msh.num_texcoord_sets];
						for (size_t j = 0; j < face.num_tmaps; ++j)
						{
							msh.texcoord_sets[j].dim = texdim;
							msh.texcoord_sets[j].texcoords = new float[msh.num_vertices * texdim];
						}
					}

					for (size_t j = 0; j < face.num_tmaps; ++j)
					{
						msh.texcoord_sets[j].texcoords[face.face_idx * texdim] = face.tmaps[j].x;
						msh.texcoord_sets[j].texcoords[face.face_idx * texdim + 1] = face.tmaps[j].y;
					}

					Vector tmp = face.v; //transform_coord(face.v, xform);
					//mult_pos(tmp, xform, face.v);

					// aabb min...
					if (tmp.x < pModel->aabb_min.x)
						pModel->aabb_min.x = tmp.x;
					if (tmp.y < pModel->aabb_min.y)
						pModel->aabb_min.y = tmp.y;
					if (tmp.z < pModel->aabb_min.z)
						pModel->aabb_min.z = tmp.z;
					// aabb max...
					if (tmp.x > pModel->aabb_max.x)
						pModel->aabb_max.x = tmp.x;
					if (tmp.y > pModel->aabb_max.y)
						pModel->aabb_max.y = tmp.y;
					if (tmp.z > pModel->aabb_max.z)
						pModel->aabb_max.z = tmp.z;
					
					// mesh bounding box
					// aabb min...
					if (tmp.x < msh.aabb_min.x)
						msh.aabb_min.x = tmp.x;
					if (tmp.y < msh.aabb_min.y)
						msh.aabb_min.y = tmp.y;
					if (tmp.z < msh.aabb_min.z)
						msh.aabb_min.z = tmp.z;
					// aabb max...
					if (tmp.x > msh.aabb_max.x)
						msh.aabb_max.x = tmp.x;
					if (tmp.y > msh.aabb_max.y)
						msh.aabb_max.y = tmp.y;
					if (tmp.z > msh.aabb_max.z)
						msh.aabb_max.z = tmp.z;
				}
			}
			delete [] faceidx_cache;
			++count;
			++it;

			Helper_ComputeUV(msh);
			Helper_ComputeTBN(msh);
		}
		return true;
	}
	return false; // "BadObject";
}

//----------------------------------------------------------------------------------
bool DumpModelCollision(IGameMesh * gM, CollisionShapeMesh * pCollisionObject, GMatrix nodeTM)
{
	bool bResult = false;

	if (gM->InitializeData()) // prepare game data
	{
/*		AffineParts	PRS;
		Matrix3f world_to_obj = Inverse(nodeTM.ExtractMatrix3());
		decomp_affine(nodeTM.ExtractMatrix3(), &PRS);

// 		Matrix xform;
// 		xform.set_rot(Quaternion(PRS.q.x, PRS.q.z, PRS.q.y, PRS.q.w));
// 		xform.set_translation(Vector(-PRS.t.x, PRS.t.z, PRS.t.y));

		unsigned int max_face_idx = 0;
		unsigned int FaceNum = Helper_GetNumberOfFaces(gM, FaceNum, max_face_idx);

		int Count = 0;

		for (int Index = 0; Index < FaceNum; ++Index)
		{
			FaceEx * pFace = gM->GetFace(Index);

			for (int j = 2; j >= 0; --j) // build the face 
			{
				vert_opt face;
				unsigned int ori_face_idx = pFace->vert[j]; // get index into the vertex array 
			
				Point3 v_world = gM->GetVertex(ori_face_idx);
				Point3 v_obj = v_world;

				Vector V(v_obj.x, v_obj.z, v_obj.y);

				bool bFind = false;

				for (int ActualIndex = 0; ActualIndex < pCollisionObject->Idndexes.size(); ++ActualIndex)
				{
					if (pCollisionObject->Points[pCollisionObject->Idndexes[ActualIndex]] == V)
					{
						pCollisionObject->Idndexes.push_back(pCollisionObject->Idndexes[ActualIndex]);
						bFind = true;
						break;
					}
				}

				if (!bFind)
				{
					pCollisionObject->Points.push_back(V);
					pCollisionObject->Idndexes.push_back(Count);
					Count++;
				}
			}
		}
		*/
	}
	return bResult;
}