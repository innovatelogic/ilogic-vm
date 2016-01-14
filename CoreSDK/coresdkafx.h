#ifndef __coresdkafx_h__
#define __coresdkafx_h__

#ifdef _WIN32
#pragma once
#endif

#include "game_types.h"
#include "StdafxFoundation.h"

#include "coresdkafx_manager.h"
#include "coresdkafx_objects.h"
#include "coresdkafx_ui.h"
#include "coresdkafx_components.h"
#include "coresdkafx_actions.h"

#include "ActionBase.h"
#include "ActionTrigger.h"
#include "ActionLoadLevel.h"
#include "ActionEventUI.h"
#include "ActionHandler.h"

#include "Property.h"

#ifndef __scenegamearcanoid_h__
#include "SceneGameArcanoid.h"
#endif

#define DATAFIELD_OFFSET(TYPE, FIELD) (BYTE*)&((TYPE*)nullptr)->FIELD - (BYTE*)nullptr

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE> class CommandDeserialize
: public CommandBase
{	
	typedef bool (T_BASE::*ExecuteCommand)(const char*, bool);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;
	std::string			Resource;
	bool				bExt;

	CommandDeserialize(ExecuteCommand pfRI, T_CLASS *pParam, const char *resource, bool bExternal = false)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(pParam)
		, Resource(resource)
		, bExt(bExternal)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)(Resource.c_str(), bExt);
	}
};

//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE, class T_STREAM = std::stringstream> class CommandStreamDeserialize
	: public CommandBase
{	
	typedef bool (T_BASE::*ExecuteCommand)(T_STREAM&, bool);
public:

	ExecuteCommand		m_pfn_RI;
	T_CLASS				*m_TParam;
	T_STREAM			&m_refStream;
	bool				m_bExt;

	CommandStreamDeserialize(ExecuteCommand pfRI, T_CLASS *pParam, T_STREAM &stream, bool bExternal = false)
		: CommandBase(INDEX_NONE)
		, m_pfn_RI(pfRI)
		, m_TParam(pParam)
		, m_refStream(stream)
		, m_bExt(bExternal)
	{
	}

	virtual void DoExecute()
	{
		(m_TParam->*m_pfn_RI)(m_refStream, m_bExt);
	}
};
//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS> class CommandDeserializeActor
: public CommandBase
{	
	typedef bool (T_CLASS::*ExecuteCommand)(const char*, bool);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;
	std::string			Resource;
	bool				bExt;

	CommandDeserializeActor(ExecuteCommand pfRI, T_CLASS *pParam, const char *resource, bool bExternal = false)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(pParam)
		, Resource(resource)
		, bExt(bExternal)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)(Resource.c_str(), bExt);
	}
};


