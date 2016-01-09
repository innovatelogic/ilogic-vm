#ifndef __scenemanager_h__
#define __scenemanager_h__

#ifdef WIN32
#pragma once
#endif

#include "game_types.h"
#include "StdafxFoundation.h"

class CORESDK_API CSceneManager
{
 	typedef std::vector<class CScene*>		TVecScenes;
 	typedef TVecScenes::iterator			TVecSceneIter;
 	typedef TVecScenes::const_iterator		TVecSceneConstIter;

public:
	CSceneManager(class CCoreSDK *pSDK);
	virtual ~CSceneManager();

	void Command_LoadScene(const char *strFileName);
	void Command_InitializeScene(const char *strFilename);
	void Command_CloseScene(const class CScene *pScene);
	void Command_CloseAll();

	bool IsLevelLoad(const char *strName);
	
	/**
	 * Load objects-tree from file. Use SuperDeserializer method.
	 *
	 * @param [in] filename.
	 */
	bool	LoadFromFile(const std::string &filename);

	bool	LoadFromXML(tinyxml2::XMLElement *pTree);

	void	Initialize();

	void	CloseAll();

protected:
private:
	class CCoreSDK	*m_pCoreSDK;
	TVecScenes		m_VecScenes;
};

#endif//__scenemanager_h__