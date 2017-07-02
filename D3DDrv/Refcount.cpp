#include "Refcount.h"
#include "dxstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        Refcount::Refcount()
            : m_nCount(1)
        {}

        //----------------------------------------------------------------------------------------------
        Refcount::~Refcount()
        {
            m_nCount = 0;
        }

        //----------------------------------------------------------------------------------------------
        long Refcount::AddRef()
        {
            return ::InterlockedExchangeAdd(&m_nCount, 1);
        }

        //----------------------------------------------------------------------------------------------
        long Refcount::Release(bool Force /*= false*/)
        {
            ::InterlockedExchangeAdd(&m_nCount, -1);

            if (Force || m_nCount == 0)
            {
                DoRelease();
                return 0;
            }
            return m_nCount;
        }

        //----------------------------------------------------------------------------------------------
        void Refcount::DoRelease()
        {
            m_nCount = 0;
            delete this;
        }
    }
}