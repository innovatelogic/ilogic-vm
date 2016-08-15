#pragma once

#include "OEMBase.h"

#if (defined(ES_WINDOWS))
#ifdef REFLX_LIB_COMPILE
#define REFLX_EXPORT _declspec(dllexport)
#define EXTERN_REFLX_EXPORT   extern REFLX_EXPORT

#else
#define REFLX_EXPORT
#endif
#endif