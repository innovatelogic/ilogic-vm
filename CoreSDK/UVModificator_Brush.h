#ifndef __uvmodificator_brush_h__
#define __uvmodificator_brush_h__

#ifdef _WIN32
#pragma once
#endif

#include "Brush_AbstractInterface.h"
#include "UVModificator.h"

class UVModificator_Brush : public UVModificator, public Brush_AbstractInterface
{
	DECLARE_CLASS_SIMPLE(UVModificator_Brush, UVModificator);

public:
	UVModificator_Brush(const CObjectAbstract * Parent);
	virtual ~UVModificator_Brush();

protected:
private:
};

#endif//__uvmodificator_brush_h__