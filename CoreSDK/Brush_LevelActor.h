#ifndef __brush_levelactor_h__
#define __brush_levelactor_h__

#ifdef WIN32
#pragma once
#endif

#include "Brush_AbstractInterface.h"
#include "LevelActor.h"
#include "TriggerObject.h"

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class Brush_LevelActor : public CLevelActor, public Brush_AbstractInterface
{
	DECLARE_CLASS_SIMPLE(Brush_LevelActor, CLevelActor);

public:
	Brush_LevelActor(const CObjectAbstract * Parent);
	Brush_LevelActor(const Brush_LevelActor & Source);
	virtual ~Brush_LevelActor();

	virtual bool	DoEventPressed(const MouseInputData& InputData);

protected:
private:
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class Brush_TriggerObject : public TriggerObject, public Brush_AbstractInterface
{
	DECLARE_CLASS_SIMPLE(Brush_TriggerObject, TriggerObject);

public:
	Brush_TriggerObject(const CObjectAbstract * Parent);
	Brush_TriggerObject(const Brush_TriggerObject & Source);
	virtual ~Brush_TriggerObject();

	virtual bool	DoEventPressed(const MouseInputData& InputData);

protected:
private:
};

#endif//__brush_levelactor_h__