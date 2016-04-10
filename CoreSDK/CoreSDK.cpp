#include "coresdkafx.h"
#include "ObjectFactory.h"
#include "editor.h"
#include <memory>

#include "include/api/NpActorTemplate.h"

template <class T_CLASS, class T_BASE> class CommandLoader
	: public CommandBase
{	
	typedef bool (T_BASE::*ExecuteCommand)(const char*, T_CLASS*);
public:

	ExecuteCommand		m_pfn_RI;
	T_BASE				*m_pBase;
	T_CLASS				*m_TParam;
	std::string			m_Resource;

	CommandLoader(ExecuteCommand pfRI, T_BASE *pBase, T_CLASS *pParam, const char *resource)
		: CommandBase(INDEX_NONE)
		, m_pfn_RI(pfRI)
		, m_pBase(pBase)
		, m_TParam(pParam)
		, m_Resource(resource)
	{
	}

	virtual void DoExecute()
	{
		(m_pBase->*m_pfn_RI)(m_Resource.c_str(), m_TParam);
	}
};

//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_STREAM = std::stringstream> class CommandDeserializeStreamData
	: public CommandBase
{	
	typedef bool (T_CLASS::*ExecuteCommand)(T_STREAM&);
public:

	ExecuteCommand		m_pfn_RI;
	T_CLASS				*m_TParam;
	T_STREAM			&m_refStream;

	CommandDeserializeStreamData(ExecuteCommand pfRI, T_CLASS *pParam, T_STREAM &stream)
		: CommandBase(INDEX_NONE)
		, m_pfn_RI(pfRI)
		, m_TParam(pParam)
		, m_refStream(stream)
	{
	}

	virtual void DoExecute()
	{
		(m_TParam->*m_pfn_RI)(m_refStream);
	}
};

//----------------------------------------------------------------------------------
CCoreSDK::CCoreSDK(const ValueParser &Value, FnStopModule pfn /*= 0*/, FnStartExplorerOnCreate pfn1 /*= 0*/)
: m_pRenderSDK(NULL)
, m_pUpdateManager(NULL)
, m_pGameRealmInfo(NULL)
, m_pRootObject(NULL)
, m_pScriptDriver(NULL)
, m_pCameraManager(NULL)
, bPaused(true)
, bShowSparitalSubdivision(false)
, bShowObjectBounds(false)
, bShowGrid(true)
, m_bIsAGame(true)
, m_bIsAEditor(false)
, EditorInputFlag(EF_EDIT_2D | EF_EDIT_3D)
, bProcessCommndsState(FALSE)
, m_pfnStopModule(pfn)
, m_pfnStartExplorerOnCreate(pfn1)
, m_ObjEditControlMode(ECM_Move)
{
	// allocate instances	
	m_pRegistryInstance = std::shared_ptr<Registry>(new Registry());

#ifdef PHYS_ENABLED
	m_pPhysicsEngine = new CPhysicsEngineBullet(this);
#endif PHYS_ENABLED

	m_pScriptDriver = new ScriptDriver(this);
	m_pCameraManager = new CameraManager(this);
	m_pUpdateManager = new UpdateManager(this);
	m_pGameRealmInfo = new GameRealmInfo();
	m_pViewportManager = new CViewportManager(this);
	m_pInputManager = new CInputManager(this);
	m_pUISceneManager = new CUISceneManager(this);
	m_pSceneManager = new CSceneManager(this);
	m_pRemoteControlMgr = new CRemoteControlManager(this);

	SAFE_ALLOC(m_pRenderSDK, CRenderSDK);

	CActor *pDummyInitActor = new CActor(0);

	pDummyInitActor->SetUserData((void*)this);
	pDummyInitActor->SetUserData((void*)GetRenderSDK(), 0);

    m_pRootObject = NEW_OBJECT(Explorer, "Root", pDummyInitActor);

	if (m_pRegistryInstance){	
		m_pRegistryInstance->Load("registry.xml");
	}

	if (m_pGameRealmInfo){
		m_pGameRealmInfo->Load("GameStaticInfo.xml");
	}

	ListCommands.reserve(65539);
	ListSubCommands.reserve(65539);
	ListPhysCommands.reserve(65539);

	delete pDummyInitActor;
}

