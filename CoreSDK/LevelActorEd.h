#ifndef __levelactored_h__
#define __levelactored_h__

#ifdef WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class CORESDK_API LevelActorEd : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(LevelActorEd, ActorAllocator);

public:
	LevelActorEd(const CObjectAbstract * Parent);
	LevelActorEd(const LevelActorEd & Source);
	~LevelActorEd();

	virtual void	OnPropertyChanged(const char* PropertyName);

	virtual void	Initialize();
	virtual void	PreRelease();

	virtual void			SetXRef(std::string& xref) { XRef = xref; }
	virtual std::string		GetXRef() { return XRef; }

protected:
private:
	std::string						XRef;
	class Comp_AnimMesh				*MeshComponent;
	class Comp_MaterialEffect		*MaterialEffect;	
};

#endif//__levelactored_h__