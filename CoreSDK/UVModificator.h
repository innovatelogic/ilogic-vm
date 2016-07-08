#ifndef __uvmodificator_h__
#define __uvmodificator_h__

#ifdef _WIN32
#pragma once
#endif

#include "BaseModificatorUI.h"

class UVModificator: public BaseModificatorUI
{
	DECLARE_CLASS_SIMPLE(UVModificator, BaseModificatorUI);

public:
	UVModificator(const CObjectAbstract * Parent);
	virtual ~UVModificator();

	/** Overloaded */
	virtual void	Initialize();

	virtual void	OnPropertyChanged(const char* PropertyName);
	virtual bool	ProcessUserController(const MouseMoveInputData& InputData);

	virtual void	ApplyModificator(const CActor * Sender);
};

#endif//__uvmodificator_h__