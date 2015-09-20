#pragma once

#include "platform_specific.h"
#include "mathlib.h"
#include "RenderSDKInterface.h"
#include "variant_render_adj.h"
#include "variant_render_command.h"

class D3DDriver;
class SRenderContext;

namespace RenderSDK
{
class EXPORT RenderAdjacency final
{
	//RenderAdjacency(const RenderAdjacency &that) = delete;
	//RenderAdjacency& operator=(const RenderAdjacency &that) = delete;

	struct SAdjContext final
	{
		size_t nIndexAdjaency;
		size_t nIndexCommand;

		// debug counters
		size_t nIndexDot;
		size_t nIndexLine;
		size_t nIndexLineZ;
		size_t nIndexTriangle;
		size_t nIndexSphere;

		SAdjContext() 
			: nIndexAdjaency(0)
			, nIndexCommand(0) 
			, nIndexDot(0)
			, nIndexLine(0)
			, nIndexLineZ(0)
			, nIndexTriangle(0)
			, nIndexSphere(0)
		{}
	};

	// debug adjacency info
	struct SDebugInfo final
	{
		SDbgPoint		*pDots;
		SDbgPoint		*pLines;
		SDbgPoint		*pLinesNoZ;
		SDbgTriangle	*pTriangles;
		SDbgSphere		*pSpheres;

		SDebugInfo();
		~SDebugInfo();
	};

public:
	// Iterators.
	class IteratorAdjacency final
	{
	public:
		IteratorAdjacency( LPRTVARIANT pptr ) : pPtr(pptr) {}
		void operator++()				{ pPtr++; }
		LPRTVARIANT operator*()   const { return pPtr; }
		LPRTVARIANT operator->()  const { return pPtr; }

		bool operator!= (const IteratorAdjacency &other) const { return pPtr != other.pPtr; }

	private:
		LPRTVARIANT pPtr;
	};

	class IteratorCommand final
	{
	public:
		IteratorCommand( LPRTVARIANTCMD pptr ) : pPtr(pptr) {}
		void operator++()      { pPtr++; }
		LPRTVARIANTCMD operator*()   const { return pPtr; }
		LPRTVARIANTCMD operator->()  const { return pPtr; }

		bool operator!= (const IteratorCommand &other) const { return pPtr != other.pPtr; }

	private:
		LPRTVARIANTCMD pPtr;
	};

public:
	RenderAdjacency(D3DDriver *driver);
	~RenderAdjacency();

	SRTVariant_Adjacency& PushRenderQuevueAdjaency();
	
	void PopRenderQuevueAdjaency();

	SRTVariant_Adjacency& GetCurrentAdjacency();

	SRVariantRenderCommand* PushRenderCommand();

	void swapBuffer();

	LPRTVARIANT getAdjBuffer(size_t index) const;
	LPRTVARIANTCMD getActiveCmd(size_t index) const;

	int getActiveStackIndex() const { return m_ActiveStack; }

	LPRTVARIANT begin_adj(size_t index) const;
	LPRTVARIANT end_adj(size_t index) const;

	LPRTVARIANTCMD begin_cmd(size_t index) const;
	LPRTVARIANTCMD end_cmd(size_t index) const;

	void render(SRenderContext *pContext);

	// aux helpers
	void auxDrawDot(const Vector &point, unsigned int Color, bool bZEnable = true);
	void auxDrawLine(const Vector &start, const Vector &end, unsigned int Color, bool bZEnable = true);
	void auxDrawLine(const Vector2f &start, const Vector2f &end, unsigned int Color, bool bZEnable = true);
	void auxDrawSphere(Vector &pos, float rad, unsigned int color, unsigned short segments = 16);
	void auxDrawTriangle(const Vector &p0, const Vector &p1, const Vector &p2, unsigned int color = 0xffffffff);
	void auxDrawTriangle2D(const Vector2f &p0, const Vector2f &p1, const Vector2f &p2, unsigned int color = 0xffffffff);
	
protected:
	void renderAdjacency(const LPRTVARIANT adjacency);

	void renderAux(const LPRTVARIANT adjacency);

private:
	LPRTVARIANT m_pVariantAdjacency;

	LPRTVARIANTCMD m_pVariantCommands;

	SAdjContext m_aContext[2];

	SDebugInfo *m_pDbgInfo;

	volatile int m_ActiveStack;

	D3DDriver	*m_pRenderDriver;
};
} // RenderSDK