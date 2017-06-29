#ifndef _std_afx_d3ddriver_h_
#define _std_afx_d3ddriver_h_

#ifdef _MSC_VER
#pragma warning(disable : 4996) // _CRT_SECURE_NO_DEPRECATE
#endif

#ifdef _WIN32
#pragma once
#define D3DX_API
#else 
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "comctl32.lib" )

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include "module.h"

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <assert.h>

#include <vector>
#include <stack>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <fstream>
#include <tchar.h>
#include <stdint.h>

#include "gl/glew.h"
#include <gl/gl.h>			// Header File For The OpenGL32 Library
#include <gl/glu.h>			// Header File For The GLu32 Library


#define DECL_CLASS_SIMPLE( className, baseClassName ) \
	typedef baseClassName Super;\
	typedef baseClassName BaseClass;\
	typedef className ThisClass;\

#if defined(DEBUG) || defined(_DEBUG)
#else
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#define FOURCC_(a, b, c, d) \
	((unsigned int)((unsigned int)(a) ) | \
	((unsigned int)(b) << 8) | \
	((unsigned int)(c) << 16) | \
	((unsigned int)(d) << 24))

#define MAX_MIPMAP_NUM 16

typedef float Matrix4f[16];
typedef float Point3f[3];

//----------------------------------------------------------------------------------------------
// Name: enum VertexProcessingType
// Desc: Enumeration of all possible D3D vertex processing types.
//----------------------------------------------------------------------------------------------
enum VertexProcessingType
{
	SOFTWARE_VP,
	MIXED_VP,
	HARDWARE_VP,
	PURE_HARDWARE_VP
};

