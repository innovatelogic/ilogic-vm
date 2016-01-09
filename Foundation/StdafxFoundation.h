#ifndef __stdafxfoundation_h__
#define __stdafxfoundation_h__

 #ifdef _WIN32
     #pragma once
 #else 
     ;//
 #endif

#include "platform_specific.h"
#include "ObjectDeclaration.h"
#include "MemoryAllocator.h"
#include "mathlib.h"
#include "geometry.h"
#include "UtMemory.h"
#include "UtFile.h"
#include "CriticalSection.h"
#include "MessageMap.h"
#include "xmlparser.h"
#include "ValueParser.h"
#include "TArray.h"
#include "ClassNode.h"
#include "ClassTree.h"
#include "IListener.h"

/**
 *  class prototypes
 */
class tinyxml2::XMLElement;

//#define FINAL_RELEASE 1

#define PI_CONST		3.14159265358979323846f

#define NUM_SHADOWSUBFRAMES 8

#define H_Transform 0
#define V_Transform 1

/** position states for UI Controls */
enum EPixelTransform
{
	PixelOwner_Transform        = 0,
	PixelViewport_Transform		= 1,
	PersentageOwner_Transform   = 2,
	PersentageViewport_Transform = 3,
};

enum EScrObjectEvent
{
	SOEvent_None      = 0,
	SOEvent_FreeMove  = 1,
	SOEvent_MoveX     = 2,
	SOEvent_MoveY     = 3,
	SOEvent_MoveZ     = 4,
	SOEvent_ScaleRX   = 5,
	SOEvent_ScaleLX   = 6,
	SOEvent_ScaleTY   = 7,
	SOEvent_ScaleBY   = 8,
	SOEvent_ScaleTRXY = 9,
	SOEvent_ScaleTLXY = 10,
	SOEvent_ScaleBRXY = 11,
	SOEvent_ScaleBLXY = 12,
	SOEvent_ControlLockX = 13,
	SOEvent_ControlLockY = 14,
	SOEvent_ControlLockZ = 15,
	SOEvent_ControlLockXY = 16,
	SOEvent_ControlLockYZ = 17,
	SOEvent_ControlLockXZ = 18,
	SOEvent_ControlLockOrigin = 19,
};

/** input modificator key*/
enum EModifKey
{
   MK_None		= 0,
   MK_Control	= 1,
   MK_Shift		= 2,
   MK_Alt		= 4,
   MK_MMButton	= 5,
};

/** system events */
enum ESystemEventID
{
	Event_None				= 0,
	Event_ObjectGenerated	= 1,
	Event_AddObjectChild	= 2,
	Event_PreLoad			= 3,
	Event_PostLoad			= 4,
	Event_PostInitialize	= 5,
	Event_ObjectRename		= 6,
	Event_ObjectReArranged	= 7,
	Event_PreCopy			= 8,
	Event_PostCopy			= 9,
	Event_PreRelease		= 10,
	Event_PostRelease		= 11,
	Event_OnSelected		= 12,
	Event_OnDeselected		= 13,
	Event_OnChangePivot		= 14,
	Event_OnChangeExtends	= 15,
	Event_Updated			= 16,
	Event_OnPressed			= 17,
	Event_OnRemoveObject	= 18,
	Event_PropertyChanged	= 19,
	Event_OnPressReleased	= 20,
	Event_MoveObjectUp		= 21,
	Event_MoveObjectDown	= 22,
	// TODO: add new

	Event_ProxyKeyInput		= 23,
	Event_ChangeEditCtrMode = 24,

	Event_MAX,
};

enum EDIT_FLAG
{
	EF_NONE		= 0x00000000,
	EF_EDIT_2D	= 0x00000001,
	EF_EDIT_3D	= 0x00000010,
};

enum ESceneInputmode
{
	INPUTMODE_None			= 0,
	INPUTMODE_ActiveOnly	= 1,
	ESceneInputmode_MAX,
};

enum EHandleInputKey
{
	HIK_HandleInputKey = 0,
	HIK_IgnoreInputKey,
	EHandleInputKey_MAX,
};

enum EHandleInputMouse
{
	HIM_HandleInputMouse = 0,
	HIM_IgnoreInputMouse,
	EHandleInputMouse_MAX,
};

