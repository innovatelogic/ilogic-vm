//xcopy "$(TargetDir)*.dle" "c:/Program Files/Autodesk/3dsMax8/plugins" /Y

#ifndef __EXPORTERMAX__H
#define __EXPORTERMAX__H

#ifdef _WIN32
#pragma once
#endif

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include "Max.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "IGame\IGame.h"
#include "IGame\IGameObject.h"
#include "IGame\IGameProperty.h"
#include "IGame\IGameControl.h"
#include "IGame\IGameModifier.h"
#include "IGame\IConversionManager.h"
#include "IGame\IGameError.h"
#include "decomp.h"
#include "utilapi.h"
#include "IDxMaterial.h"

#include "m_interface.h"
#include "m_material.h"
#include "m_texture.h"
#include "m_node.h"
#include "m_scene.h"
#include "m_model.h"
#include "m_streams.h"
#include "m_file.h"
#include "m_factory.h"
#include "geometry.h"
#include "decomp.h"

#include "ExportTypes.h"

#define EXPORTERMAX_CLASS_ID	Class_ID(0x3d3d2d84, 0x4e6e6223)

typedef std::map<class IGameMaterial*, struct m_material*>	GMatMap;
typedef GMatMap::value_type						GMatMapPair;
typedef GMatMap::iterator						GMatMapIt;

typedef std::map<struct m_material*, int>		MatIdxMap;
typedef MatIdxMap::value_type					MatIdxMapPair;
typedef MatIdxMap::iterator						MatIdxMapIt;
typedef MatIdxMap::const_iterator				MatIdxMapCIt;

typedef std::map<unsigned int, MatTextureInfo>	IdxBKTexMap;
typedef IdxBKTexMap::value_type					IdxBKTexMapPair;
typedef IdxBKTexMap::iterator					IdxBKTexMapIt;

extern TCHAR* GetString(int id);
extern HINSTANCE hInstance;
extern IdxBKTexMap bk_texs;

struct SVecNodes
{
	std::vector<struct m_node*>	VNodes;
};

struct SMeshBank
{
	std::vector<SVecNodes> VBankNodes;
};

//--------------------------------------------------------------------------------------
// decl.
void ExportMaterials();
void DumpMaxNode(class IGameNode *pGameNode);
void DumpNodeCollision(class IGameNode * pGameNode);
bool DumpModel(class IGameMesh *gM, m_model *pModel, IGameNode * pGameNode);
bool DumpModelCollision(class IGameMesh * gM, class CollisionShapeMesh * pCollisionObject, GMatrix nodeTM);
bool CompleteExport(const char * filename);
bool CompleteExportSkeletonTree(const char * filename);
bool CompleteExportAnim(const char * filename);
bool CompleteExportMat(const char * filename);
bool CompleteExportObjects(const char * filename);
bool CompleteExportCollision(const char * filename);

//--------------------------------------------------------------------------------------
class ExporterMAX : public SceneExport 
{
public:
	ExporterMAX();
	virtual ~ExporterMAX();

	int					ExtCount();					// Number of extensions supported
	const TCHAR*		Ext(int n);					// Extension #n (i.e. "3DS")
	const TCHAR*		LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR*		ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	const TCHAR*		AuthorName();				// ASCII Author name
	const TCHAR*	    CopyrightMessage();			// ASCII Copyright message
	const TCHAR*	    OtherMessage1();			// Other message #1
	const TCHAR*	    OtherMessage2();			// Other message #2
	unsigned int	    Version();					// Version number
	void			    ShowAbout(HWND hWnd);		// Show DLL's "About..." box

	BOOL				SupportsOptions(int ext, DWORD options);

	int					DoExport(const TCHAR *name, 
								 ExpInterface *pEi,
								 Interface *pI,
								 BOOL suppressPrompts = FALSE,
								 DWORD options = 0);

	BOOL				ExportAnim();
   
	const char*			GetFilename() const { return m_strFilename.c_str(); }
	void				SetFilename(const char * filename)  { m_strFilename = filename; }

