
#pragma once


#include "platform_specific.h"
#include "module.h"
#include "D3DDriver.h"

enum RENDER_INFO
{
	RI_NONE								= 0x00000000,
	RI_NUMBER_CALLS						= 0x00000001,
	RI_RENDER_BOUNDS					= 0x00000010,
	RI_RENDER_BOUNDS_SHADOW_CASTER		= 0x00000100,
	RI_RENDER_BOUNDS_SHADOW_RECIEVERS	= 0x00001000,
};

enum EFogMode
{
	EFOG_LINEAR = 0,
	EFOG_EXP,
	EFOG_EXP2,
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
struct SDbgPoint
{
	Vector Point;
	unsigned int Color;

	SDbgPoint(const Vector& point, unsigned int color)
		: Point(point)
		, Color(color)
	{
	}
};

struct SDbgTriangle
{
	Vector P0;
	Vector P1;
	Vector P2;
	unsigned int Color;

	SDbgTriangle(const Vector &p0, const Vector &p1, const Vector &p2, unsigned int color)
		: P0(p0)
		, P1(p1)
		, P2(p2)
		, Color(color)
	{
	}
};

struct SDbgSphere
{
	Vector Pos;
	float  rad;
	unsigned int Color;
	unsigned short Seg; // short

	SDbgSphere(const Vector &pos, float rad, unsigned int color, unsigned short seg)
		: Pos(pos)
		, rad(rad)
		, Color(color)
		, Seg(seg)
	{
	}
};

struct AdjDebugInfo
{
	std::vector<SDbgPoint>	DotsList;
	std::vector<SDbgPoint>	DotsListNoZ;
	std::vector<SDbgPoint>	PointList;
	std::vector<SDbgPoint>	PointListNoZ;

	std::vector<SDbgTriangle>	TriangleList;
	std::vector<SDbgSphere>		SphereList;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------

struct RenderQuevueAdjacency
{
	size_t	IdxCommandStart;	// start command index
	size_t	NumCommands;		// end command index

	size_t	IdxObjectStart;		// start objects index
	size_t	NumObjects;			// objects number 

	Matrix	ProjMatrix;
	Matrix	ViewMatrix;

	float	fNearPlane;
	float	fFarPlane;

	Vector	ViewPos;

	const class D3DRenderTarget * RTarget;
	Matrix	RT_ProjMatrix;
	Matrix	RT_ViewMatrix;
	bool	bClearTarget;
	DWORD	m_ClearColor;

	unsigned int RenderInfoMask;

	// viewport params
	DWORD X;
	DWORD Y;
	DWORD Width;	// Zero value mean default viewport
	DWORD Height;	// Zero value mean default viewport
	float MinZ;
	float MaxZ;

	// fog params
	bool			bFog;
	EFogMode		FogMode;
	float			fFogMin;
	float			fFogMax;
	float			fFogDensity;
	unsigned int	FogColor;
	
	AdjDebugInfo DebugInfo; // debug render info

	oes::d3d::SRenderContext *pRenderContext;