enum EPhysEventCallback
{
	EPhysEventContact		= 0,
	EPhysEventContactsSum	= 1,
	EPhysEventMAX,
};

enum EObjEditControlMode
{
	ECM_Move = 0,
	ECM_Rotate,
	ECM_Scale
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
typedef std::map<std::string, class CActor*>	TMapActorChild;
typedef TMapActorChild::iterator				TMapActorChildIterator;
typedef TMapActorChild::const_iterator			TMapActorChildConstIterator;
typedef TMapActorChild::reverse_iterator		TMapActorChildRevIterator;

typedef std::vector<class CActor*>				TVecActorChild;
typedef TVecActorChild::iterator				TVecActorChildIterator;
typedef TVecActorChild::const_iterator			TVecActorChildConstIterator;
typedef TVecActorChild::reverse_iterator		TVecActorChildRevIterator;
typedef TVecActorChild::const_reverse_iterator	TVecActorChildRevConstIterator;

typedef std::map<std::string, class Property_Base*> TMapProperty;
typedef TMapProperty::iterator					TMapPropertyIterator;
typedef TMapProperty::const_iterator			TMapPropertyConstIterator;

typedef std::vector<class Property_Base*>		TVecProperty;
typedef TVecProperty::iterator					TVecPropertyIterator;
typedef TVecProperty::const_iterator			TVecPropertyConstIterator;

typedef std::vector<struct SInterfaceDecl*>	TVecInterfaceDecl;
typedef TVecInterfaceDecl::iterator			TVecInterfaceDeclIter;
typedef TVecInterfaceDecl::const_iterator	TVecInterfaceDeclConstIter;

// typename class EventAbstract<int>*				TypeEventAbstract;
// typename class EventKeyInputProxy<int>*		TypeEventKeyInputProxy;

template<class T> class EventAbstract;
template<class T> class EventKeyInputProxy;

typedef std::vector< class EventAbstract<CActor>*> TVecEvents;
typedef TVecEvents::iterator					TVecEventsIterator;
typedef TVecEvents::const_iterator				TVecConstEventsIterator;

//----------------------------------------------------------------------------------------------
struct DeleteVectorFntor
{
	// Overloaded () operator.
	// This will be called by for_each() function.
	template<typename T>
	void operator()(const T* x) const
	{
		delete x; // Delete pointer
	}
};

//----------------------------------------------------------------------------------------------
// Base command
//----------------------------------------------------------------------------------------------
class CommandBase
{
public:
	CommandBase(int CallMax = INDEX_NONE)
		: CallCountMax(CallMax)
		, CallCount(0)
	{
	}

	virtual ~CommandBase()
	{
	}

	void Execute()
	{
		if (CallCountMax == INDEX_NONE || CallCount < CallCountMax)
		{
			DoExecute();
			IncCall();
		}
	}
	void RedoExecute() { DoRedoExecute(); }

protected:
	virtual void DoExecute() {}
	virtual void DoRedoExecute() {}

	int GetCallCount() const { return CallCount; }
	int GetCallCountMax() const { return CallCountMax; }

