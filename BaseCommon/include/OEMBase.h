#pragma once

#include "stddef.h"

// 
#ifdef _MSC_VER
#define ES_MSVC
#elif defined(__ghs__)
#define ES_GHSC
#elif __GNUC__ || __SNC__
#define ES_GNUC
#elif
#error "Unknown compiler"
#endif

// OS definition
#ifdef ES_MSVC
#   ifdef _M_IX86
#       define ES_X86
#       define ES_WINDOWS
#   elif _M_X64
#       define ES_X64
#       define ES_WINDOWS
#   endif
#endif

#if defined(ES_X64) || defined(ES_A64)
#define PX_P64 // pointers are 64 bit
#endif

//offset
#ifdef ES_MSVC
#define OFFSET_OF(X, Y) offsetof(X, Y)
#endif

namespace common_base
{


}