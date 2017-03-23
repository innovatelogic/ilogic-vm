#pragma once

#include "reflx.h"
#include "NpActorTemplate.h"
#include "../ActorAllocator.h"
#include "../IDrawInterface.h"
#include "../include/PxLevelActor.h"

class CORESDK_API CLevelActor : public core_sdk_api::NpActorTemplate<core_sdk_api::PxLevelActor>,
                                public IDrawInterface
{
	DECLARE_CLASS_SIMPLE(CLevelActor, core_sdk_api::NpActorTemplate<core_sdk_api::PxLevelActor>);

public:
	CLevelActor(const CObjectAbstract *pParent);
	virtual ~CLevelActor();

	virtual void			OnPropertyChanged(const char *pPropertyName);
	
	virtual void			Initialize();
	virtual void			DoDraw();

	virtual void				SetNameMesh(const char *name) { m_XRef = name; }
	virtual const std::string&	GetNameMesh() const { return m_XRef; }

    bool	Load(const char *url);
    void	Clear();

protected:
private:
	class Comp_StaticMesh			*m_pMeshComponent;
	class ActorCollisionController	*m_pCollisionObject;
	
	std::string	m_XRef;
};