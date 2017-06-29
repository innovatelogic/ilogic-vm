
#pragma once

#include "dxstdafx.h"

namespace oes
{
    namespace d3d
    {
        class D3DDRIVER_API Refcount
        {
        public:
            Refcount();
            virtual ~Refcount();

            long AddRef();
            long Release(bool Force = false);

            bool IsClear() { return (m_nCount == 0); }

            long GetRefCount() const { return m_nCount; }

        protected:
            virtual void DoRelease();

        public:
            volatile long m_nCount;
        };
    }
}