//----------------------------------------------------------------------------------
CCoreSDK::~CCoreSDK()
{
	// Final cleanup
	GetExplorerInstance()->Release(); //ProcessRelease(true);

#ifdef PHYS_ENABLED
	m_pPhysicsEngine->Release();
	DELETESINGLE(m_pPhysicsEngine);
#endif//PHYS_ENABLED

    m_pRootObject = NULL;

    m_pRegistryInstance.reset();

	
	DELETESINGLE(m_pCameraManager);
	DELETESINGLE(m_pUpdateManager);
	DELETESINGLE(m_pGameRealmInfo);
	DELETESINGLE(m_pViewportManager);
	DELETESINGLE(m_pInputManager);
    DELETESINGLE(m_pRenderSDK);
	DELETESINGLE(m_pScriptDriver);
	DELETESINGLE(m_pUISceneManager);
	DELETESINGLE(m_pSceneManager);
	DELETESINGLE(m_pRemoteControlMgr);
}

//------------------------------------------------------------------------
bool CCoreSDK::Deserialize(const char *filename, CActor *pParent/*=NULL*/)
{
	std::vector<class CommandBase*> ArrayCmd;

	ArrayCmd.push_back(new CommandLoader<CActor, CCoreSDK>(&CCoreSDK::DeserializeImpl, this, pParent, filename));

	AddCommandsArray(ArrayCmd);

	return true;
}

//----------------------------------------------------------------------------------
bool CCoreSDK::DeserializeImpl(const char *filename, CActor *pParent /*= NULL*/)
{
	bool bResult = false;

	NObjectFactory::TClassFactory *pClassFactory = NObjectFactory::GetClassFactoryA();
	assert(pClassFactory);

	std::string sFilename;
	if (FindFile(filename, &sFilename))
	{
		XML_PARSE_FILE(sFilename.c_str())
		{
			const char *pTypeName = XML_CURRENT_NODE->Value();

			ClassNode *pNode = pClassFactory->GetClassTree().Find(pTypeName);

			if (pNode)
			{
				if (pNode->IsInheritBaseClass("UIScene"))
				{
					GetUISceneManager()->Command_CloseAll();
					GetUISceneManager()->Command_LoadScene(filename);
					GetUISceneManager()->Command_InitializeScene(filename);

					bResult = true;
				} 
				else if (pNode->IsInheritBaseClass("CScene"))
				{
					GetSceneManager()->Command_CloseAll();
					GetSceneManager()->Command_LoadScene(filename);
					GetSceneManager()->Command_InitializeScene(filename);

					bResult = true;
				}
				else
				{
					// TODO: other entities
				}
			}
			else
			{
				// error
			}
		}
	}
	return bResult;
}

//----------------------------------------------------------------------------------
bool CCoreSDK::DeserializeBufferImpl(std::stringstream &stream)
{
	bool bResult = false;

	NObjectFactory::TClassFactory *pClassFactory = NObjectFactory::GetClassFactoryA();

	assert(pClassFactory);

	std::string data(stream.str());

	XML_PARSE_DATA(const_cast<char*>(data.c_str()))
	{
		XML_FOR_EACH_TREE(XML_CURRENT_NODE)
		{
			XML_CHECK_NODE("Explorer3D")
			{
				XML_FOR_EACH_TREE(XML_CURRENT_NODE)
				{
					const char *pTypeName = XML_CURRENT_NODE->Value();

					ClassNode *pNode = pClassFactory->GetClassTree().Find(pTypeName);

					if (pNode && pNode->IsInheritBaseClass("CScene"))
					{
						GetSceneManager()->LoadFromXML(XML_CURRENT_NODE);
						GetSceneManager()->Initialize();
					}
				}
			}
				
			XML_CHECK_NODE("Explorer2D")
			{
				XML_FOR_EACH_TREE(XML_CURRENT_NODE)
				{
					const char *pTypeName = XML_CURRENT_NODE->Value();

					ClassNode *pNode = pClassFactory->GetClassTree().Find(pTypeName);

					if (pNode && pNode->IsInheritBaseClass("UIScene"))
					{
						GetUISceneManager()->LoadFromXML(XML_CURRENT_NODE);
						GetUISceneManager()->Initialize();
					}
				}
			}
		}
	}
	return bResult;
}