	bool				GetExportSelected() const { return m_bExportSelected; };
	void				SetExportSelected(bool flag) { m_bExportSelected = flag; }
	
	bool				GetForceDDS() const { return m_bExportDDS; };
	void				SetForceDDS(bool flag) { m_bExportDDS = flag; }

	class IGameScene*	GetIGame()  { assert(m_pIgame); return m_pIgame; }
	static ExporterMAX* GetExporter() { assert(m_pExporter); return m_pExporter; }
	
	int					GetStaticFrame() const { return m_nStaticFrame; }

	m_scene&			GetSceneRef() const { assert(m_pScene); return *m_pScene; }
	std::vector<m_node*>& GetNodesRef() { return m_VNodes; }
	
	unsigned int		AddMaterial(IGameMaterial *pIGMat, m_material *pMat); 
	unsigned int		FindMaterialIdx(IGameMaterial *pIGMat);
	m_material*			GetMaterial(unsigned int idx);

    unsigned int		AddTexture(m_texture *pTex);
	unsigned int		FindTexture(m_texture *pTex);
	
	unsigned int		AddNode(m_node *pNode, size_t lvlLOD = 0, size_t lvlDestr = 0);
	unsigned int		FindNode(const char *name, size_t lvlLOD = 0, size_t lvlDestr = 0);
	
	unsigned int		GetNumMaterials();
	const MatIdxMap&	GetMaterials() const;
	std::vector<m_texture*>& GetTexturesRef() { return m_VTextures; }

private:
public:
	static ExporterMAX				*m_pExporter;	// static pointer to self
	class  IGameScene				*m_pIgame;		// max interface pointer
	m_scene							*m_pScene;		// export scene
	
	std::string						m_strFilename;   

	std::vector<struct m_texture*>  m_VTextures;

	std::vector<struct m_node*>		m_VNodes;
	std::vector<SVecNodes>			m_VNodesLOD;
	std::vector<struct m_node*>		m_VDestrNodes;
	std::vector<SVecNodes>			m_VDestrNodesLOD;

	std::vector<SMeshBank>			m_VMeshBanks;	// Dstr0[ Lod0[Node0, ...], Lod1[Node0, ...], ...],
													// Dstr1[ Lod0[Node0, ...], Lod1[Node0, ...], ...],
													// Dstr2[ Lod0[Node0, ...], Lod1[Node0, ...], ...], 

	// export options
	bool                        m_bExportSelected;  // Export only selected objects in the scene
	bool                        m_bExportDDS;		// forces bitmap filenames to reference .dds files

	// animation options
	int                         m_nStaticFrame;
	int                         m_nSamplesPerFrame;

	// material managament
	GMatMap                     m_MapMaterials;			// material map <IGameMaterial*, struct m_material>
	MatIdxMap                   m_MaterialsTOC;		// material Table of Contents std::map<m_material*, int>
	unsigned int                m_MatIdxCntr;

	//SkeletonObjectMax			Skeleton;			// contain duped skeleton
	//AnimTrack					Track;

	std::vector<std::string>				CollisionSoupData;
	std::vector<class CollisionShapeBase*>	m_VCollisionObjects;
};

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
class ExporterMAXClassDesc : public ClassDesc2 
{
public:
	int 			    IsPublic()                          { return TRUE; }
	void*			    Create(BOOL loading = FALSE)        { return new ExporterMAX(); }
	const TCHAR*		ClassName()							{ return _T("ExporterMAX"); } //GetString(IDS_CLASS_NAME);
	SClass_ID			SuperClassID()						{ return SCENE_EXPORT_CLASS_ID; } //UTILITY_CLASS_ID
	Class_ID			ClassID()							{ return EXPORTERMAX_CLASS_ID; }
	const TCHAR*		Category()                          { return _T("OpenES"); }
	const TCHAR*		InternalName()                      { return _T("OpenES ExporterMAX"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE			HInstance()                         { return hInstance; }			// returns owning module handle
};

#endif//__EXPORTERMAX__H