#ifndef __explorer_h__
#define __explorer_h__

#ifdef _WIN32
#pragma once
#endif

#include "ActorAllocator.h"
#include "ViewportInterface.h"

class EXPORT Explorer : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(Explorer, ActorAllocator);

public:
	//Constructor/Destructor
	Explorer(const CObjectAbstract *parent);
	virtual ~Explorer();

	virtual void		SuperDeserializer(tinyxml2::XMLElement *xml_current_tree);

	virtual void		Initialize();

	void				Release();

	/**  buffer operations*/
	virtual void		Copy(CActor * Source);
	virtual void		Paste(CActor * InPlaceActor = NULL);

	virtual bool		HitTest(const Vector2f &Position) { return false; }

	class Explorer2D*	GetExplorer2D() const;
	class Explorer3D*	GetExplorer3D() const;

private:
	// registered path
	std::vector<std::string> RegisterPaths;

	// buffer data
	class CActor * BufferDataObject;

	class Explorer2D	*pExplorer2D;
	class Explorer3D	*pExplorer3D;
};
#endif//__explorer_h__