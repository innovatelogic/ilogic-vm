#pragma once

#include "reflx.h"
#include "NpActorTemplate.h"
#include "ActorAllocator.h"

class CORESDK_API CLevelActor : public ActorAllocator, IDrawInterface
{
	DECLARE_CLASS_SIMPLE(CLevelActor, ActorAllocator);

public:
	CLevelActor(const CObjectAbstract *pParent);
	virtual ~CLevelActor();

	virtual void			OnPropertyChanged(const char *pPropertyName);
	
	virtual void			Initialize();
	virtual void			DoDraw();

	virtual void				SetNameMesh(const char *name) { m_XRef = name; }
	virtual const std::string&	GetNameMesh() const { return m_XRef; }

protected:
	bool	Load(const char *url);
	void	Clear();

private:
	class Comp_StaticMesh			*m_pMeshComponent;
	class ActorCollisionController	*m_pCollisionObject;
	
	std::string	m_XRef;
};