#ifndef __memoryallocator_h__
#define __memoryallocator_h__

#ifdef WIN32
#pragma once
#endif

#include "platform_specific.h"
#include "CriticalSection.h"

#define MB_THREADSAFE

struct PoolInfo
{
	DWORD				Bytes;
	DWORD				OsBytes;
	DWORD				Taken;
	BYTE*				Mem;
	struct PoolTable*	Table;
	struct FreeMem*		FirstMem;
	PoolInfo*			Next;
	PoolInfo**			PrevLink;

	void Link(PoolInfo*& Before)
	{
		if (Before){
			Before->PrevLink = &Next;
		}
		Next = Before;
		PrevLink = &Before;
		Before = this;
	}

	void Unlink()
	{
		if (Next){
			Next->PrevLink = PrevLink;
		}
		*PrevLink = Next;
	}
};

struct FreeMem
{
	FreeMem*	Next;
	DWORD		Blocks;
};

struct PoolTable
{
	PoolInfo* FirstPool;
	PoolInfo* ExhaustedPool;
	DWORD	  BlockSize;
};

class EXPORT AllocWindows
{
public:
	AllocWindows();
	~AllocWindows();

	void	Initialize();
	void*	Malloc(DWORD Size);
	void	Free(void* Ptr);

	PoolInfo* CreateIndirect();

private:
	enum {POOL_COUNT	= 43	  };
	enum {POOL_MAX		= 32768+1 };

	PoolTable	PoolTables[POOL_COUNT];
	PoolTable	OsTable;
	PoolInfo*	PoolIndirect[256];
	PoolTable*	MemSizeToPoolTable[POOL_MAX];
	INT			MemInit;
	INT			OsCurrent, OsPeak, UsedCurrent, UsedPeak, CurrentAllocs, TotalAllocs;
	DOUBLE		MemTime;

#ifdef MB_THREADSAFE
	CriticalSection CS;
#endif
};

#endif//__memoryallocator_h__
