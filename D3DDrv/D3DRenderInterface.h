#ifndef __d3drenderinterface_h__
#define __d3drenderinterface_h__

#ifdef _WIN32
#pragma once
#endif

#include "dxstdafx.h"

class D3DDriver;

class D3DDRIVER_API D3DRenderInterface
{
public:
	D3DRenderInterface(const D3DDriver* Interface);
	D3DRenderInterface(const D3DRenderInterface &Source);
	virtual ~D3DRenderInterface();

	D3DDriver* GetInterface() const;

protected:
	D3DDriver * m_pD3DInterface;
};

#endif//__d3drenderinterface_h__