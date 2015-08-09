#ifndef __UIPanel_h__
#define __UIPanel_h__

#ifdef _WIN32
#pragma once
#endif

#include "UIScreenObject.h"
#include "VisualStyle.h"

class EXPORT UIPanel : public UIScreenObject
{
	DECLARE_CLASS_SIMPLE(UIPanel, UIScreenObject);

public:
	UIPanel(const CObjectAbstract * Parent);
	UIPanel(const UIPanel &Source);
	virtual ~UIPanel();

	//MESSAGE_FUNC( OnTick, "Tick" );

protected:
protected:

};

#endif//__UIPanel_h__