	void IncCall() { CallCount++; }

private:
	int CallCount;
	int CallCountMax;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE> class Command
: public CommandBase
{	
	typedef void (T_BASE::*ExecuteCommand)();
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;

	Command(ExecuteCommand pfRI, T_CLASS* Param)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)();
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE = T_CLASS, class T_LISTENER = IListener> class CommandL
: public CommandBase
{	
	typedef void (T_BASE::*ExecuteCommand)(const wchar_t *url, T_LISTENER *pListener);

public:
	ExecuteCommand		m_pFn;
	std::wstring		URL;
	T_CLASS				*m_pTParam;
	T_LISTENER			*m_pTListener;
	
	CommandL(ExecuteCommand pFn, const wchar_t *url, T_CLASS *pParam, T_LISTENER *pListener)
		: CommandBase(INDEX_NONE)
		, m_pFn(pFn)
		, URL(url)
		, m_pTParam(pParam)
		, m_pTListener(pListener)
	{
	}

	virtual void DoExecute()
	{
		(m_pTParam->*m_pFn)(URL.c_str(), m_pTListener);
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE> class CommandBOOL
: public CommandBase
{	
	typedef bool (T_BASE::*ExecuteCommand)();
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;

	CommandBOOL(ExecuteCommand pfRI, T_CLASS* Param)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)();
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE, class T_PARAM1> class CommandBOOL_Val
: public CommandBase
{	
	typedef bool (T_BASE::*ExecuteCommand)(T_PARAM1);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;
	T_PARAM1			TValue;

	CommandBOOL_Val(ExecuteCommand pfRI, T_CLASS* Param, T_PARAM1 Value)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, TValue(Value)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)(TValue);
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE, class T_PARAM1> class Command_Val
: public CommandBase
{	
	typedef void (T_BASE::*ExecuteCommand)(T_PARAM1);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;
	T_PARAM1			TValue;

	Command_Val(ExecuteCommand pfRI, T_CLASS* Param, T_PARAM1 Value)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, TValue(Value)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)(TValue);
	}
};
//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE, class T_PARAM1, class T_PARAM2> class Command_Val2
: public CommandBase
{	
	typedef void (T_BASE::*ExecuteCommand)(T_PARAM1*, T_PARAM2);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;
	T_PARAM1			*TValue1;
	T_PARAM2			TValue2;

	Command_Val2(ExecuteCommand pfRI, T_CLASS* Param, T_PARAM1 *Value1, T_PARAM2 Value2)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, TValue1(Value1)
		, TValue2(Value2)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)(TValue1, TValue2);
	}
};
//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE, class T_PARAM1> class CommandBOOL_ValRef
: public CommandBase
{	
	typedef bool (T_BASE::*ExecuteCommand)(const T_PARAM1&);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;
	T_PARAM1			TValue;

	CommandBOOL_ValRef(ExecuteCommand pfRI, T_CLASS* Param, const T_PARAM1 &Value)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, TValue(Value)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)(TValue);
	}
};

typedef std::vector<class CommandBase*> TVCommandList;
typedef TVCommandList::iterator			TVCommandListIter;
typedef TVCommandList::const_iterator	TVCommandListConstIter;

//----------------------------------------------------------------------------------------------
struct MouseMoveInputData
{
	Vector2f MousePos;
	Vector2f DeltaPos;
	int	ModifKey;
	bool bMiddleButtonPressed;
	class SRenderContext *pRenderContext;

	MouseMoveInputData()
	: ModifKey(0)
	, bMiddleButtonPressed(false)
	, pRenderContext(0)
	{
		MousePos = DeltaPos = Vector2f(0.f, 0.f);
	}

	MouseMoveInputData(const MouseMoveInputData& Source)
	{
		if (&Source != this)
		{
			MousePos = Source.MousePos;
			DeltaPos = Source.DeltaPos;
			ModifKey = Source.ModifKey;
			bMiddleButtonPressed = Source.bMiddleButtonPressed;
			pRenderContext = Source.pRenderContext;
		}
	}
};
//----------------------------------------------------------------------------------------------
struct MouseInputData
{
	Event		event;
	MouseCode	Code;
	int			ModifKey;
	Vector2f	MousePos;
	class SRenderContext *pRenderContext;

	MouseInputData()
		: ModifKey(0)
		, pRenderContext(0)
	{
		MousePos = Vector2f(0.f, 0.f);
	}
};

//----------------------------------------------------------------------------------------------
typedef std::vector<class Property_Base*>	TVecPropertyBase;
typedef TVecPropertyBase::iterator			TVecPropertyBaseIter;
typedef TVecPropertyBase::const_iterator	TVecPropertyBaseConstIter;

struct SPropertyClass
{
	std::string			ClassName;
	TVecPropertyBase	InheritProperties;
	int					nOverrideByteShift; // overrides byte shift for interface 
	void				*pDataObject; // pointer to actor

	SPropertyClass(const char *Name, void *pData, int shift = -1)
		: ClassName(Name)
		, pDataObject(pData)
		, nOverrideByteShift(shift)
	{
	}
};

typedef std::vector<struct SPropertyClass*>	TVecPropertyClass;
typedef TVecPropertyClass::const_iterator	TVecPropertyClassConstIter;
typedef TVecPropertyClass::iterator			TVecPropertyClassIter;

#endif //__stdafxfoundation_h__