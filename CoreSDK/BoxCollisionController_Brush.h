#ifndef __boxcollisioncontroller_brush_h__
#define __boxcollisioncontroller_brush_h__

#ifdef _WIN32
#pragma once
#endif

#include "Brush_AbstractInterface.h"
#include "BoxCollisionController.h"

class BoxCollisionController_Brush : public BoxCollisionController, public Brush_AbstractInterface
{
	DECLARE_CLASS_SIMPLE(BoxCollisionController_Brush, BoxCollisionController);

public:
	BoxCollisionController_Brush(const class CObjectAbstract * Parent);
	BoxCollisionController_Brush(const BoxCollisionController_Brush & Source);
	~BoxCollisionController_Brush();

protected:
private:
};


#endif//__boxcollisioncontrolller_brush_h__