/** Render states */
typedef enum _RENDERSTATETYPE {
	RS_ZENABLE                   = 7,    /* D3DZBUFFERTYPE (or TRUE/FALSE for legacy) */
	RS_FILLMODE                  = 8,    /* D3DFILLMODE */
	RS_SHADEMODE                 = 9,    /* D3DSHADEMODE */
	RS_ZWRITEENABLE              = 14,   /* TRUE to enable z writes */
	RS_ALPHATESTENABLE           = 15,   /* TRUE to enable alpha tests */
	RS_LASTPIXEL                 = 16,   /* TRUE for last-pixel on lines */
	RS_SRCBLEND                  = 19,   /* D3DBLEND */
	RS_DESTBLEND                 = 20,   /* D3DBLEND */
	RS_CULLMODE                  = 22,   /* D3DCULL */
	RS_ZFUNC                     = 23,   /* D3DCMPFUNC */
	RS_ALPHAREF                  = 24,   /* D3DFIXED */
	RS_ALPHAFUNC                 = 25,   /* D3DCMPFUNC */
	RS_DITHERENABLE              = 26,   /* TRUE to enable dithering */
	RS_ALPHABLENDENABLE          = 27,   /* TRUE to enable alpha blending */
	RS_FOGENABLE                 = 28,   /* TRUE to enable fog blending */
	RS_SPECULARENABLE            = 29,   /* TRUE to enable specular */
	RS_FOGCOLOR                  = 34,   /* D3DCOLOR */
	RS_FOGTABLEMODE              = 35,   /* D3DFOGMODE */
	RS_FOGSTART                  = 36,   /* Fog start (for both vertex and pixel fog) */
	RS_FOGEND                    = 37,   /* Fog end      */
	RS_FOGDENSITY                = 38,   /* Fog density  */
	RS_RANGEFOGENABLE            = 48,   /* Enables range-based fog */
	RS_STENCILENABLE             = 52,   /* BOOL enable/disable stenciling */
	RS_STENCILFAIL               = 53,   /* D3DSTENCILOP to do if stencil test fails */
	RS_STENCILZFAIL              = 54,   /* D3DSTENCILOP to do if stencil test passes and Z test fails */
	RS_STENCILPASS               = 55,   /* D3DSTENCILOP to do if both stencil and Z tests pass */
	RS_STENCILFUNC               = 56,   /* D3DCMPFUNC fn.  Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
	RS_STENCILREF                = 57,   /* Reference value used in stencil test */
	RS_STENCILMASK               = 58,   /* Mask value used in stencil test */
	RS_STENCILWRITEMASK          = 59,   /* Write mask applied to values written to stencil buffer */
	RS_TEXTUREFACTOR             = 60,   /* D3DCOLOR used for multi-texture blend */
	RS_WRAP0                     = 128,  /* wrap for 1st texture coord. set */
	RS_WRAP1                     = 129,  /* wrap for 2nd texture coord. set */
	RS_WRAP2                     = 130,  /* wrap for 3rd texture coord. set */
	RS_WRAP3                     = 131,  /* wrap for 4th texture coord. set */
	RS_WRAP4                     = 132,  /* wrap for 5th texture coord. set */
	RS_WRAP5                     = 133,  /* wrap for 6th texture coord. set */
	RS_WRAP6                     = 134,  /* wrap for 7th texture coord. set */
	RS_WRAP7                     = 135,  /* wrap for 8th texture coord. set */
	RS_CLIPPING                  = 136,
	RS_LIGHTING                  = 137,
	RS_AMBIENT                   = 139,
	RS_FOGVERTEXMODE             = 140,
	RS_COLORVERTEX               = 141,
	RS_LOCALVIEWER               = 142,
	RS_NORMALIZENORMALS          = 143,
	RS_DIFFUSEMATERIALSOURCE     = 145,
	RS_SPECULARMATERIALSOURCE    = 146,
	RS_AMBIENTMATERIALSOURCE     = 147,
	RS_EMISSIVEMATERIALSOURCE    = 148,
	RS_VERTEXBLEND               = 151,
	RS_CLIPPLANEENABLE           = 152,
	RS_POINTSIZE                 = 154,   /* float point size */
	RS_POINTSIZE_MIN             = 155,   /* float point size min threshold */
	RS_POINTSPRITEENABLE         = 156,   /* BOOL point texture coord control */
	RS_POINTSCALEENABLE          = 157,   /* BOOL point size scale enable */
	RS_POINTSCALE_A              = 158,   /* float point attenuation A value */
	RS_POINTSCALE_B              = 159,   /* float point attenuation B value */
	RS_POINTSCALE_C              = 160,   /* float point attenuation C value */
	RS_MULTISAMPLEANTIALIAS      = 161,  // BOOL - set to do FSAA with multisample buffer
	RS_MULTISAMPLEMASK           = 162,  // DWORD - per-sample enable/disable
	RS_PATCHEDGESTYLE            = 163,  // Sets whether patch edges will use float style tessellation
	RS_DEBUGMONITORTOKEN         = 165,  // DEBUG ONLY - token to debug monitor
	RS_POINTSIZE_MAX             = 166,   /* float point size max threshold */
	RS_INDEXEDVERTEXBLENDENABLE  = 167,
	RS_COLORWRITEENABLE          = 168,  // per-channel write enable
	RS_TWEENFACTOR               = 170,   // float tween factor
	RS_BLENDOP                   = 171,   // D3DBLENDOP setting
	RS_POSITIONDEGREE            = 172,   // NPatch position interpolation degree. D3DDEGREE_LINEAR or D3DDEGREE_CUBIC (default)
	RS_NORMALDEGREE              = 173,   // NPatch normal interpolation degree. D3DDEGREE_LINEAR (default) or D3DDEGREE_QUADRATIC
	RS_SCISSORTESTENABLE         = 174,
	RS_SLOPESCALEDEPTHBIAS       = 175,
	RS_ANTIALIASEDLINEENABLE     = 176,
	RS_MINTESSELLATIONLEVEL      = 178,
	RS_MAXTESSELLATIONLEVEL      = 179,
	RS_ADAPTIVETESS_X            = 180,
	RS_ADAPTIVETESS_Y            = 181,
	RS_ADAPTIVETESS_Z            = 182,
	RS_ADAPTIVETESS_W            = 183,
	RS_ENABLEADAPTIVETESSELLATION = 184,
	RS_TWOSIDEDSTENCILMODE       = 185,   /* BOOL enable/disable 2 sided stenciling */
	RS_CCW_STENCILFAIL           = 186,   /* D3DSTENCILOP to do if ccw stencil test fails */
	RS_CCW_STENCILZFAIL          = 187,   /* D3DSTENCILOP to do if ccw stencil test passes and Z test fails */
	RS_CCW_STENCILPASS           = 188,   /* D3DSTENCILOP to do if both ccw stencil and Z tests pass */
	RS_CCW_STENCILFUNC           = 189,   /* D3DCMPFUNC fn.  ccw Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
	RS_COLORWRITEENABLE1         = 190,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	RS_COLORWRITEENABLE2         = 191,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	RS_COLORWRITEENABLE3         = 192,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	RS_BLENDFACTOR               = 193,   /* D3DCOLOR used for a constant blend factor during alpha blending for devices that support D3DPBLENDCAPS_BLENDFACTOR */
	RS_SRGBWRITEENABLE           = 194,   /* Enable rendertarget writes to be DE-linearized to SRGB (for formats that expose D3DUSAGE_QUERY_SRGBWRITE) */
	RS_DEPTHBIAS                 = 195,
	RS_WRAP8                     = 198,   /* Additional wrap states for vs_3_0+ attributes with D3DDECLUSAGE_TEXCOORD */
	RS_WRAP9                     = 199,
	RS_WRAP10                    = 200,
	RS_WRAP11                    = 201,
	RS_WRAP12                    = 202,
	RS_WRAP13                    = 203,
	RS_WRAP14                    = 204,
	RS_WRAP15                    = 205,
	RS_SEPARATEALPHABLENDENABLE  = 206,  /* TRUE to enable a separate blending function for the alpha channel */
	RS_SRCBLENDALPHA             = 207,  /* SRC blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
	RS_DESTBLENDALPHA            = 208,  /* DST blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
	RS_BLENDOPALPHA              = 209,  /* Blending operation for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */


	RS_FORCE_DWORD               = 0x7fffffff, /* force 32-bit size enum */
} RENDERSTATETYPE;