//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE> class CommandRelease
: public CommandBase
{	
	typedef void (T_BASE::*ExecuteCommand)();
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;

	CommandRelease(ExecuteCommand pfRI, T_CLASS* Param)
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
template <class T_CLASS, class T_BASE> class CommandProcessRelease
: public CommandBase
{	
	typedef bool (T_BASE::*ExecuteCommand)(bool force);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;
	bool				bForce;

	CommandProcessRelease(ExecuteCommand pfRI, T_CLASS* Param, bool Force = false)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, bForce(Force)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)(bForce);
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE> class CommandProcessReleaseA
: public CommandBase
{	
	typedef void (T_BASE::*ExecuteCommand)(bool force);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;
	bool				bForce;

	CommandProcessReleaseA(ExecuteCommand pfRI, T_CLASS* Param, bool Force = false)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, bForce(Force)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)(bForce);
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE> class CommandInitializeBrushes
: public CommandBase
{	
	typedef void (T_BASE::*ExecuteCommand)();
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;

	CommandInitializeBrushes(ExecuteCommand pfRI, T_CLASS* Param)
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
template <class T_CLASS, class T_BASE> class CommandMouseInput
: public CommandBase
{	
	typedef bool (T_BASE::*ExecuteCommand)(const MouseInputData& InputData, const Matrix &WTM);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;

	Event				ParamEvent;
	MouseCode			ParamCode;
	int					ParamX;
	int					ParamY;
	int					ParamModifKey;

	CommandMouseInput(ExecuteCommand pfRI, T_CLASS* Param, Event event, MouseCode code, int x, int y, int ModifKey)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, ParamEvent(event)
		, ParamCode(code)
		, ParamX(x)
		, ParamY(y)
		, ParamModifKey(ModifKey)
	{
	}

	virtual void DoExecute()
	{
		Matrix IndentlyMatrix;

		MouseInputData InputData;
		InputData.Code = ParamCode;
		InputData.event = ParamEvent;
		InputData.MousePos = Vector2f((float)ParamX, (float)ParamY);
		InputData.ModifKey = ParamModifKey;

		(TParam->*pfn_RI)(InputData, IndentlyMatrix);
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE> class CommandKeyInput
: public CommandBase
{	
	typedef bool (T_BASE::*ExecuteCommand)(const EventInput& InputData);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;

	EventInput			ParamEventInput;

	CommandKeyInput(ExecuteCommand pfRI, T_CLASS* Param, const EventInput& InputData)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, ParamEventInput(InputData)
	{
	}

	virtual void DoExecute()
	{
		Matrix IndentlyMatrix;
		(TParam->*pfn_RI)(ParamEventInput);
	}
};
//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE> class CommandMouseMove
: public CommandBase
{	
	typedef bool (T_BASE::*ExecuteCommand)(const MouseMoveInputData& InputData, const Matrix &WTM);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;

	int					ParamX;
	int					ParamY;
	int					ParamDeltaX;
	int					ParamDeltaY;
	int					ParamModifKey;

	CommandMouseMove(ExecuteCommand pfRI, T_CLASS* Param, int x, int y, int dx, int dy, int ModifKey)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, ParamX(x)
		, ParamY(y)
		, ParamDeltaX(dx)
		, ParamDeltaY(dy)
		, ParamModifKey(ModifKey)
	{
	}

	virtual void DoExecute()
	{
		Matrix IndentlyMatrix;
		MouseMoveInputData InputData;

		InputData.MousePos = Vector2f((float)ParamX, (float)ParamY);
		InputData.DeltaPos = Vector2f((float)ParamDeltaX, (float)ParamDeltaY);
		InputData.ModifKey = ParamModifKey;
		
		(TParam->*pfn_RI)(InputData, IndentlyMatrix);
	}
};

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template <class T_CLASS, class T_BASE> class CommandMouseWheel
: public CommandBase
{	
	typedef bool (T_BASE::*ExecuteCommand)(float ds, int x, int y);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;

	int					ParamX;
	int					ParamY;
	float				Delta;

	CommandMouseWheel(ExecuteCommand pfRI, T_CLASS* Param, float ds, int x, int y)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, ParamX(x)
		, ParamY(y)
		, Delta(ds)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)(Delta, ParamX, ParamY);
	}
};

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template <class T_CLASS, class T_BASE> class CommandSetPosition
: public CommandBase
{	
	typedef void (T_BASE::*ExecuteCommand)(const Vector&);
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;
	Vector				Param;

	CommandSetPosition(ExecuteCommand pfRI, T_CLASS* Param, const Vector& Pos)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, Param(Pos)
	{
	}

	virtual void Execute() const
	{
		(TParam->*pfn_RI)(Param);
	}
};

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template <class T_CLASS, class T_BASE> class CommandSetLocalTransform
: public CommandBase
{	
	typedef void (T_BASE::*ExecuteCommand)(const Matrix&) const;
public:

	ExecuteCommand		pfn_RI;
	T_CLASS				*TParam;
	Matrix				Param;

	CommandSetLocalTransform(ExecuteCommand pfRI, T_CLASS* AParam, const Matrix& Pose)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(AParam)
	{
		Param = Pose;
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)(Param);
	}
};

#endif//__coresdkafx_h__