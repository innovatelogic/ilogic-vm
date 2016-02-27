#pragma once

#include "OEMBase.h"

//namespace oes
//{
//    namespace common_base
//    {
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


#define m_eps		        TFlt32(10e-6)
#define m_one		        TFlt32(1.0f)
#define m_two			    TFlt32(2)

#define m_zero		        TFlt32(0.0f)

#define m_pi                TFlt32(3.14159265358979323846264338327950288419716939937510582)
#define m_two_pi			TFlt32(m_pi * 2.0)

#define m_zero_5            TFlt32(0.5)

#define m_to_rad            m_pi / TFlt32(180)

#define BAD_IDX             0xffffffff

#define INDEX_NONE          -1

#define V_FLT_MAX            3.402823466e+38F        /* max value */

        // Type ranges
#define MAX_SBYTE           0x7f                            //!<    max possible sbyte value
#define MIN_SBYTE           0x80                            //!<    min possible sbyte value
#define MAX_UBYTE           0xff                            //!<    max possible ubyte value
#define MIN_UBYTE           0x00                            //!<    min possible ubyte value
#define MAX_SWORD           0x7fff                          //!<    max possible sword value
#define MIN_SWORD           0x8000                          //!<    min possible sword value
#define MAX_UWORD           0xffff                          //!<    max possible uword value
#define MIN_UWORD           0x0000                          //!<    min possible uword value
#define MAX_SDWORD          0x7fffffff                      //!<    max possible sdword value
#define MIN_SDWORD          0x80000000                      //!<    min possible sdword value
#define MAX_UDWORD          0xffffffff                      //!<    max possible udword value
#define MIN_UDWORD          0x00000000                      //!<    min possible udword value
#define MAX_FLOAT           FLT_MAX                         //!<    max possible float value
#define MIN_FLOAT           (-FLT_MAX)                      //!<    min possible loat value
#define IEEE_1_0            0x3f800000                      //!<    integer representation of 1.0
#define IEEE_255_0          0x437f0000                      //!<    integer representation of 255.0
#define IEEE_MAX_FLOAT      0x7f7fffff                      //!<    integer representation of MAX_FLOAT
#define IEEE_MIN_FLOAT      0xff7fffff                      //!<    integer representation of MIN_FLOAT

//}
//}