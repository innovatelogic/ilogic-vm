#include "MemoryAllocator.h"

template<class T> inline T Align(const T Ptr, INT Alignment)
{
	return (T)(((DWORD)Ptr + Alignment - 1) & ~(Alignment-1));
}

//----------------------------------------------------------------------------------------------
AllocWindows::AllocWindows()
: MemInit(0)
, OsCurrent(0)
, OsPeak(0)
, UsedCurrent(0)
, UsedPeak(0)
, CurrentAllocs(0)
, TotalAllocs(0)
, MemTime(0)
{
	Initialize();
}

//----------------------------------------------------------------------------------------------
AllocWindows::~AllocWindows()
{

}

//----------------------------------------------------------------------------------------------
void AllocWindows::Initialize()
{
	assert(MemInit == 0);
	MemInit = 1;

	// page size 4096;
	OsTable.FirstPool = nullptr;
	OsTable.ExhaustedPool = nullptr;
	OsTable.BlockSize = 0;

	PoolTables[0].FirstPool = nullptr;
	PoolTables[0].ExhaustedPool = nullptr;
	PoolTables[0].BlockSize = 8;

	for (size_t Index = 1; Index < 6; ++Index)
	{
		PoolTables[Index].FirstPool = nullptr;
		PoolTables[Index].ExhaustedPool = nullptr;
		PoolTables[Index].BlockSize = (8 << (Index >> 2)) + (2 << (Index - 1));
	}

	for (size_t Index = 6; Index < POOL_COUNT; ++Index)
	{
		PoolTables[Index].FirstPool = nullptr;
		PoolTables[Index].ExhaustedPool = nullptr;
		PoolTables[Index].BlockSize = (4 +((Index+6)&3)) << (1+((Index+6)>>2));
	}

	for (size_t Index = 0; Index < POOL_MAX; ++Index)
	{
		size_t i;
		for (i = 0; PoolTables[i].BlockSize < Index; ++i);
		assert(i < POOL_COUNT);
		MemSizeToPoolTable[Index] = &PoolTables[i];
	}

	for (size_t Index = 0; Index < 256; ++Index)
	{
		PoolIndirect[Index] = nullptr;
	}
	assert(POOL_MAX-1 == PoolTables[POOL_COUNT-1].BlockSize);
}

//----------------------------------------------------------------------------------------------
void* AllocWindows::Malloc(DWORD Size)
{
	assert(Size > 0);
	assert(MemInit == 1);

	// increment stats
	CurrentAllocs++;
	TotalAllocs++;

	FreeMem* Free = nullptr;

#ifdef MB_THREADSAFE
	CS.enter();
#endif

	if (Size < POOL_MAX)
	{
		// Alloc from pool.
		PoolTable* Table = MemSizeToPoolTable[Size];
		assert(Size <= Table->BlockSize);

		PoolInfo* Pool = Table->FirstPool;
		if (!Pool)
		{
			DWORD Blocks = 65536 / Table->BlockSize;
			DWORD Bytes  = Blocks * Table->BlockSize;
			assert(Blocks >= 1);
			assert(Blocks*Table->BlockSize<=Bytes);

			Free = (FreeMem*)VirtualAlloc(nullptr, Bytes, MEM_COMMIT, PAGE_READWRITE);
			assert(Free);

			PoolInfo*& Indirect = PoolIndirect[((DWORD)Free>>24)];
			if (!Indirect){
				Indirect = CreateIndirect();
			}
			Pool = &Indirect[((DWORD)Free>>16)&255];

			Pool->Link(Table->FirstPool);
			Pool->Mem	= (BYTE*)Free;
			Pool->Bytes		= Bytes;
			Pool->OsBytes	= Align(Bytes, 4096);

			Pool->Table = Table;
			Pool->Taken = 0;
			Pool->FirstMem = Free;

			Free->Blocks  = Blocks;
			Free->Next	  = nullptr;
		}

		//pick first available block and unlink it
		Pool->Taken++;
		assert(Pool->FirstMem);
		assert(Pool->FirstMem->Blocks > 0);
		Free = (FreeMem*)((BYTE*)Pool->FirstMem + --Pool->FirstMem->Blocks * Table->BlockSize);

		if (Pool->FirstMem->Blocks == 0)
		{
			Pool->FirstMem = Pool->FirstMem->Next;
			if (!Pool->FirstMem)
			{
				// move to exhausted list
				Pool->Unlink();
				Pool->Link(Table->ExhaustedPool);
			}
		}

	}
	else
	{
		// use os for large alloc.
		int AlignedSize = Align(Size, 4096);
		Free = (FreeMem*)VirtualAlloc(nullptr, AlignedSize, MEM_COMMIT, PAGE_READWRITE);
		assert(Free);

		assert(!((size_t)Free&65535));
		PoolInfo*& Indirect = PoolIndirect[((DWORD)Free>>24)];
		if (!Indirect){
			Indirect = CreateIndirect();
		}

		// init pool
		PoolInfo* Pool = &Indirect[((DWORD)Free>>16)&255];
		Pool->Mem		= (BYTE*)Free;
		Pool->Bytes		= Size;
		Pool->OsBytes	= AlignedSize;
		Pool->Table		= &OsTable;
	}

#ifdef MB_THREADSAFE
	CS.leave();
#endif

	return Free;
}

//----------------------------------------------------------------------------------------------
void AllocWindows::Free(void* Ptr)
{
	if (!Ptr){
		return;
	}
#ifdef MB_THREADSAFE
	CS.enter();
#endif

	assert(MemInit);

	PoolInfo* Pool = &PoolIndirect[(DWORD)Ptr>>24][((DWORD)Ptr>>16)&255];
	assert(Pool->Bytes!=0);

	CurrentAllocs--;

	if (Pool->Table != &OsTable)
	{
		// If this pool was exhausted, move to available list.
		if( !Pool->FirstMem )
		{
			Pool->Unlink();
			Pool->Link( Pool->Table->FirstPool );
		}

		// Free a pooled allocation.
		FreeMem* Free		= (FreeMem *)Ptr;
		Free->Blocks		= 1;
		Free->Next			= Pool->FirstMem;
		Pool->FirstMem		= Free;

		// Free this pool.
		assert(Pool->Taken>=1);
		if (--Pool->Taken == 0)
		{
			// Free the OS memory.
			Pool->Unlink();
			VirtualFree(Pool->Mem, 0, MEM_RELEASE);
			OsCurrent -= Pool->OsBytes;
		}
	}
	else
	{
		// Free an OS allocation.
		assert(!((INT)Ptr&65535));
		UsedCurrent -= Pool->Bytes;
		OsCurrent   -= Pool->OsBytes;
		VirtualFree(Ptr, 0, MEM_RELEASE);
	}
#ifdef MB_THREADSAFE
	CS.leave();
#endif
}
//----------------------------------------------------------------------------------------------
PoolInfo* AllocWindows::CreateIndirect()
{
	PoolInfo* Indirect = (PoolInfo*)VirtualAlloc(nullptr, 256*sizeof(PoolInfo), MEM_COMMIT, PAGE_READWRITE);
	assert(Indirect);
	return Indirect;
}