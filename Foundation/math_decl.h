#ifndef _math_decl_h_
#define _math_decl_h_

#ifdef _WIN32
#pragma once
#endif

typedef float scalar_t;

#define m_eps		        scalar_t(10e-6)
#define m_one		        scalar_t(1.0f)
#define m_two			    scalar_t(2)

#define m_zero		        scalar_t(0.0f)

#define m_pi                scalar_t(3.14159265358979323846264338327950288419716939937510582)
#define m_two_pi			scalar_t(m_pi * 2.0)

#define m_zero_5            scalar_t(0.5)

#define m_to_rad            m_pi / scalar_t(180)

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

#endif //_math_decl_h_