//----------------------------------------------------------------------------------
bool CCoreSDK::Serialize(const char *filename)
{
	std::vector<class CommandBase*> ArrayCmd;

	ArrayCmd.push_back(new CommandProcessRelease<Explorer, CActor>(&CActor::ProcessRelease, GetExplorerInstance()));
	ArrayCmd.push_back(new CommandDeserialize<Explorer, CActor>(&CActor::SaveToFile, GetExplorerInstance(), filename));

	AddCommandsArray(ArrayCmd);

	return true;
}

//----------------------------------------------------------------------------------------------
bool CCoreSDK::DeserializeActor(const char *filename, CActor *SourceActor, bool bExternal /*= false*/)
{
 	std::vector<class CommandBase*> ArrayCmd;
 
	ArrayCmd.push_back(new CommandBOOL_Val<CActor, CActor, bool>(&CActor::ReleaseChilds, SourceActor, true));
	ArrayCmd.push_back(new CommandProcessRelease<CActor, CActor>(&CActor::ProcessRelease, SourceActor));

	ArrayCmd.push_back(new CommandProcessReleaseA<CRenderSDK, CRenderSDK>(&CRenderSDK::UpdateResources, GetRenderSDK(), true));
	ArrayCmd.push_back(new Command<CActor, CActor>(&CActor::ResetInitialize, SourceActor));

 	ArrayCmd.push_back(new CommandDeserializeActor<CActor>(&CActor::LoadFromFile, SourceActor, filename, bExternal));
	ArrayCmd.push_back(new Command<CActor, CActor>(&CActor::Initialize, SourceActor));
 	
 	AddCommandsArray(ArrayCmd);
	
	return true;
}

//----------------------------------------------------------------------------------------------
bool CCoreSDK::SerializeActor(const char *filename, CActor *pSourceActor, bool bExternal /*= false*/)
{
	std::vector<class CommandBase*> ArrayCmd;
 
 	ArrayCmd.push_back(new CommandProcessRelease<CActor, CActor>(&CActor::ProcessRelease, pSourceActor));
	ArrayCmd.push_back(new CommandDeserializeActor<CActor>(&CActor::SaveToFile, pSourceActor, filename, bExternal));
 
 	AddCommandsArray(ArrayCmd);

	return true;
}

//----------------------------------------------------------------------------------------------
bool CCoreSDK::ReleaseAll()
{
	std::vector<class CommandBase*> ArrayCmd;

	ArrayCmd.push_back(new CommandRelease<Explorer, CActor>(&CActor::Release, GetExplorerInstance()));
	ArrayCmd.push_back(new CommandProcessRelease<Explorer, CActor>(&CActor::ProcessRelease, GetExplorerInstance()));
	ArrayCmd.push_back(new CommandProcessReleaseA<CRenderSDK, CRenderSDK>(&CRenderSDK::UpdateResources, GetRenderSDK(), true));

	AddCommandsArray(ArrayCmd);

	return true;
}

//----------------------------------------------------------------------------------------------
bool CCoreSDK::Initialize(HWND hWnd, unsigned int Width, unsigned int Height)
{
	//m_pAllocMemory->Initialize();

	m_pRenderSDK->Initialize(hWnd, Width, Height, NULL);

#ifdef PHYS_ENABLED
	m_pPhysicsEngine->Initialize();
#endif PHYS_ENABLED

    m_pRootObject->Initialize();

	return true;
}

//----------------------------------------------------------------------------------------------
void CCoreSDK::LoopWindow(float dt)
{
	ElapsedTime = dt;
}

//----------------------------------------------------------------------------------------------
bool CCoreSDK::SimulatePhysics(float dt)
{
	if (!bPaused)
	{
		//GetPhysicsEngine()->Simulate(dt);

		GetUpdateManager()->PostPhysUpdate(dt);
	}
	return true;
}

//----------------------------------------------------------------------------------------------
void CCoreSDK::AddCommand(CommandBase* Command, bool bPhys /*= false*/)
{
	if (bPhys)
	{
		ListPhysCommands.push_back(Command);
	}
	else if (bProcessCommndsState == FALSE)
	{
		ListCommands.push_back(Command);
	}
	else
	{
		ListSubCommands.push_back(Command);
	}
}

//----------------------------------------------------------------------------------------------
void CCoreSDK::AddCommandLock(class CommandBase* Command)
{
	EnterCommandCS();

	AddCommand(Command);
	
	LeaveCommandCS();
}

