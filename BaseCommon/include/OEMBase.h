#pragma once

#include "stddef.h"
#include <cstdint>

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

#if (defined(ES_WINDOWS))
#ifdef COMMON_LIB_COMPILE
#define COMMON_BASE_EXPORT _declspec(dllexport)
#define EXTERN_COMMON_BASE_EXPORT extern COMMON_BASE_EXPORT

#else
#define COMMON_BASE_EXPORT
#endif
#endif

#ifdef ES_WINDOWS
#define inline_   
#else
#define inline_   inline
#endif

#define DATAFIELD_OFFSET(TYPE, FIELD) (uint8_t*)&((TYPE*)nullptr)->FIELD - (uint8_t*)nullptr

#define ARRAY_COUNT_SIZE( array ) \
	( sizeof(array) / sizeof((array)[0]) )


namespace common_base
{


}