typedef enum _BLEND {
	BLEND_ZERO               = 1,
	BLEND_ONE                = 2,
	BLEND_SRCCOLOR           = 3,
	BLEND_INVSRCCOLOR        = 4,
	BLEND_SRCALPHA           = 5,
	BLEND_INVSRCALPHA        = 6,
	BLEND_DESTALPHA          = 7,
	BLEND_INVDESTALPHA       = 8,
	BLEND_DESTCOLOR          = 9,
	BLEND_INVDESTCOLOR       = 10,
	BLEND_SRCALPHASAT        = 11,
	BLEND_BOTHSRCALPHA       = 12,
	BLEND_BOTHINVSRCALPHA    = 13,
	BLEND_BLENDFACTOR        = 14, /* Only supported if D3DPBLENDCAPS_BLENDFACTOR is on */
	BLEND_INVBLENDFACTOR     = 15, /* Only supported if D3DPBLENDCAPS_BLENDFACTOR is on */
	BLEND_FORCE_DWORD        = 0x7fffffff, /* force 32-bit size enum */
} BLEND;

typedef enum
{
	DDS_FORMAT_DEFAULT = 0,
	DDS_FORMAT_RGB8,
	DDS_FORMAT_RGBA8,
	DDS_FORMAT_BGR8,
	DDS_FORMAT_ABGR8,
	DDS_FORMAT_R5G6B5,
	DDS_FORMAT_RGBA4,
	DDS_FORMAT_RGB5A1,
	DDS_FORMAT_RGB10A2,
	DDS_FORMAT_R3G3B2,
	DDS_FORMAT_A8,
	DDS_FORMAT_L8,
	DDS_FORMAT_L8A8,
	DDS_FORMAT_AEXP,
	DDS_FORMAT_YCOCG,
	DDS_FORMAT_MAX
} DDS_FORMAT_TYPE;

typedef enum
{
	DDS_COMPRESS_NONE = 0,
	DDS_COMPRESS_BC1, /* DXT1 */
	DDS_COMPRESS_BC2, /* DXT3 */
	DDS_COMPRESS_BC3, /* DXT5 */
	DDS_COMPRESS_BC3N, /* DXT5n */
	DDS_COMPRESS_BC4, /* ATI1 */
	DDS_COMPRESS_BC5, /* ATI2 */
	DDS_COMPRESS_AEXP, /* DXT5 */
	DDS_COMPRESS_YCOCG, /* DXT5 */
	DDS_COMPRESS_YCOCGS, /* DXT5 */
	DDS_COMPRESS_MAX
} DDS_COMPRESSION_TYPE;

//----------------------------------------------------------------------------------------------
// Error codes
enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOREF };

#ifdef D3DX_API
#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESETFAILED         0x8200000c
#define D3DAPPERR_NULLREFDEVICE       0x8200000d
#define D3DTRIANGLE_MAX               2048
#endif//D3DX_API

enum EEffectDefault
{
	EEF_Diffuse			= 0,
	EEF_DiffuseDbg		= 1,
	EEF_PlainDbg		= 2,
	EEF_PlainColorDbg	= 3,
	EEF_MAX				= 4,
};

//----------------------------------------------------------------------------------------------
struct D3DVertex
{
	GLfloat m_Position[3];  // position
	GLfloat m_Normal[3];	// normal
	GLfloat m_Color[3];		// color
	GLfloat m_Texture[2];	// texture coordinates 0
	GLfloat m_Texture2[2];	// texture coordinates 1
	GLfloat m_S[3];			// normalized tangent along the first texture coordinate s
	GLfloat m_T[3];			// normalized tangent along the second texture coordinate t
	GLfloat m_SxT[3];		// normalized cross product of the two previous tangents
	GLfloat m_Weights[4];	// four bones
	GLfloat m_Indices[4];	// indexes of bones
};