//----------------------------------------------------------------------------------------------
void CCoreSDK::AddCommandsArray(std::vector<class CommandBase*> &ArrayCmd)
{
	EnterCommandCS();

	for (std::vector<class CommandBase*>::const_iterator Iter = ArrayCmd.begin(); Iter != ArrayCmd.end(); ++Iter)
	{
		AddCommand(*Iter);
	}

	LeaveCommandCS();
}

//----------------------------------------------------------------------------------------------
void CCoreSDK::ProcessCommands()
{	
	GetRenderSDK()->EnterCS();

	EnterCommandCS();

	// HACK: set default context
	D3DDriver *pDriver = GetRenderSDK()->GetRenderDriver();
	pDriver->PushContext(pDriver->GetDefaultContext());
	
	::InterlockedExchange(&bProcessCommndsState, TRUE);
	
	for_each(ListCommands.begin(), ListCommands.end(), std::mem_fun(&CommandBase::Execute));
	for_each(ListCommands.begin(), ListCommands.end(), DeleteVectorFntor());
	ListCommands.clear();

	::InterlockedExchange(&bProcessCommndsState, FALSE);

	for_each(ListSubCommands.begin(), ListSubCommands.end(), std::mem_fun(&CommandBase::Execute));
	for_each(ListSubCommands.begin(), ListSubCommands.end(), DeleteVectorFntor());
	ListSubCommands.clear();

	if (!bPaused)
	{
#ifdef PHYS_ENABLED
		m_pPhysicsEngine->PostSimulateUpdate();
#endif PHYS_ENABLED
	}
		
	if (IsNeedCollect())
	{
		ProcessRelease();
		GetRenderSDK()->SwapBuffer();
		GetRenderSDK()->SwapBuffer();
	}

	pDriver->PopContext();

	LeaveCommandCS();
	GetRenderSDK()->LeaveCS();
}

//----------------------------------------------------------------------------------
bool CCoreSDK::IsNeedCollect()
{
	return GetExplorerInstance()->IsNeedCollect();
}

//----------------------------------------------------------------------------------
bool CCoreSDK::ProcessRelease()
{
	return m_pRootObject->ProcessRelease();
}

//----------------------------------------------------------------------------------
void CCoreSDK::ProcessUpdate(float DeltaTime)
{
	m_pUpdateManager->Update(DeltaTime);

	CUISceneManager *pMgr = GetUISceneManager();
	if (pMgr){
		pMgr->Update(DeltaTime);
	}

	CameraManager * CamMgr = GetCameraManager();
	if (CamMgr){
		CamMgr->Update(DeltaTime);
	}
}

//----------------------------------------------------------------------------------
void CCoreSDK::ProcessInputMouse(Event event, MouseCode code, int x, int y, 
								 int ModifKey/*=0*/, SRenderContext *pRenderContext /*= 0*/)
{
	Matrix IndentlyMatrix;

	MouseInputData InputData;
	InputData.Code = code;
	InputData.event = event;
	InputData.MousePos = Vector2f((float)x, (float)y);
	InputData.ModifKey = ModifKey;
	InputData.pRenderContext = pRenderContext;

	if (IsAEditor() && !IsAGame())
	{
		GetViewportManager()->ProcessInputMouse(InputData);
	}
	else
	{
		if (!GetRemoteControlMgr()->ProcessInputMouse(InputData))
		{
			GetViewportManager()->ProcessInputMouse(InputData, static_cast<IDrawInterface*>(GetExplorerInstance()->GetExplorer2D()));
		}
	}
}

//----------------------------------------------------------------------------------
void CCoreSDK::ProcessMouseMove(float x, float y, 
								float dx, float dy, int ModifKey, SRenderContext *pRenderContext /*= 0*/)
{
	MouseMoveInputData InputData;

	InputData.MousePos = Vector2f((float)x, (float)y);
	InputData.DeltaPos = Vector2f((float)dx, (float)dy);
	InputData.ModifKey = ModifKey;
	InputData.pRenderContext = pRenderContext;

	CViewportManager *pMgrVprt = GetViewportManager();

	if (IsAEditor() && !IsAGame())
	{
		pMgrVprt->ProcessInputMouse(InputData);
	}
	else
	{
		if (!GetRemoteControlMgr()->ProcessInputMouse(InputData))
		{
			pMgrVprt->ProcessInputMouse(InputData, static_cast<IDrawInterface*>(GetExplorerInstance()->GetExplorer2D()));
		}
	}
}

