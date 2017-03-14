
#pragma once

#include "game_types.h"
#include "StdafxFoundation.h"
#include "ObjectAbstract.h"
#include "IEventManager.h"
#include "include/BaseActor.h"
#include "include/PxCamera.h"

// BUS functions prototypes
// prototype functions calls from kernel to launcher
typedef void	  (*FnStopModule) (void);

namespace core_sdk_api
{
    class CViewportManager;
}

class CORESDK_API CCoreSDK
{
	DECLARE_CLASS_SIMPLE(CCoreSDK, CObjectAbstract);

public:
	// Construction / destruction 
	CCoreSDK(const ValueParser &Value, FnStopModule pfn = 0);
	virtual ~CCoreSDK();
	
	bool	Deserialize(const char *filename, class CActor *pParent = NULL);
	bool	Serialize(const char *filename);

	bool	DeserializeActor(const char *filename, class CActor *SourceActor, bool bExternal = false);
	bool	SerializeActor(const char *filename, class CActor *SourceActor, bool bExternal = false);

	bool	ReleaseAll();
	
	bool	Initialize(HWND hWnd, unsigned int Width, unsigned int Height);
	void	LoopWindow(float dt);
	bool	SimulatePhysics(float dt);

	void	AddCommand(class CommandBase *Command, bool bPhys = false);
	void	AddCommandLock(class CommandBase *Command);
	void	AddCommandsArray(std::vector<class CommandBase*> &ArrayCmd);

	void	ProcessCommands();
	
	bool	IsNeedCollect();
	bool	ProcessRelease();

	void	ProcessUpdate(float DeltaTime);

	/** global resources accessors*/
	inline_ class Registry*					GetRegistry()		const { return m_pRegistryInstance.get(); }
	inline_ class CRenderSDK*				GetRenderSDK()		const { return m_pRenderSDK; }      

#ifdef PHYS_ENABLED
	inline_ class IPhysicsEngine*			GetPhysicsEngine()	const { return m_pPhysicsEngine; }
#endif//PHYS_ENABLED

    CActor*					        GetRootActor()	const;
	class Explorer*					GetExplorerInstance()	const { return m_pRootObject; }
	class CameraManager*			GetCameraManager()	const { return m_pCameraManager; }
	class UpdateManager*			GetUpdateManager()	const { return m_pUpdateManager; }
    class CInputManager*			GetInputManager() const { return m_pInputManager; }
	class CUISceneManager*			GetUISceneManager() const { return m_pUISceneManager; }
	class CSceneManager*			GetSceneManager() const { return m_pSceneManager; }
	class CRemoteControlManager*	GetRemoteControlMgr() const { return m_pRemoteControlMgr; }

    core_sdk_api::CViewportManager*	GetViewportManager() const { return m_pViewportManager; }
    oes::foundation::IEventManager* GetEventManager() const { return m_eventManager; }

	void	Draw();
	
    void	ProcessInputKey(const EventInput& InputData);

    /** Copy/Paste system events */
	void	ProcessEventCopy();
	void	ProcessEventPaste();

	void	Render(class SRenderContext *pContext = 0,/* bool bSwapBuffer = false,*/ int cps = 0);
	void	RebuildTransform();

	void	SetSparitalSubdivisionVisible(bool bVisible) { bShowSparitalSubdivision = bVisible; }
	bool	GetSparitalSubdivisionVisible()				 { return bShowSparitalSubdivision; }
	
	void	SetObjectBoundsVisible(bool bVisible) { bShowObjectBounds = bVisible; }
	bool	GetObjectBoundsVisible()			  { return bShowObjectBounds; }

	bool	GetShowGrid() {return bShowGrid; }
	void	SetShowGrid(bool bVisible) { bShowGrid = bVisible; }

	void	SetCollisionDebugRender(bool bVisible);
	bool	GetCollisionDebugRender();

	void	EnterDrawCS() { CommandDrawCS.enter(); }
	void	LeaveDrawCS() { CommandDrawCS.leave(); }

	void	EnterCommandCS() { CommandsProcessCS.enter(); }
	void	LeaveCommandCS() { CommandsProcessCS.leave(); }

	inline_ bool IsAGame() const { return m_bIsAGame; }
	void		 SetIsAGame(bool flag) { m_bIsAGame = flag; }

	inline_ bool IsAEditor() const { return m_bIsAEditor; }
	void		 SetIsAEditor(bool flag) { m_bIsAEditor = flag; }

	inline_ unsigned int	GetEditorInputFlag() const { return EditorInputFlag; }
	void					SetEditorInputFlag(unsigned int Flag) { EditorInputFlag = Flag; }

	void					QuitApplication();

	void					SetPause(bool flag) { bPaused = flag; }
	bool					GetPause() const { return bPaused; }

	/** Editor Save/Restore state */
	void					SaveState();
	void					RestoreState();

	/** Editor's object control mode */
	EObjEditControlMode		GetEditControlMode() const { return m_ObjEditControlMode; }
	void					SetEditControlMode(EObjEditControlMode mode) { m_ObjEditControlMode = mode; }


    //////////////////////////////////////////////////////////////////////////
    // Public API Section

    core_sdk_api::PxCamera* getCamera();

    bool    GetWireframeMode(SRenderContext *ctxt) const;
    void    SetWireframeMode(SRenderContext *ctxt, bool value);

    //////////////////////////////////////////////////////////////////////////

private:
	bool	DeserializeImpl(const char *filename, class CActor *pParent = NULL);
	bool	DeserializeBufferImpl(std::stringstream &stream);

private:
	std::shared_ptr<Registry> m_pRegistryInstance; 

	/** Global resources */
#ifdef PHYS_ENABLED
	class IPhysicsEngine			*m_pPhysicsEngine;
#endif//PHYS_ENABLED

	class CRenderSDK				*m_pRenderSDK;
    class Explorer					*m_pRootObject;
	class CameraManager				*m_pCameraManager;
	class UpdateManager				*m_pUpdateManager;
    core_sdk_api::CViewportManager	*m_pViewportManager;
	class CInputManager				*m_pInputManager; 
	class CUISceneManager			*m_pUISceneManager;
	class CSceneManager				*m_pSceneManager;
	class CRemoteControlManager		*m_pRemoteControlMgr;
    oes::foundation::IEventManager  *m_eventManager;

	TVCommandList					ListCommands;
	TVCommandList					ListSubCommands;
	TVCommandList					ListPhysCommands;

	CriticalSection					CommandsProcessCS;
	CriticalSection					CommandDrawCS;

	bool							bShowSparitalSubdivision;
	bool							bShowObjectBounds;
	bool							bShowGrid;

	bool							m_bIsAGame;
	bool							m_bIsAEditor;

	volatile LONG					bProcessCommndsState;

	FnStopModule					m_pfnStopModule;

	std::stringstream				m_StreamStateHolder;

	EObjEditControlMode				m_ObjEditControlMode;

public:
	float  ElapsedTime;

	/*main App pause flag*/
	bool bPaused;

	unsigned int EditorInputFlag;
};