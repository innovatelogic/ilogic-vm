#ifndef __uiscenemanager_h__
#define __uiscenemanager_h__

#ifdef WIN32
#pragma once
#endif

#include "game_types.h"
#include "StdafxFoundation.h"

class CORESDK_API CUISceneManager
{
	typedef std::vector<class UIScene*>			TVecUIScenes;
	typedef TVecUIScenes::iterator				TVecUISceneIter;
	typedef TVecUIScenes::const_iterator		TVecUISceneConstIter;

public:
	CUISceneManager(class CCoreSDK *pSDK);
	virtual ~CUISceneManager();

	void Command_LoadScene(const char *strFileName);
	void Command_InitializeScene(const char *strFileName);
	void Command_CloseScene(class UIScene * pScene);
	void Command_CloseAll();

	bool RemoveScene(const class UIScene * pScene);
	
	void Initialize();

	void CloseAll();

	/**
	 * Load objects-tree from file. Use SuperDeserializer method.
	 *
	 * @param [in] filename.
	 */
	bool LoadFromFile(const std::string &filename);

	bool LoadFromXML(TiXmlElement *pTree);

	/**
	 * Update all scenes.
	 */
	void Update(float DeltaTime);

protected:
private:
	CCoreSDK				*m_pCoreSDK;
	TVecUIScenes			m_VecUIScenes;
};

#endif//__uiscenemanager_h__