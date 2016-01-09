#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
CUISceneManager::CUISceneManager(CCoreSDK *pSDK)
: m_pCoreSDK(pSDK)
{
}

//----------------------------------------------------------------------------------------------
CUISceneManager::~CUISceneManager()
{
}

//----------------------------------------------------------------------------------------------
void CUISceneManager::Command_LoadScene(const char *strFileName)
{
	std::vector<class CommandBase*> ArrayCmd;
 
	ArrayCmd.push_back(new CommandBOOL_ValRef<CUISceneManager, CUISceneManager, std::string>(&CUISceneManager::LoadFromFile, this, strFileName));
 
	m_pCoreSDK->AddCommandsArray(ArrayCmd);
}

//----------------------------------------------------------------------------------------------
void CUISceneManager::Command_InitializeScene(const char *strFileName)
{
	std::vector<class CommandBase*> ArrayCmd;

	ArrayCmd.push_back(new Command<CUISceneManager, CUISceneManager>(&CUISceneManager::Initialize, this));

	m_pCoreSDK->AddCommandsArray(ArrayCmd);
}

//----------------------------------------------------------------------------------------------
void CUISceneManager::Command_CloseScene(UIScene *pScene)
{
	TVecUISceneIter IterFind = std::find(m_VecUIScenes.begin(), m_VecUIScenes.end(), pScene);
	if (IterFind != m_VecUIScenes.end())
	{
		m_pCoreSDK->AddCommand(new Command<UIScene, UIScene>(&UIScene::Release, *IterFind));
		m_VecUIScenes.erase(IterFind);
	}
}

//----------------------------------------------------------------------------------------------
void CUISceneManager::Command_CloseAll()
{
	TVecUISceneIter Iter = m_VecUIScenes.begin();
	while (Iter != m_VecUIScenes.end())
	{
		m_pCoreSDK->AddCommand(new Command<UIScene, UIScene>(&UIScene::Release, *Iter));
		++Iter;			
	}
}

//----------------------------------------------------------------------------------------------
bool CUISceneManager::RemoveScene(const UIScene *pScene)
{
	bool bResult = false;

	TVecUISceneIter IterFind = std::find(m_VecUIScenes.begin(), m_VecUIScenes.end(), pScene);
	if (IterFind != m_VecUIScenes.end())
	{
		m_VecUIScenes.erase(IterFind);
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
void CUISceneManager::CloseAll()
{
	TVecUISceneIter Iter = m_VecUIScenes.begin();
	while (Iter != m_VecUIScenes.end())
	{
		(*Iter)->Release();
		++Iter;			
	}
}

//----------------------------------------------------------------------------------------------
bool CUISceneManager::LoadFromFile(const std::string &filename)
{
	bool bResult = false;

	std::string sFilename;

	NObjectFactory::TClassFactory *classFactory = GetClassFactoryA();

	Explorer2D * SourceObject = m_pCoreSDK->GetExplorerInstance()->GetExplorer2D();

	if (FindFile(filename.c_str(), &sFilename) && SourceObject)
	{
		XML_PARSE_FILE(sFilename.c_str())
		{
			std::string V = std::string(xml_current_tree->Value());

			Utility::CClassFactory<
				CObjectAbstract,
				NObjectFactory::TGenFunction,
				NObjectFactory::TCloneFunction>::TMapGenerator::const_iterator Iter = classFactory->m_MapGenerator.begin();

			for (;Iter != classFactory->m_MapGenerator.end(); ++Iter)
			{
				XML_CHECK_NODE(xml_current_tree->Value())
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

							UIScene *pScene = static_cast<UIScene*>(NewObject);

							if (pScene)
							{
								TVecUISceneIter IterFind = std::find(m_VecUIScenes.begin(), m_VecUIScenes.end(), pScene);
								if (IterFind == m_VecUIScenes.end())
								{
									m_VecUIScenes.push_back(pScene);
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
bool CUISceneManager::LoadFromXML(tinyxml2::XMLElement *pTree)
{
	return true;
}

//----------------------------------------------------------------------------------------------
void CUISceneManager::Initialize()
{
	// initialize
	TVecUISceneIter Iter = m_VecUIScenes.begin();
	while (Iter != m_VecUIScenes.end())
	{
		if (!(*Iter)->IsInitialized()){
			(*Iter)->Initialize();
		}
		++Iter;			
	}
}

//----------------------------------------------------------------------------------------------
void CUISceneManager::Update(float DeltaTime)
{
	//for_each(VecUIScenes.begin(), VecUIScenes.end(), boost::bind(&UIScene::Update, _1, DeltaTime));
}