//----------------------------------------------------------------------------------------------
struct SD3DVertexFont
{
	GLfloat m_Position[3];  // position
	GLfloat m_Texture[2];	// texture coordinates 0
};

//----------------------------------------------------------------------------------------------
struct D3DVertexDbg
{
	GLfloat m_Position[3];  // position
	GLfloat m_Color[3];		// color
};

//----------------------------------------------------------------------------------------------
typedef	struct									
{
	GLubyte	*imageData;		// Image Data (Up To 32 Bits)
	GLuint	bpp;			// Image Color Depth In Bits Per Pixel
	GLuint	width;			// Image Width
	GLuint	height;			// Image Height
	GLuint	texID;			// Texture ID Used To Select A Texture
	GLuint	type;			// Image Type (GL_RGB, GL_RGBA)
} STextureBitmap;

//----------------------------------------------------------------------------------------------
struct SDDS_IMAGE_DATA
{
	GLsizei  width;
	GLsizei  height;
	GLint    components;
	GLenum   format;
	int      numMipMaps;
	GLubyte *pixels;
};

//----------------------------------------------------------------------------------------------
typedef struct 
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
} SVector3f;

//----------------------------------------------------------------------------------------------
typedef struct 
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
} SVector4f;

//----------------------------------------------------------------------------------------------
typedef struct 
{
	GLfloat x;
	GLfloat y;
} SVector2f;

//----------------------------------------------------------------------------------------------
typedef struct
{
	GLubyte Header[12];		// TGA File Header
} STGAHeader;

//----------------------------------------------------------------------------------------------
typedef struct
{
	GLubyte		header[6];		// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;	// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;		// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;			// Temporary Variable
	GLuint		type;	
	GLuint		Height;			// Height of Image
	GLuint		Width;			// Width ofImage
	GLuint		Bpp;			// Bits Per Pixel
} STGADesc;

//----------------------------------------------------------------------------------------------
static void Matrix4Mul(Matrix4f M, Matrix4f A, Matrix4f B)
{
	M[ 0] = A[ 0] * B[ 0] + A[ 1] * B[ 4] + A[ 2] * B[ 8] + A[ 3] * B[12];
	M[ 1] = A[ 0] * B[ 1] + A[ 1] * B[ 5] + A[ 2] * B[ 9] + A[ 3] * B[13];
	M[ 2] = A[ 0] * B[ 2] + A[ 1] * B[ 6] + A[ 2] * B[10] + A[ 3] * B[14];
	M[ 3] = A[ 0] * B[ 3] + A[ 1] * B[ 7] + A[ 2] * B[11] + A[ 3] * B[15];
	M[ 4] = A[ 4] * B[ 0] + A[ 5] * B[ 4] + A[ 6] * B[ 8] + A[ 7] * B[12];
	M[ 5] = A[ 4] * B[ 1] + A[ 5] * B[ 5] + A[ 6] * B[ 9] + A[ 7] * B[13];
	M[ 6] = A[ 4] * B[ 2] + A[ 5] * B[ 6] + A[ 6] * B[10] + A[ 7] * B[14];
	M[ 7] = A[ 4] * B[ 3] + A[ 5] * B[ 7] + A[ 6] * B[11] + A[ 7] * B[15];
	M[ 8] = A[ 8] * B[ 0] + A[ 9] * B[ 4] + A[10] * B[ 8] + A[11] * B[12];
	M[ 9] = A[ 8] * B[ 1] + A[ 9] * B[ 5] + A[10] * B[ 9] + A[11] * B[13];
	M[10] = A[ 8] * B[ 2] + A[ 9] * B[ 6] + A[10] * B[10] + A[11] * B[14];
	M[11] = A[ 8] * B[ 3] + A[ 9] * B[ 7] + A[10] * B[11] + A[11] * B[15];
	M[12] = A[12] * B[ 0] + A[13] * B[ 4] + A[14] * B[ 8] + A[15] * B[12];
	M[13] = A[12] * B[ 1] + A[13] * B[ 5] + A[14] * B[ 9] + A[15] * B[13];
	M[14] = A[12] * B[ 2] + A[13] * B[ 6] + A[14] * B[10] + A[15] * B[14];
	M[15] = A[12] * B[ 3] + A[13] * B[ 7] + A[14] * B[11] + A[15] * B[15];
}

//----------------------------------------------------------------------------------------------
static inline const GLubyte *BUFFER_OFFSET(size_t bytes)
{
    return reinterpret_cast<const GLubyte *>(0) + bytes;
}

#endif // _std_afx_d3ddriver_h_