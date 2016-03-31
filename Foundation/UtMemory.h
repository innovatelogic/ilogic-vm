///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Contains all memory macros.
*  \file       NVBMemoryMacros.h
*  \author     Pierre Terdiman / modifyed Yur.G
*  \date       April, 4, 2000
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __memorymacros_h__
#define __memorymacros_h__

#pragma once

#include "platform_specific.h"
/*
#undef ZeroMemory
#undef CopyMemory
#undef MoveMemory
#undef FillMemory

//! A function to clear a buffer.
inline_ void EXPORT ZeroMemory(void* addr, TUInt32 size) { memset(addr, 0, size);}

//! A function to fill a buffer with a given byte.
inline_ void EXPORT FillMemory(void* dest, TUInt32 size, ubyte val) { memset(dest, val, size);}

//! A function to copy a buffer.
inline_ void EXPORT CopyMemory(void* dest, const void* src, TUInt32 size) { memcpy(dest, src, size); }

//! A function to move a buffer.
inline_ void EXPORT MoveMemory(void* dest, const void* src, TUInt32 size) { memmove(dest, src, size); }
*/
#endif // __NVBMEMORYMACROS_H__
