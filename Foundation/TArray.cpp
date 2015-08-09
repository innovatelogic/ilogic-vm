#include "TArray.h"

void* appMemmove( void* Dest, const void* Src, INT Count )
{
	return memmove( Dest, Src, Count );
}

void appMemset( void* Dest, INT C, INT Count )
{
	memset( Dest, C, Count );
}

//#ifndef DEFINED_appMemzero
void appMemzero( void* Dest, INT Count )
{
	memset( Dest, 0, Count );
}

void appMemcpy( void* Dest, const void* Src, INT Count )
{
	memcpy( Dest, Src, Count );
}

void appMemswap( void* Ptr1, void* Ptr2, DWORD Size )
{
	void* Temp = appAlloca(Size);
	appMemcpy( Temp, Ptr1, Size );
	appMemcpy( Ptr1, Ptr2, Size );
	appMemcpy( Ptr2, Temp, Size );
}

