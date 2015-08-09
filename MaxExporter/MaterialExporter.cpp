#include "expstdafx.h"

#define DXMATERIAL_DYNAMIC_UI Class_ID(0xef12512, 0x11351ed1)
#define MATRIX_SIZE4x4 16

void DumpMaterial(IGameMaterial * mat);
void DumpTexture(m_material * pMat,IGameMaterial * mat);

IdxBKTexMap bk_texs;
typedef float scalar;

//-----------------------------------------------------------------------
typedef enum eNVTYPEID
{
	// Type ID's are FIXED.
	// Do not rearrange these, just add new to the end.
	NVTYPEID_VEC4 = 0,
	NVTYPEID_VEC3 = 1,
	NVTYPEID_VEC2 = 2,
	NVTYPEID_FLOAT = 3,
	NVTYPEID_BOOL = 4,
	NVTYPEID_MATRIX = 5,
	NVTYPEID_TEXTURE = 6,
	NVTYPEID_DWORD = 7,
	NVTYPEID_INT = 8,
	NVTYPEID_STRING = 9,
	NVTYPEID_VERTEXSHADER = 10,
	NVTYPEID_PIXELSHADER = 11,
	NVTYPEID_SAMPLER = 12,
	NVTYPEID_SAMPLER2D = 13,
	NVTYPEID_SAMPLER3D = 14,
	NVTYPEID_SAMPLERCUBE = 15,
	NVTYPEID_PIXELFRAGMENT = 16,
	NVTYPEID_VERTEXFRAGMENT = 17,
	NVTYPEID_VOID = 18,
	NVTYPEID_TEXTURE2D = 19,
	NVTYPEID_TEXTURE3D = 20,
	NVTYPEID_TEXTURECUBE = 21,
	NVTYPEID_INTERFACE = 22,

	// New, need to keep enum values fixed for max plugin
	NVTYPEID_SAMPLER1D = 23,
	NVTYPEID_TEXTURE1D = 24,

	NVTYPEID_UNKNOWN = 0x7FFFFFFF
} eNVTYPEID;

const m_texture::tex_type texture_type[NTEXMAPS] = 
{ 
	m_texture::AMBIENT,            // ID_AM 0   ambient
	m_texture::DIFFUSE,            // ID_DI 1   diffuse
	m_texture::SPECULAR,           // ID_SP 2   specular
	m_texture::SPECULAR_POWER,     // ID_SH 3   shininesNs
	m_texture::GLOSS,              // ID_SS 4   shininess strength
	m_texture::SELF_ILLUMATION,    // ID_SI 5   self-illumination
	m_texture::OPACITY,            // ID_OP 6   opacity
	m_texture::CUSTOM,             // ID_FI 7   filter color
	m_texture::BUMP,               // ID_BU 8   bump           
	m_texture::REFLECTION,         // ID_RL 9   reflection
	m_texture::REFRACTION,         // ID_RR 10  refraction 
	m_texture::CUSTOM,             // ID_DP 11  displacement 
};

struct tex_channel
{
	m_texture     *pTex;
	unsigned int   channel;

	tex_channel() : pTex(NULL), channel(INDEX_NONE) {}
};

