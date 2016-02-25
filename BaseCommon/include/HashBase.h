#pragma once

#include "TypesBase.h"

namespace oes
{
namespace common_base
{
    //----------------------------------------------------------------------------------------------
    // Thomas Wang 32 hash
    TUInt32 hash(const TUInt32 key)
    {
        TUInt32 k = key;
        k += ~(k << 15);
        k ^= (k >> 10);
        k += (k << 3);
        k ^= (k >> 6);
        k += ~(k << 11);
        k ^= (k >> 16);

        return (TUInt32)k;
    }

    //----------------------------------------------------------------------------------------------
    TUInt32 hash(const TInt32 key)
    {
        return hash(TUInt32(key));
    }

    //----------------------------------------------------------------------------------------------
    // Thomas Wang 64 hash
    TUInt32 hash(const TUInt64 key)
    {
        TUInt64 k = key;
        k += ~(k << 32);
        k ^= (k >> 22);
        k += ~(k << 13);
        k ^= (k >> 8);
        k += (k << 3);
        k ^= (k >> 15);
        k += ~(k << 27);
        k ^= (k >> 31);
        return (TUInt32)(ES_MAX_U32 & k);
    }

    //----------------------------------------------------------------------------------------------
    // Hash function for pointers
    TUInt32 hash(const void* ptr)
    {
#if defined(ES_P64)
        return hash(TUInt64(ptr));
#else
        return hash((TUInt32)(ES_MAX_U32 & (size_t)ptr));
#endif
    }
}
}