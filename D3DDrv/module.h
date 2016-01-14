#pragma once

#ifdef LIB_COMPILE
#define D3DDRIVER_API _declspec(dllexport)
#else
#define D3DDRIVER_API _declspec(dllimport)
#endif