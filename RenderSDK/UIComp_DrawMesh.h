#ifndef __uicomp_drawmesh_h__
#define __uicomp_drawmesh_h__

#ifdef _WIN32
#pragma once
#endif

#include "RenderObject.h"

class RENDERSDK_API UIComp_DrawMesh : public CRenderObject
{
	DECLARE_CLASS_SIMPLE(UIComp_DrawMesh, CRenderObject);

public:
	UIComp_DrawMesh(const CObjectAbstract* Parent);
	UIComp_DrawMesh(const UIComp_DrawMesh &Source);
	~UIComp_DrawMesh();

protected:
private:
};

#endif//__uicomp_drawmesh_h__