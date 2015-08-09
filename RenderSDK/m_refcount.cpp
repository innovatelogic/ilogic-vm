#include "m_refcount.h"

//-----------------------------------------------------------------------
m_refcount::m_refcount()
: _count(0)
{
   ;// 
}

//-----------------------------------------------------------------------
m_refcount::~m_refcount()
{
   _count = 0;
}

//-----------------------------------------------------------------------
unsigned int m_refcount::add_ref()
{
   return _count++;
}

//-----------------------------------------------------------------------
unsigned int m_refcount::release()
{
  unsigned int temp_count = _count--;

  if (temp_count==0)
  {
	  delete this;
  }
  return temp_count;
}