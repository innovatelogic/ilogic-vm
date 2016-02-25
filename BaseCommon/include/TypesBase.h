#pragma once

#include "OEMBase.h"

namespace oes
{
namespace common_base
{
#ifdef ES_WINDOWS

        typedef signed __int64      TInt64;
        typedef signed int          TInt32;
        typedef signed short        TInt16;
        typedef signed char         TInt8;

        typedef unsigned __int64    TUInt64;
        typedef unsigned int        TUInt32;
        typedef unsigned short      TUInt16;
        typedef unsigned char       TUInt8;

        typedef float               TFlt32;
        typedef double              TDbl64;

#elif defined(ES_LINUX) || defined (ES_ANDROID)

        typedef signed long long	TInt64;
        typedef signed int			TInt32;
        typedef signed short		TInt16;
        typedef signed char			TInt8;

        typedef unsigned long long	TUInt64;
        typedef unsigned int		TUInt32;
        typedef unsigned short		TUInt16;
        typedef unsigned char		TUInt8;

        typedef float				TFlt32;
        typedef double				TDbl64;

#else
#error Unknown platform

#endif

// Type ranges
#define	ES_MAX_I8			127					//maximum possible sbyte value, 0x7f
#define	ES_MIN_I8			(-128)				//minimum possible sbyte value, 0x80
#define	ES_MAX_U8			255U				//maximum possible ubyte value, 0xff
#define	ES_MIN_U8			0					//minimum possible ubyte value, 0x00
#define	ES_MAX_I16			32767				//maximum possible sword value, 0x7fff
#define	ES_MIN_I16			(-32768)			//minimum possible sword value, 0x8000
#define	ES_MAX_U16			65535U				//maximum possible uword value, 0xffff
#define	ES_MIN_U16			0					//minimum possible uword value, 0x0000
#define	ES_MAX_I32			2147483647			//maximum possible sdword value, 0x7fffffff
#define	ES_MIN_I32			(-2147483647 - 1)	//minimum possible sdword value, 0x80000000
#define	ES_MAX_U32			4294967295U			//maximum possible udword value, 0xffffffff
#define	ES_MIN_U32			0					//minimum possible udword value, 0x00000000
#define	ES_MAX_F32			3.4028234663852885981170418348452e+38F	

//maximum possible float value
#define	ES_MAX_F64			DBL_MAX				//maximum possible double value

#define ES_EPS_F32			FLT_EPSILON			//maximum relative error of float rounding
#define ES_EPS_F64			DBL_EPSILON			//maximum relative error of double rounding


}
}