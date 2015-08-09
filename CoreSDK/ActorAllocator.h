#ifndef __actorallocator_h__
#define __actorallocator_h__

#ifdef _WIN32
#pragma once
#endif

#include "Actor.h"
#include "CoreSDK.h"
#include "IPhysicsEngine.h"
#include "RenderSDKInterface.h"

class CORESDK_API ActorAllocator : public CActor, public RenderSDKInterface
{
	DECLARE_CLASS_SIMPLE(ActorAllocator, CActor);

public:
	/** 
	* Constructor & destruction
	*/
	ActorAllocator(const CObjectAbstract * Parent);
	ActorAllocator(const ActorAllocator& Source);
	virtual ~ActorAllocator();

   /**
    *  Deserialize from XML file
    */
	virtual void	SuperDeserializer(TiXmlElement *Tree);

	inline_ void			SetAppMain(class CCoreSDK * App);
	inline_ class CCoreSDK* GetAppMain() const { return AppMain; }
	
   /** 
	*  helper accessors component management. 
	*/
	//inline_ virtual ScriptDriver*	GetObjectScriptDriver() const;

	inline_ IPhysicsEngine*	GetPhysicsEngine() const;

	virtual bool	DoEventReleased(const MouseInputData& InputData);

	virtual bool	IsBrushActivated();

public:
	static bool		MoveObjectHierarchy(CActor * ActorMove, const Registry * Reg, bool Up = true, bool bMoveEditorVisible = false);

protected:
protected:
	class CCoreSDK * AppMain;
};

#endif//__actorallocator_h__