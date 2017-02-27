#ifndef __renderabstract_h__
#define __renderabstract_h__

#ifdef _WIN32
   #pragma once
#endif

#include "Actor.h"
#include "RenderSDKInterface.h"
#include "IRenderInterface.h"
#include "RenderTypes.h"

//----------------------------------------------------------------------------------
class RENDERSDK_API CRenderObject : 
	public CActor, public RenderSDKInterface, public oes::common_base::IRenderInterface
{
	DECLARE_CLASS_SIMPLE(CRenderObject, CActor);

public:
	// Constructor / Destructor
	CRenderObject(const CObjectAbstract *pParent);
    CRenderObject(const CRenderObject &Source);
	virtual ~CRenderObject();
	
	virtual void			SuperDeserializer(tinyxml2::XMLElement *xml_current_tree);

	virtual void			Render() const;
	virtual void			PostRender() const;

	virtual void			DoRebuildBounds() {} // render object keeps original size

	virtual bool			GetCastShadows() const { return bCastShadows; }
	virtual void			SetCastShadows(bool Value) { bCastShadows = Value; }

	virtual bool			GetReciveShadows() const { return bReciveShadows; }
	virtual void			SetReciveShadows(bool Value) { bReciveShadows = Value; }

	void					RebuildMesh();

protected:
	virtual void			DoRebuildMesh();

public:
	bool					bCastShadows;
	bool					bReciveShadows;
};

#endif//__render_abstract_h__