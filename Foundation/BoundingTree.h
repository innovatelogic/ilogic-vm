#ifndef __boundingtree_h__
#define __boundongtree_h__

#pragma once

#include "Bounds2f.h"

enum EBoundType
{
	EBT_Occluder = 0,
	EBT_Hole,
};

struct SBoundingNode
{
	Bounds2f bound;
	
	std::list<SBoundingNode> negativeNodes;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class CBoundingTree2f
{
public:
	CBoundingTree2f(Bounds2f &rootBound)
		: m_RootBound(rootBound)
	{
	}

	//----------------------------------------------------------------------------------------------
	void PushBound(Bounds2f &bound)
	{
		// clamp bound to roots
	}

	//----------------------------------------------------------------------------------------------
	bool TestBoundOverlapped(const Bounds2f &inbound, Bounds2f &outBound)
	{

	}

private:
	std::list<SBoundingNode> m_Nodes;

	Bounds2f m_RootBound;
};



#endif//__boundingtree_h__