#ifndef __spherecollisioncontroller_brush_h__
#define __spherecollisioncontroller_brush_h__

#ifdef _WIN32
#pragma once
#endif

#include "Brush_AbstractInterface.h"
#include "SphereCollisionController.h"

class SphereCollisionController_Brush : public CSphereCollisionController, public Brush_AbstractInterface
{
	DECLARE_CLASS_SIMPLE(SphereCollisionController_Brush, CSphereCollisionController);

public:
	SphereCollisionController_Brush(const class CObjectAbstract * Parent);
	~SphereCollisionController_Brush();

protected:
private:
};


#endif//__spherecollisioncontroller_brush_h__