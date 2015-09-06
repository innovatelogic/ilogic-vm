#pragma once

#include "platform_specific.h"
#include "mathlib.h"
#include "RenderSDKInterface.h"
#include "variant_render_adj.h"
#include "variant_render_command.h"

namespace RenderSDK
{
class EXPORT RenderAdjacency
{
	//RenderAdjacency(const RenderAdjacency &that) = delete;
	//RenderAdjacency& operator=(const RenderAdjacency &that) = delete;

	struct SAdjContext
	{
		size_t nIndexAdjaency;
		size_t nIndexCommand;

		SAdjContext() : nIndexAdjaency(0), nIndexCommand(0) 
		{}
	};

public:
	// Iterator.
	class IteratorAdjacency
	{
	public:
		IteratorAdjacency( LPRTVARIANT pptr ) : pPtr(pptr) {}
		void operator++()      { pPtr++; }
		LPRTVARIANT operator*()   const { return pPtr; }
		LPRTVARIANT operator->()  const { return pPtr; }

		bool operator!= (const IteratorAdjacency& other) const { return pPtr != other.pPtr; }

	private:
		LPRTVARIANT pPtr;
	};

public:
	RenderAdjacency();
	~RenderAdjacency();

	SRTVariant_Adjacency& PushRenderQuevueAdjaency();
	
	void PopRenderQuevueAdjaency();

	SRTVariant_Adjacency& GetCurrentAdjacency();

	SRVariantRenderCommand& PushRenderCommand();

	void swapBuffer();

	const LPRTVARIANT getAdjBuffer(size_t index) const { return &m_pVariantAdjacency[index]; }

	const LPRTVARIANTCMD getActiveCmd(size_t index) const { return &m_pVariantCommands[index]; }

	int getActiveStackIndex() const { return m_ActiveStack; }

	LPRTVARIANT begin(size_t index) const;
	LPRTVARIANT end(size_t index) const;

protected:
private:
	LPRTVARIANT m_pVariantAdjacency;

	LPRTVARIANTCMD m_pVariantCommands;

	SAdjContext m_aContext[2];

	volatile int m_ActiveStack;
};
}