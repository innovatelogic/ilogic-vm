#include "coresdkafx.h"
#include "ObjectFactory.h"

//----------------------------------------------------------------------------------------------
CSceneManager::CSceneManager(CCoreSDK *pSDK)
: m_pCoreSDK(pSDK)
{

}

//----------------------------------------------------------------------------------------------
CSceneManager::~CSceneManager()
{

}

//----------------------------------------------------------------------------------------------
void CSceneManager::Command_LoadScene(const char *strFileName)
{
/*	std::vector<class CommandBase*> ArrayCmd;

	ArrayCmd.push_back(new CommandBOOL_ValRef<CSceneManager, CSceneManager, std::string>(&CSceneManager::LoadFromFile, this, strFileName));

	m_pCoreSDK->AddCommandsArray(ArrayCmd);*/

    LoadFromFile(strFileName);
}

//----------------------------------------------------------------------------------------------
void CSceneManager::Command_InitializeScene(const char *strFileName)
{
	/*std::vector<class CommandBase*> ArrayCmd;

	ArrayCmd.push_back(new Command<CSceneManager, CSceneManager>(&CSceneManager::Initialize, this));

	m_pCoreSDK->AddCommandsArray(ArrayCmd);*/

    Initialize();
}

//----------------------------------------------------------------------------------------------
void CSceneManager::Command_CloseScene(const CScene *pScene)
{
	TVecSceneIter IterFind = std::find(m_VecScenes.begin(), m_VecScenes.end(), pScene);
	if (IterFind != m_VecScenes.end())
	{
		//m_pCoreSDK->AddCommand(new Command<CScene, CScene>(&CScene::Release, *IterFind));

        (*IterFind)->Release();
		m_VecScenes.erase(IterFind);
	}
}

//----------------------------------------------------------------------------------------------
void CSceneManager::Command_CloseAll()
{
	// initialize
	TVecSceneIter Iter = m_VecScenes.begin();
	while (Iter != m_VecScenes.end())
	{
		//m_pCoreSDK->AddCommand(new Command<CScene, CScene>(&CScene::Release, *Iter));

        (*Iter)->Release();
		++Iter;			
	}
	m_VecScenes.clear();
}

//----------------------------------------------------------------------------------------------
bool CSceneManager::LoadFromFile(const std::string &filename)
{
	bool bResult = false;

    oes::rflex::TClassFactory *classFactory = oes::rflex::GetClassFactoryA();

	Explorer3D * SourceObject = m_pCoreSDK->GetExplorerInstance()->GetExplorer3D();

	std::string sFilename;

	if (FindFile(filename.c_str(), &sFilename) && SourceObject)
	{
		XML_PARSE_FILE(sFilename.c_str())
		{
			std::string V = std::string(xml_current_tree->Value());

			oes::rflex::CClassFactory<
				IObjectAbstract,
                oes::rflex::TGenFunction,
                oes::rflex::TCloneFunction>::TMapGenerator::const_iterator Iter = classFactory->m_MapGenerator.begin();

			for (;Iter != classFactory->m_MapGenerator.end(); ++Iter)
			{
				XML_CHECK_NODE(Iter->first)
				{
					XML_DEF_ATTRIBUTES_NODE(Transient);
					XML_DEF_ATTRIBUTES_NODE(Value);

					ValueParser ValueStore(Value);
					if (Transient == "0" || Transient == "")
					{
						CActor * NewObject = static_cast<CActor*>(GENERATE_OBJECT_SDK(xml_current_tree->Value(), ValueStore.GetValueString("Name").c_str(), SourceObject));
						if (NewObject)
						{
							SourceObject->AddChildNode(NewObject);
							NewObject->SuperDeserializer(XML_CURRENT_NODE);

							CScene * pScene = static_cast<CScene*>(NewObject);

							if (pScene)
							{
								TVecSceneIter IterFind = std::find(m_VecScenes.begin(), m_VecScenes.end(), pScene);
								if (IterFind == m_VecScenes.end())
								{
									m_VecScenes.push_back(pScene);
								}
							}
						}
					}
				}
			}
		}

		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CSceneManager::LoadFromXML(tinyxml2::XMLElement *xml_current_tree)
{
    oes::rflex::TClassFactory *classFactory = oes::rflex::GetClassFactoryA();

	Explorer3D *pSourceObject = m_pCoreSDK->GetExplorerInstance()->GetExplorer3D();

	std::string V = std::string(xml_current_tree->Value());

	XML_DEF_ATTRIBUTES_NODE(Transient);
	XML_DEF_ATTRIBUTES_NODE(Value);

	ValueParser ValueStore(Value);
	if (Transient == "0" || Transient == "")
	{
		CActor * NewObject = static_cast<CActor*>(GENERATE_OBJECT_SDK(xml_current_tree->Value(), ValueStore.GetValueString("Name").c_str(), pSourceObject));
		if (NewObject)
		{
			pSourceObject->AddChildNode(NewObject);
			NewObject->SuperDeserializer(XML_CURRENT_NODE);

			CScene * pScene = static_cast<CScene*>(NewObject);

			if (pScene)
			{
				TVecSceneIter IterFind = std::find(m_VecScenes.begin(), m_VecScenes.end(), pScene);
				if (IterFind == m_VecScenes.end())
				{
					m_VecScenes.push_back(pScene);
				}
			}
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------
void CSceneManager::Initialize()
{
	// initialize
	TVecSceneIter Iter = m_VecScenes.begin();
	while (Iter != m_VecScenes.end())
	{
		if (!(*Iter)->IsInitialized())
		{
			(*Iter)->Initialize();
		}
		++Iter;			
	}
}

//----------------------------------------------------------------------------------------------
void CSceneManager::CloseAll()
{
	TVecSceneIter Iter = m_VecScenes.begin();
	while (Iter != m_VecScenes.end())
	{
		(*Iter)->Release();
		++Iter;			
	}

	m_VecScenes.clear();
}

//----------------------------------------------------------------------------------------------
bool CSceneManager::IsLevelLoad(const char *strName)
{
	TVecSceneIter Iter = m_VecScenes.begin();
	while (Iter != m_VecScenes.end())
	{
		if (!strcmp((*Iter)->GetName(), strName) && (*Iter)->IsInitialized()){
			return true;
		}
		++Iter;			
	}
	return false;
}