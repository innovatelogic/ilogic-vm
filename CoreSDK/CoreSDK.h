//-------------------------------------------------------------------------------------
// OpenES: Open Entertainment System
// Copyright (C) 2011  Yura Gunko email: yura.gunko@gmail.com
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//-------------------------------------------------------------------------------------
#pragma once

#include "game_types.h"
#include "../Foundation/StdafxFoundation.h"
#include "ObjectAbstract.h"
#include "ieditor.h"
#include "icommand.h"

#include "include/api/BaseActor.h"
#include "include/api/PxCamera.h"

// BUS functions prototypes
// prototype functions calls from kernel to launcher
typedef int	      (*CALLBACK_DialogInputText) (LPCTSTR szTitle, LPCTSTR szPrompt, LPTSTR szResult, DWORD nResultSize, bool bMultiLine);
typedef void	  (*FnStopModule) (void);
typedef void	  (*FnStartExplorerOnCreate) (const class CActor *pSender, ESystemEventID EventId);

class CORESDK_API CCoreSDK
{
	DECLARE_CLASS_SIMPLE(CCoreSDK, CObjectAbstract);

public:
	// Construction / destruction 
	CCoreSDK(const class ValueParser &Value, FnStopModule pfn = 0, FnStartExplorerOnCreate pfn1 = 0);
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
	inline_ class ScriptDriver*				GetScriptDriver()	const { return m_pScriptDriver; } 

#ifdef PHYS_ENABLED
	inline_ class IPhysicsEngine*			GetPhysicsEngine()	const { return m_pPhysicsEngine; }
#endif//PHYS_ENABLED

	inline_ class Explorer*					GetExplorerInstance()	const { return m_pRootObject; }
	inline_ class CameraManager*			GetCameraManager()	const { return m_pCameraManager; }
	inline_ class UpdateManager*			GetUpdateManager()	const { return m_pUpdateManager; }
	inline_ class GameRealmInfo*			GetRealmInfo()		const { return m_pGameRealmInfo; }
	inline_ class CViewportManager*			GetViewportManager() const { return m_pViewportManager; }
	inline_ class CInputManager*			GetInputManager() const { return m_pInputManager; }
	inline_ class CUISceneManager*			GetUISceneManager() const { return m_pUISceneManager; }
	inline_ class CSceneManager*			GetSceneManager() const { return m_pSceneManager; }
	inline_ class CRemoteControlManager*	GetRemoteControlMgr() const { return m_pRemoteControlMgr; }

	void	Draw();
	void	ProcessInputMouse(Event event, MouseCode code, int x, int y, int ModifKey = 0, class SRenderContext *pRenderContext = 0);
	void	ProcessMouseMove(float x, float y, float dx, float dy, int ModifKey, class SRenderContext *pRenderContext = 0);
	void	ProcessMouseWheel(float ds, int x, int y, class SRenderContext *pRenderContext = 0);
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

    /*!
        Editor's management
    */
    editors::TIEditor       CreateEdtior(const char *name);


    //////////////////////////////////////////////////////////////////////////
    // API Section

    core_sdk_api::PxCamera* getCamera();




    //////////////////////////////////////////////////////////////////////////

private:
	bool					DeserializeImpl(const char *filename, class CActor *pParent = NULL);
	bool					DeserializeBufferImpl(std::stringstream &stream);

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
	class GameRealmInfo				*m_pGameRealmInfo;
	class ScriptDriver				*m_pScriptDriver;
	class CViewportManager			*m_pViewportManager;
	class CInputManager				*m_pInputManager; 
	class CUISceneManager			*m_pUISceneManager;
	class CSceneManager				*m_pSceneManager;
	class CRemoteControlManager		*m_pRemoteControlMgr;

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
	FnStartExplorerOnCreate			m_pfnStartExplorerOnCreate;

	std::stringstream				m_StreamStateHolder;

	EObjEditControlMode				m_ObjEditControlMode;

public:
	float  ElapsedTime;

	/*main App pause flag*/
	bool bPaused;

	unsigned int EditorInputFlag;
};