#include "expstdafx.h"

ExporterMAX * ExporterMAX::m_pExporter = NULL;

static ExporterMAXClassDesc ExporterMAXDesc;

ClassDesc2* GetExporterMAXDesc() { return &ExporterMAXDesc; }

extern bool	bExportSkeleton;
extern bool	bExportModel;
extern bool	bExportAnim;
extern bool bExportCollision;

//----------------------------------------------------------------------------------
ExporterMAX::ExporterMAX()
: m_MatIdxCntr(BAD_IDX)
, m_nStaticFrame(0)
, m_nSamplesPerFrame(1)
, m_bExportSelected(false)
, m_bExportDDS(true)
{
	// singleton
	assert(m_pExporter == NULL);
	m_pExporter = this;
}

//----------------------------------------------------------------------------------
ExporterMAX::~ExporterMAX()
{
    m_pExporter = NULL;
}

//----------------------------------------------------------------------------------
int ExporterMAX::ExtCount()
{
	return 1;
}

//----------------------------------------------------------------------------------
const TCHAR *ExporterMAX::Ext(int n)
{		
	return _T("lac");
}

//----------------------------------------------------------------------------------
const TCHAR *ExporterMAX::LongDesc()
{
	return _T("OpenES Exporter v.0.01");
}

//----------------------------------------------------------------------------------
const TCHAR *ExporterMAX::ShortDesc() 
{			
	return _T("OpenES v.0.01");
}

//----------------------------------------------------------------------------------
const TCHAR *ExporterMAX::AuthorName()
{			
	return _T("Yura Gunko (Yur.G)   email: yura.gunko@gmail.com");
}

//----------------------------------------------------------------------------------
const TCHAR *ExporterMAX::CopyrightMessage() 
{	
	return _T("Innovatelogic. (c) 2013");
}

//----------------------------------------------------------------------------------
const TCHAR *ExporterMAX::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

//----------------------------------------------------------------------------------
const TCHAR *ExporterMAX::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

//----------------------------------------------------------------------------------
unsigned int ExporterMAX::Version()
{				
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return 101;
}

//----------------------------------------------------------------------------------
void ExporterMAX::ShowAbout(HWND hWnd)
{			
	// Optional
}

//----------------------------------------------------------------------------------
BOOL ExporterMAX::SupportsOptions(int ext, DWORD options)
{
    return TRUE;
}

//----------------------------------------------------------------------------------
DWORD WINAPI fn(LPVOID arg)
{
	return(0); // Dummy function for progress bar
}

//----------------------------------------------------------------------------------
int ExporterMAX::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options /*= 0*/)
{
	Interface *pIp = GetCOREInterface();
   
	SetFilename(name);

	pIp->ProgressStart(_T("Exporting To OpenES"), TRUE, fn, NULL);
	pIp->ProgressUpdate(0);

	m_pIgame = GetIGameInterface();
	m_pIgame->InitialiseIGame(GetExportSelected()); 

	m_pScene = m_factory::get_factory()->new_scene();
	
	ExportMaterials();
	
	for (int Index = 0; Index < m_pIgame->GetTopLevelNodeCount(); ++Index)
	{
		IGameNode *pGameNode = m_pIgame->GetTopLevelNode(Index);
	
		if (pGameNode->IsTarget()){ // exclude lights and cameras
			continue;
		}
		DumpNodeCollision(pGameNode);
		DumpMaxNode(pGameNode);
	}
	
	ExportAnim();

	CompleteExport(name);

	/*if (ExporterMAX::GetExporter()->Track.GetNumFrames() > 0)
	{
		GMatMapIt it = m_MapMaterials.begin();
		while (it != m_MapMaterials.end())
		{
			it->second->fx_file = "dx9_hlsl_paletteskin.fx";
			it->second->tehnique = "PaletteSkinTechnique";
			++it;
		}
	}*/

	// complete export
	
	CompleteExportObjects(name);
	
	/*if (bExportSkeleton){
		CompleteExportSkeletonTree(name);
	}
	if (bExportAnim){
		CompleteExportAnim(name);
	}*/
	
	CompleteExportMat(name);

	m_pScene->release();
    m_pScene = NULL;

	// clear physics data
	for (size_t Index = 0; Index < ExporterMAX::GetExporter()->m_VCollisionObjects.size(); ++Index)
	{
		delete ExporterMAX::GetExporter()->m_VCollisionObjects[Index];
	}

	ExporterMAX::GetExporter()->m_VCollisionObjects.clear();

    m_pIgame->ReleaseIGame();
    pIp->ProgressEnd();	

	return TRUE;
}

//----------------------------------------------------------------------------------
BOOL ExporterMAX::ExportAnim()
{
	// export single skeleton
	/*Tab<IGameNode*> meshes = pIgame->GetIGameNodeByType(IGameObject::IGAME_MESH);
	if (meshes.Count() == 0){
		return FALSE;
	}

	IGameNode * node = meshes[0];
	
	if (node != NULL)
	{
		IGameObject * object = node->GetIGameObject();
		
		if (object != NULL)
		{
			object->InitializeData();
			Track.Build(object, &Skeleton);
		}
	}
	*/
	return TRUE;
}

