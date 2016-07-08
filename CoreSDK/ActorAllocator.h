#pragma once

#include "Actor.h"
#include "CoreSDK.h"
#include "IEventInterface.h"
#include "RenderSDKInterface.h"

class CCoreSDK;

class CORESDK_API ActorAllocator : 
    public CActor, 
    public RenderSDKInterface, 
    public core_sdk_api::IEventInterface
{
	DECLARE_CLASS_SIMPLE(ActorAllocator, CActor);

public:
	/** 
	* Constructor & destruction
	*/
	ActorAllocator(const CObjectAbstract *parent);
	ActorAllocator(const ActorAllocator &source) = delete;
	virtual ~ActorAllocator();

   /**
    *  Deserialize from XML file
    */
	virtual void	SuperDeserializer(tinyxml2::XMLElement *tree);

	inline_ void			SetAppMain(CCoreSDK *app);
	inline_ CCoreSDK*       GetAppMain() const { return m_pAppMain; }

	//virtual bool	DoEventReleased(const MouseInputData &input);

public:
	static bool		MoveObjectHierarchy(CActor *actorMove, const Registry *reg, bool Up = true, bool bMoveEditorVisible = false);

protected:
protected:
	CCoreSDK *m_pAppMain;
};