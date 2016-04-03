#include "TArray.h"

extern "C" void* __cdecl _alloca(size_t);
#define appAlloca(size) ((size==0) ? 0 : _alloca((size+7)&~7));

namespace oes
{
    namespace common_base
    {
        void* appMemmove(void* Dest, const void* Src, size_t Count)
        {
            return memmove(Dest, Src, Count);
        }

        void appMemset(void* Dest, size_t C, size_t Count)
        {
            memset(Dest, C, Count);
        }

        //#ifndef DEFINED_appMemzero
        void appMemzero(void *Dest, size_t Count)
        {
            memset(Dest, 0, Count);
        }

        void appMemcpy(void* Dest, const void* Src, size_t Count)
        {
            memcpy(Dest, Src, Count);
        }

        void appMemswap(void* Ptr1, void* Ptr2, size_t Size)
        {
            void* Temp = appAlloca(Size);
            appMemcpy(Temp, Ptr1, Size);
            appMemcpy(Ptr1, Ptr2, Size);
            appMemcpy(Ptr2, Temp, Size);
        }

    }
}