//----------------------------------------------------------------------------------
unsigned int ExporterMAX::AddMaterial(IGameMaterial *pIGMat, m_material *pMat)
{
  GMatMapIt it = m_MapMaterials.find(pIGMat);

  if (it == m_MapMaterials.end())
   {
	   // add to the map
	   m_MapMaterials.insert(GMatMapPair(pIGMat, pMat));
	   m_MaterialsTOC.insert(MatIdxMapPair(pMat, ++m_MatIdxCntr));
  }
  else
  {
	  // already exist
	  MatIdxMapIt idx_it = m_MaterialsTOC.find((*it).second);
	  assert(idx_it != m_MaterialsTOC.end());
	  return (*idx_it).second;
  }
  return m_MatIdxCntr;
}

//----------------------------------------------------------------------------------
unsigned int ExporterMAX::AddTexture(m_texture* pTex)
{
	unsigned int ret_idx(m_VTextures.size());
	m_VTextures.push_back(pTex);
	return ret_idx;
}

//----------------------------------------------------------------------------------
unsigned int ExporterMAX::FindTexture(m_texture * pTex)
{
	unsigned int idx = 0;
	std::vector<m_texture*>::iterator it = m_VTextures.begin();
	while (it != m_VTextures.end())
	{
		if (memcmp(*it, pTex, sizeof(m_texture)) == 0)
			return idx;
		++it;
		++idx;
	}
	return BAD_IDX;
}

//----------------------------------------------------------------------------------
unsigned int ExporterMAX::AddNode(m_node *pNode, size_t lvlLOD /*= 0*/, size_t lvlDestr /*= 0*/)
{
	assert(lvlLOD >= 0 && lvlLOD < 5);
	assert(lvlDestr >= 0 && lvlDestr < 5);

	// ensure destruction depth enough
	while ((int)m_VMeshBanks.size() <= lvlDestr)
	{
		m_VMeshBanks.push_back(SMeshBank());
		assert(m_VMeshBanks.size() < 5);
	}

	SMeshBank *pMeshBank = &m_VMeshBanks[lvlDestr];

	// ensure LOD depth
	while ((int)pMeshBank->VBankNodes.size() <= lvlLOD)
	{
		pMeshBank->VBankNodes.push_back(SVecNodes());
		assert(pMeshBank->VBankNodes.size() < 5);
	}
	
	std::vector<m_node*> *VRef = &pMeshBank->VBankNodes[lvlLOD].VNodes; 
		
	for (unsigned int i = 0; i < VRef->size(); ++i)
	{
		if (!strcmp(pNode->name.c_str(), (*VRef)[i]->name.c_str()))
		{
			return i;
		}
	}

	unsigned int ret_idx((*VRef).size());
	VRef->push_back(pNode);
	return ret_idx;
}

//----------------------------------------------------------------------------------
unsigned int ExporterMAX::FindNode(const char *name, size_t lvlLOD /*= 0*/, size_t lvlDestr /*= 0*/)
{
	assert(lvlLOD >= 0 && lvlLOD < 5);
	assert(lvlDestr >= 0 && lvlDestr < 5);

	// ensure destruction depth enough
	if (lvlDestr >= (int)m_VMeshBanks.size()){
		return BAD_IDX;
	}

	SMeshBank* pMeshBank = &m_VMeshBanks[lvlDestr];

	// ensure LOD depth
	if (lvlLOD >= (int)pMeshBank->VBankNodes.size()){
		return BAD_IDX;
	}
	
	std::vector<m_node*> &VRef = pMeshBank->VBankNodes[lvlLOD].VNodes; 
	for (unsigned int i = 0; i < VRef.size(); ++i)
	{
		if (!strcmp(name, VRef[i]->name.c_str())){
			return i;
		}
	}
	return BAD_IDX;
}

//----------------------------------------------------------------------------------
unsigned int ExporterMAX::FindMaterialIdx(IGameMaterial * pIGMat)
{
	unsigned int idx = BAD_IDX;
	GMatMapIt it = m_MapMaterials.find(pIGMat);
	if (it != m_MapMaterials.end())
	{
		MatIdxMapIt idx_it = m_MaterialsTOC.find((*it).second);
		return (*idx_it).second;
	}
	return idx;
}

//----------------------------------------------------------------------------------
m_material* ExporterMAX::GetMaterial(unsigned int idx)
{
	MatIdxMapIt it = m_MaterialsTOC.begin();
	while (it != m_MaterialsTOC.end())
	{
		if ((*it).second == idx)
			return (*it).first;
		++it;
	}
	return NULL;
}

//----------------------------------------------------------------------------------
unsigned int ExporterMAX::GetNumMaterials()
{
	return (unsigned int)m_MaterialsTOC.size();
}

//----------------------------------------------------------------------------------
const MatIdxMap& ExporterMAX::GetMaterials() const
{
	return m_MaterialsTOC;
}

