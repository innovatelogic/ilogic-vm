#pragma once

#ifdef LIB_COMPILE
#define RENDERSDK_API _declspec(dllexport)
#else
#define RENDERSDK_API _declspec(dllimport)
#endif