	RenderQuevueAdjacency(size_t Start, size_t ObjStart)
		: IdxCommandStart(Start)
		, NumCommands(0)
		, IdxObjectStart(ObjStart)
		, NumObjects(0)
		, RTarget(0)
		, bClearTarget(false)
		, m_ClearColor(0x00000000)
		, X(0)
		, Y(0)
		, Width(0)
		, Height(0)
		, MinZ(0)
		, MaxZ(1)
		, bFog(false)
		, FogMode(EFOG_EXP2)
		, fFogMin(0)
		, fFogMax(1)
		, fFogDensity(0)
		, FogColor(0)
		, pRenderContext(0)
	{
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class RENDERSDK_API RenderSDKInterface
{
public:
	RenderSDKInterface(class CRenderSDK * pRenderSDK);
	virtual ~RenderSDKInterface();

	inline_ class CRenderSDK*	GetRenderComponent() const { return m_pRenderSDK; }
	void						SetRenderComponent(class CRenderSDK* RenderSDK) const;

protected:
	mutable class CRenderSDK * m_pRenderSDK;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_BASE> class RenderChunk 
: public CommandBase
{	
	typedef void (T_BASE::*RI)() const;

public:
	RI					pfn_RI;
	const T_CLASS		*TParam;

	RenderChunk(RI pfRI, const T_CLASS* Param)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)();
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_VALUE> class RenderChunk_SetViewMatrix 
: public CommandBase
{
public:
	T_CLASS	  *TParam;
	T_VALUE	   MT;

	RenderChunk_SetViewMatrix(T_CLASS *Param, const T_VALUE &T)
		: CommandBase(INDEX_NONE)
		, TParam(Param)
		, MT(T)
	{
	}

	virtual void DoExecute()
	{
		TParam->SetViewMatrix(MT.GetPtr());
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_VALUE> class RenderChunk_SetProjMatrix 
: public CommandBase
{
public:
	T_CLASS	  *TParam;
	T_VALUE	   MT;

	RenderChunk_SetProjMatrix(T_CLASS *Param, const T_VALUE &T)
		: CommandBase(INDEX_NONE) 
		, TParam(Param)
		, MT(T)
	{
	}

	virtual void DoExecute()
	{
		TParam->SetProjMatrix(MT.GetPtr());
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_VALUE> class RenderChunk_SetProjCropMatrix 
: public CommandBase
{
public:
	T_CLASS	  *TParam;
	T_VALUE	   MT;

	RenderChunk_SetProjCropMatrix(T_CLASS *Param, const T_VALUE &T)
		: CommandBase(INDEX_NONE) 
		, TParam(Param)
		, MT(T)
	{
	}

	virtual void DoExecute()
	{
		TParam->SetProjCropMatrix(MT.GetPtr());
	}
};

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
template <class T_CLASS, class T_VALUE> class RenderChunk_SetWorldMatrix 
: public CommandBase
{
public:
	T_CLASS	  *TParam;
	T_VALUE	   MT;

	RenderChunk_SetWorldMatrix(T_CLASS *Param, const T_VALUE &T)
		: CommandBase(INDEX_NONE)
		, TParam(Param)
		, MT(T)
	{
	}

	virtual void DoExecute()
	{
		TParam->SetWorldMatrix(MT.GetPtr());
	}
};

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
template <class T_CLASS, class T_VALUE> class RenderChunk_SetOpacity
: public CommandBase
{
public:
	T_CLASS	  *TParam;
	T_VALUE	   TP;

	RenderChunk_SetOpacity(T_CLASS *Param, T_VALUE& P0)
		: CommandBase(INDEX_NONE)
		, TParam(Param)
		, TP(P0)
	{
	}

	virtual void DoExecute()
	{
		TParam->SetBlendOpacity(TP);
	}
};

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
template <class T_CLASS, class T_VALUE, class T_VALUE2, class T_VALUE3> class RenderChunk_SetLightProjected
: public CommandBase
{
public:
	T_CLASS			*TParam;
	T_VALUE			TP;		// position
	T_VALUE			TD;		// direction
	T_VALUE2		TLV;	// view matrix
	T_VALUE2		TLP;	// projection matrix
	unsigned int	Color;	// light color	
	T_VALUE3		TSD;	// split distances
	T_VALUE2		*TCPM;	// crop matrices
	unsigned int	Num;
	
	RenderChunk_SetLightProjected(T_CLASS *Param,
								  const T_VALUE& P0,
								  const T_VALUE& P1,
								  const T_VALUE2& P2,
								  const T_VALUE2& P3,
								  unsigned int	color,
								  const T_VALUE3& P4,
								  const T_VALUE2*	P5,
								  unsigned int	num)
		: CommandBase(INDEX_NONE)
		, TParam(Param)
		, TP(P0)
		, TD(P1)
		, TLV(P2)
		, TLP(P3)
		, Color(color)
		, TSD(P4)
		, TCPM(NULL)
		, Num(num)
	{
		assert(Num > 0);
		TCPM = new T_VALUE2[Num];
		memcpy(TCPM, P5, sizeof(T_VALUE2) * Num);
	}

	~RenderChunk_SetLightProjected()
	{
		if (TCPM != NULL)
		{
			delete [] TCPM;
			TCPM = NULL;
		}
	}

	virtual void DoExecute()
	{
		TParam->SetLight(TP.GetPtr(), TD.GetPtr(), TLV.GetPtr(), TLP.GetPtr(), Color, TSD.GetPtr(), (float*)TCPM, Num);
	}
};

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
template <class T_CLASS, class T_VALUE> class RenderChunk_AdvanceAnim 
: public CommandBase
{
public:
	T_CLASS *TParam;
	T_VALUE  TP;

	RenderChunk_AdvanceAnim(T_CLASS *Param, const T_VALUE& P0, int MAX_CALLS = INDEX_NONE)
		: CommandBase(MAX_CALLS)
		, TParam(Param)
		, TP(P0)
	{
	}

	virtual void DoExecute()
	{
		TParam->AdvanceAnim(TP);
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_VALUE, class T_VALUE2> class RenderChunk_SetViewport
: public CommandBase
{
	T_CLASS		*TParam;
	T_VALUE		X, Y, Width, Height;
	T_VALUE2	MinZ, MaxZ;

public:
	RenderChunk_SetViewport(T_CLASS *Param, T_VALUE x, T_VALUE y, T_VALUE width, T_VALUE height, T_VALUE2 minz = 0.f, T_VALUE2 maxz = 1.f)
		: CommandBase()
		, TParam(Param)
		, X(x)
		, Y(y)
		, Width(width)
		, Height(height)
		, MinZ(minz)
		, MaxZ(maxz)
	{
	}

	virtual void DoExecute()
	{
		TParam->SetViewport(X, Y, Width, Height, MinZ, MaxZ);
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_VALUE> class RenderChunk_SetState
: public CommandBase
{	
	typedef void (T_CLASS::*RI)(T_VALUE);

public:
	RI					pfn_RI;
	T_CLASS				*TParam;
	T_VALUE				TValue;

	RenderChunk_SetState(RI pfRI, T_CLASS* Param, T_VALUE Value)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, TValue(Value)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)(TValue);
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_VALUE> class RenderChunk_SetBias
: public CommandBase
{	
	typedef void (T_CLASS::*RI)(T_VALUE, T_VALUE);
public:
	RI					pfn_RI;
	T_CLASS				*TParam;
	T_VALUE				TValue1;
	T_VALUE				TValue2;

	RenderChunk_SetBias(RI pfRI, T_CLASS* Param, T_VALUE Value1, T_VALUE Value2)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, TValue1(Value1)
		, TValue2(Value2)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)(TValue1, TValue2);
	}
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template <class T_CLASS, class T_VALUE1, class T_VALUE2> class RenderChunk_FontState
: public CommandBase
{	
	typedef void (T_CLASS::*RI)(const wchar_t*, T_VALUE2);

public:
	RI					pfn_RI;
	T_CLASS				*TParam;
	T_VALUE1			TValue1;
	T_VALUE2			TValue2;

	RenderChunk_FontState(RI pfRI, T_CLASS* Param, const T_VALUE1& Value1, T_VALUE2 Value2)
		: CommandBase(INDEX_NONE)
		, pfn_RI(pfRI)
		, TParam(Param)
		, TValue1(Value1)
		, TValue2(Value2)
	{
	}

	virtual void DoExecute()
	{
		(TParam->*pfn_RI)(TValue1.c_str(), TValue2);
	}
};