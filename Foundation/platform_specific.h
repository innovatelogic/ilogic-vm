#pragma once

#include "OEMBase.h"
#include "TypesBase.h"
#include "mathlib.h"

#ifdef _WIN32

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.

#ifndef WINVER
//#define WINVER 0x0601		// Windows 7 or later
#define WINVER 0x0501		// Windows XP or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows version
//#define _WIN32_WINNT 0x0700     // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0502	// Windows XP.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define NOMINMAX			// For PhysX

#define ASM_CODE 1

//#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <commctrl.h>
#include <shellapi.h>	
#include <shlwapi.h>

#else
	// TODO: specify platform
#endif

#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <stack>
#include <algorithm>
#include <string>

#include <process.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h> // for rand()
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <tchar.h>

/** boost libraries*/
#include "boost/bind.hpp"
#include "boost/signals2.hpp"


#include "DefTypes.h"
#include "math_decl.h"
#include "singleton.h"

using namespace oes::common_base;

#ifndef EXPORT
#define EXPORT	            _declspec(dllexport)
#define INL_EXPORT          inline EXPORT 
#define FORCE_EXPORT        __forceinline EXPORT

#define EXTERN_EXPORT       extern EXPORT
#define INL_EXTERN_EXPORT   inline EXTERN_EXPORT
#define FORCE_EXTERN_EXPORT __forceinline EXTERN_EXPORT 
#else 
#define EXPORT	
#endif

//#pragma intrinsic(memcpy)

#define CULLMODE_CCW		TRUE

#ifdef WIN32
#pragma warning ( disable : 4251 ) // needs to have dll-interface to be used by clients of class
#endif

/** macro helpers */
#define DELETESINGLE(x)     if (x) { delete x;   x = nullptr; }     //!< Deletes an instance of a class.
#define DELETEARRAY(x)      if (x) { delete []x; x = nullptr; }     //!< Deletes an array.

#define SIZEOFOBJECT        sizeof(*this)                                   //!< Gives the size of current object. Avoid some mistakes (e.g. "sizeof(this)").
#define CLEAROBJECT         { memset(this, 0, SIZEOFOBJECT); }              //!< Clears current object. Laziness is my business. HANDLE WITH CARE.
#define DELETESINGLE(x)     if (x) { delete x;              x = nullptr; }     //!< Deletes an instance of a class.
#define DELETEARRAY(x)      if (x) { delete []x;            x = nullptr; }     //!< Deletes an array.
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)     if (x) { (x)->Release();        (x) = nullptr; }   //!< Safe D3D-style release
#endif
#define SAFE_DESTRUCT(x)    if (x) { (x)->SelfDestruct();   (x) = nullptr; }   //!< Safe NVB-style release
#define CHECKALLOC(x)       if(!x) return false;
#define CHECKALLOCX(x) assert(x);

//! Standard allocation cycle
#define SAFE_ALLOC(ptr, type)	DELETESINGLE(ptr);	ptr = new type; CHECKALLOCX(ptr);
#define SAFE_ALLOC_ARRAY(ptr, type, count) DELETEARRAY(ptr); ptr = new type[count]; CHECKALLOC(ptr);

/** standart colors ARGB format */
#define COLOR_RED   0xffff0000
#define COLOR_GREEN 0xff00ff00
#define COLOR_BLUE  0xff0000ff
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0xff000000
#define COLOR_YELLOW 0xffffff00

#define __ES(F) F ? "" : ""
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
template<class T_Function , class T_Caster >
class FMapSecondBinder
{
public:
	FMapSecondBinder(T_Function &Function)
		: m_Function(Function)
	{
	}

public:
	template<
		typename T_Key, 
		typename T_Value> inline
		void operator()( const std::pair<const T_Key, T_Value> &Pair )const
	{
		m_Function.operator()(static_cast<T_Caster>(Pair.second));
	}

	template<
		typename T_Key, 
		typename T_Value> inline
		void operator()( std::pair<const T_Key, T_Value> &Pair )const
	{
		m_Function.operator()(static_cast<T_Caster>(Pair.second));
	}

private:
	T_Function & m_Function;
};
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
template<
typename T_Function, 
typename T_Caster > inline
FMapSecondBinder<T_Function,T_Caster> map_second_binder(T_Function &Function)
{
	return FMapSecondBinder<T_Function,T_Caster>(Function);
}
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
template<class T_Caster>
class FMapForEachSecondHelper
{
public:
	template<
		typename T_Iterator, 
		typename T_Function> 
		static inline void for_each(T_Iterator First, T_Iterator Last, T_Function &Func)
	{
		std::for_each(
			First,
			Last,
			map_second_binder<T_Function,T_Caster>(Func));
	}
};
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
template<
typename T_Iterator,
typename T_Function> inline
void map_for_each_second(T_Iterator First, T_Iterator Last, T_Function &Func)
{
	typedef T_Iterator::value_type::second_type T_Caster;

	FMapForEachSecondHelper<T_Caster>::for_each(
		First,Last,Func
		);
}
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
template<
typename T_Key, typename T_Value, 
typename T_Result,
typename T_Class,
typename T_Arg>
void map_for_each_second( std::map<T_Key,T_Value> &Container, T_Result (T_Class::*Function)(T_Arg) , const T_Arg &Arg )
{
	FMapForEachSecondHelper<T_Class *>::for_each(
		Container.begin(),
		Container.end(),
		std::bind2nd(std::mem_fun(Function),Arg));
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
template<class T> class Ptr {
public:
	Ptr(unsigned int n) { m_Ptr = (n ? new T[n] : 0); }
	~Ptr() { if (m_Ptr) delete [] m_Ptr; }
	T& operator[](unsigned int i) const { return m_Ptr[i]; }
	size_t sizeof_T() const { return sizeof(T); }
private:
	T* m_Ptr;
};

//----------------------------------------------------------------------------------------------
#define BYTE_OFFSET_CPP(name) \
		(BYTE*)&((ThisClass*)nullptr)->name - (BYTE*)nullptr

#define ARRAY_COUNT_SIZE( array ) \
	( sizeof(array) / sizeof((array)[0]) )

//----------------------------------------------------------------------------------------------
#define appFree				free
#define appRealloc			realloc

extern "C" void* __cdecl _alloca(size_t);
#define appAlloca(size) ((size==0) ? 0 : _alloca((size+7)&~7));

//void* appMemmove(void* Dest, const void* Src, int Count);
void  appMemset(void* Dest, int C, int Count);
void  appMemzero(void* Dest, int Count);
void  appMemcpy(void* Dest, const void* Src, int Count);
void  appMemswap(void* Ptr1, void* Ptr2, DWORD Size);