//-----------------------------------------------------------------------------------
void CCoreSDK::ProcessMouseWheel(float ds, int x, int y, SRenderContext *pRenderContext /*= 0*/)
{
	CViewportManager *pMgrVprt = GetViewportManager();

	if (IsAEditor() && !IsAGame())
	{
		pMgrVprt->ProcessMouseWheel(ds, pRenderContext);
	}
}

//----------------------------------------------------------------------------------
void CCoreSDK::ProcessInputKey(const EventInput& InputData)
{
	CInputManager *pMgrInpt = GetInputManager();

	if (IsAEditor() && !IsAGame())
	{
		pMgrInpt->ProcessInputKey(InputData);
	}
	else
	{
		if (!GetRemoteControlMgr()->ProcessInputKey(InputData))
		{
			pMgrInpt->ProcessInputKey(InputData);
		}
	}
}

//----------------------------------------------------------------------------------
void CCoreSDK::ProcessEventCopy()
{
	if (CActor *pFocused = GetViewportManager()->GetFocusedActor())
	{
		GetExplorerInstance()->Copy(pFocused);
	}
}

//------------------------------------------------------------------------
void CCoreSDK::ProcessEventPaste()
{
	if (CActor *pFocused = GetViewportManager()->GetFocusedActor())
	{
		GetExplorerInstance()->Paste(pFocused->GetParent());
	}
}

//------------------------------------------------------------------------
void CCoreSDK::Draw()
{	
	ProcessCommands();

	m_pRenderSDK->SwapBuffer();

	if (!m_pRenderSDK->GetFreezeRender())
	{
		m_pViewportManager->Draw();
	}
}

//------------------------------------------------------------------------
void CCoreSDK::Render(SRenderContext *pContext /*= 0*/, int cps /* = 0*/)
{
	GetRenderSDK()->Render(pContext, cps);
}

//------------------------------------------------------------------------
void CCoreSDK::RebuildTransform()
{
	GetViewportManager()->RebuildTransform();
}

//------------------------------------------------------------------------
void CCoreSDK::SetCollisionDebugRender(bool bVisible)
{
	//GetPhysicsEngine()->SetRenderDebug(bVisible);
}

//------------------------------------------------------------------------
bool CCoreSDK::GetCollisionDebugRender()
{
	return false;// GetPhysicsEngine()->GetRenderDebug();
}

//----------------------------------------------------------------------------------------------
void CCoreSDK::QuitApplication()
{
	if (m_pfnStopModule)
	{
		m_pfnStopModule();
	}
}

//----------------------------------------------------------------------------------------------
void CCoreSDK::SaveState()
{
	std::vector<class CommandBase*> ArrayCmd;

	m_StreamStateHolder.clear();

	ArrayCmd.push_back(new CommandProcessRelease<Explorer, CActor>(&CActor::ProcessRelease, GetExplorerInstance()));
	ArrayCmd.push_back(new CommandStreamDeserialize<Explorer, CActor>(&CActor::SaveToBuffer, GetExplorerInstance(), m_StreamStateHolder));

	AddCommandsArray(ArrayCmd);
}

//----------------------------------------------------------------------------------------------
void CCoreSDK::RestoreState()
{
	GetSceneManager()->Command_CloseAll();
	GetUISceneManager()->Command_CloseAll();

	std::vector<class CommandBase*> ArrayCmd;

	ArrayCmd.push_back(new CommandDeserializeStreamData<CCoreSDK>(&CCoreSDK::DeserializeBufferImpl, this, m_StreamStateHolder));
	
	AddCommandsArray(ArrayCmd);
}

//----------------------------------------------------------------------------------------------
editors::TIEditor CCoreSDK::CreateEdtior(const char *name)
{
    CActor *actor = nullptr;
    {
        NEW_OBJECT_TRANSIENT_CHILD(actor, Explorer3D, name, m_pRootObject);
    }
    return std::move(editors::TIEditor(new editors::EditorBase(actor, new editors::CommandBuffer)));
}

//----------------------------------------------------------------------------------------------
core_sdk_api::PxCamera* CCoreSDK::getCamera()
{
    core_sdk_api::PxCamera *camera = nullptr;
    {
        CameraManager * CamMgr = GetCameraManager();

        camera = static_cast<core_sdk_api::PxCamera*>(CamMgr->GetActiveCamera());
    }
    return camera;
}