//-----------------------------------------------------------------------
bool IsDynamicDxMaterial(MtlBase * newMtl)
{
	DllDir * lookup = GetCOREInterface()->GetDllDirectory();
	ClassDirectory & dirLookup = lookup->ClassDir();

	ClassDesc * cd = dirLookup.FindClass(MATERIAL_CLASS_ID, newMtl->ClassID());
	
	if(cd && cd->SubClassID() == DXMATERIAL_DYNAMIC_UI){
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------
void ExportMaterials()
{
	// get total num of materials
	const int matCount = ExporterMAX::GetExporter()->GetIGame()->GetRootMaterialCount();

	for (int index = 0; index < matCount; ++index)
	{
		IGameMaterial *pMat = ExporterMAX::GetExporter()->GetIGame()->GetRootMaterial(index);

		if (pMat){
			DumpMaterial(pMat);
		}
	}
}

//----------------------------------------------------------------------------------
void DumpMaterial(IGameMaterial *pGMaxMat)
{
    m_material *pMat = NULL;

	if (!pGMaxMat->IsMultiType()) // check not a mix material
	{
        pMat = new m_material;

		// set the name of the material...
	    pMat->name = pGMaxMat->GetMaterialName();

		// add the new material to the TOC(Table Of Contants) and set its id...
		pMat->id = ExporterMAX::GetExporter()->AddMaterial(pGMaxMat, pMat);

		Mtl *pMaxMaterial = pGMaxMat->GetMaxMaterial();
				
		if (pMaxMaterial->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
		{
			StdMat *pStandardMaterial = (StdMat*)pMaxMaterial;
			
			Color col;

			// diffuse color...
			col = pStandardMaterial->GetDiffuse(ExporterMAX::GetExporter()->GetStaticFrame());
			pMat->diffuse = Vector4f(col.r, col.g, col.b, 1.f);

			// ambient color...
			col = pStandardMaterial->GetAmbient(ExporterMAX::GetExporter()->GetStaticFrame());
			pMat->ambient = Vector4f(col.r, col.g, col.b, 1.f);

			// specular color...
			col = pStandardMaterial->GetSpecular(ExporterMAX::GetExporter()->GetStaticFrame());
			pMat->specular = Vector4f(col.r, col.g, col.b, 1.f);

			// emissive color...
			pMat->emission = Vector4f(0.f, 0.f, 0.f, 1.f);

			// level of transparency
			pMat->transparent = pStandardMaterial->GetOpacity(ExporterMAX::GetExporter()->GetStaticFrame());

			// specular exponent...
			pMat->shininess = pStandardMaterial->GetShininess(ExporterMAX::GetExporter()->GetStaticFrame());
	
			// transparency
			if (pMat->transparent < 1.f){
				pMat->diffuse.z = pMat->transparent;
			}
		
		}// 3dsmax6 HLSL Material support. (IDxMaterial)
		else if (IsDynamicDxMaterial((MtlBase*)pMaxMaterial))
		{
			IDxMaterial *idxm = (IDxMaterial*)pMaxMaterial->GetInterface(IDXMATERIAL_INTERFACE);
			
			int lightParams = idxm->GetNumberOfLightParams();

			for (int i = 0; i < lightParams; ++i)
			{
                INode *pNode = idxm->GetLightNode(i);

				if (pNode){
                    TCHAR *paramName = idxm->GetLightParameterName(i);
				}
			}

			// Other attributes are located in paramblk 0...
			IParamBlock2 *pParamBlk2 = (IParamBlock2*)(pMaxMaterial->GetParamBlock(0));
			TimeValue t0 = ExporterMAX::GetExporter()->GetIGame()->GetSceneStartTime();
			TimeValue t1 = ExporterMAX::GetExporter()->GetIGame()->GetSceneEndTime();
			TimeValue DeltaTime = ExporterMAX::GetExporter()->GetIGame()->GetSceneTicks();

			const int SamplingRate = 1;

			int numkeys = (t1 - t0) / (DeltaTime * SamplingRate) + 1; 

			for (int i = 0; i < pParamBlk2->NumParams(); ++i)
			{
				ParamID id = pParamBlk2->IndextoID(i);
				ParamDef paramDef = pParamBlk2->GetParamDef(id);

				// we want the variable name not the UI Name...
				OutputDebugString(paramDef.int_name);
			}
		}
		//do the textures if they are there
		DumpTexture(pMat, pGMaxMat);
	}
}

//----------------------------------------------------------------------------------
// dump material textures
void DumpTexture(m_material *pMat, IGameMaterial *pGMaxMat)
{
	std::vector<tex_channel>		bk_tex_channel;
	std::vector<unsigned int>		bk_tex_idx;
    std::vector<MatTextureInfo>		TexInfos;

	int texCount = pGMaxMat->GetNumberOfTextureMaps();

	for (int i = 0; i < texCount; ++i)
	{
		IGameTextureMap * pGMaxTex = pGMaxMat->GetIGameTextureMap(i);

		int tex_type = pGMaxTex->GetStdMapSlot();

		if (pGMaxTex->IsEntitySupported() && tex_type >= 0)	//its a bitmap texture
		{
			MatTextureInfo TexInfo;
			tex_channel tc;

			TexInfo.mat_id = pMat->id;

			m_texture * pTex = new m_texture;

			std::string pathname = pGMaxTex->GetBitmapFileName();

			int idx = (int)pathname.rfind('\\');
			if (idx == INDEX_NONE){
				idx = (int)pathname.rfind('/');
			}
			
			std::string filename = pathname.substr(idx + 1, INDEX_NONE);

			pTex->name = filename;

			// set the texture xform...
			IGameUVGen *pUVGen = pGMaxTex->GetIGameUVGen();
			GMatrix UVMat = pUVGen->GetUVTransform();
            TexInfo.tex_mat = pTex->tex_mat = (scalar*)UVMat.GetAddr(); // save mapping matrix

          	// get the uv channel to use...
			Texmap *pTMap = pGMaxTex->GetMaxTexmap();
			BitmapTex *pBTex = (BitmapTex*)pTMap;
			StdUVGen *pStdUVGen = pBTex->GetUVGen();

			if (pStdUVGen){
				tc.channel = pStdUVGen->GetMapChannel() - 1;
			}

			IParamBlock2 *pUVWCropParam = (IParamBlock2*)(pBTex->GetReference(1));

			if (pUVWCropParam)
			{
				 pUVWCropParam->GetValue(0, ExporterMAX::GetExporter()->GetStaticFrame(), TexInfo.uv_offset.x, FOREVER);
				 pUVWCropParam->GetValue(1, ExporterMAX::GetExporter()->GetStaticFrame(), TexInfo.uv_offset.y, FOREVER);
				 pUVWCropParam->GetValue(2, ExporterMAX::GetExporter()->GetStaticFrame(), TexInfo.uv_scale.x,  FOREVER);
				 pUVWCropParam->GetValue(3, ExporterMAX::GetExporter()->GetStaticFrame(), TexInfo.uv_scale.y,  FOREVER);
			}

			// set the type of texture...
			pTex->type = texture_type[tex_type];

 			// if we have a bump map, we create a normal map with the convention
 			// that the filename will be the same name as the bump map + "_normal" 
 			// appended to it.
  			if (pTex->type == m_texture::BUMP)
 			{
 				std::string normal_map = pTex->name;
 				std::string::size_type pos = normal_map.rfind(".");
 				normal_map.insert(pos, "_normal");
 
 				m_texture  *pTexNormal = new m_texture;
 				*pTexNormal = *pTex;
 
 				pTexNormal->name = normal_map;
 				pTexNormal->type = m_texture::NORMAL;
 
 				tc.pTex = pTexNormal;
 
 				bk_tex_channel.push_back(tc);	// add the new texture to the local TOC
 				TexInfos.push_back(TexInfo);
 			}

			 tc.pTex = pTex;
						 
			 bk_tex_channel.push_back(tc);	// add the new texture to the local TOC
			 TexInfos.push_back(TexInfo);
		}
	}

	// lets check if we don't have them already in our global TOC...
	for (size_t index = 0; index < bk_tex_channel.size(); ++index)
	{
		m_texture * pTex = bk_tex_channel[index].pTex;

		unsigned int idx = ExporterMAX::GetExporter()->FindTexture(pTex);
		
		if (idx == INDEX_NONE){	
			idx = ExporterMAX::GetExporter()->AddTexture(pTex); // add the new texture to the TOC
		}

		bk_tex_idx.push_back(idx);

		pMat->tex_channel.push_back(bk_tex_channel[index].channel);

		TexInfos[index].base_channel = bk_tex_channel[index].channel;

		bk_texs.insert(IdxBKTexMapPair(idx, TexInfos[index]));
	}

	// set the texture indices...
	for (size_t index = 0; index < bk_tex_idx.size(); ++index){
		pMat->textures.push_back(bk_tex_idx[index]);
	}
}