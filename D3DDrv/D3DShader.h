#ifndef __d3dshader_h_
#define __d3dshader_h_

#ifdef _WIN32
#pragma once
#endif

#include "D3DRenderInterface.h"

class D3DDRIVER_API D3DShader : public D3DRenderInterface
{
	DECL_CLASS_SIMPLE(D3DShader, D3DRenderInterface);

public:
	D3DShader(const class D3DDriver * Interface);
	virtual ~D3DShader();

protected:
private:
//	LPD3DXEFFECT		Shader;
};


#endif//__d3dshader_h_