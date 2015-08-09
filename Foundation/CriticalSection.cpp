#include "CriticalSection.h"

//------------------------------------------------------------------------
CriticalSection::CriticalSection(void)
{
	InitializeCriticalSection(&m_criticalSection);
	m_valid = true;
}
//------------------------------------------------------------------------
CriticalSection::~CriticalSection(void)
{
	DeleteCriticalSection(&m_criticalSection);
	m_valid = false;
}
//------------------------------------------------------------------------
bool CriticalSection::isValid()
{
	return m_valid;
}
//------------------------------------------------------------------------
bool CriticalSection::enter()
{
	assert(m_valid);

	bool output = false;
	EnterCriticalSection(&m_criticalSection);
	output = true;

	return output;
}
//------------------------------------------------------------------------
bool CriticalSection::leave()
{
	assert(m_valid);

	bool output = false;

	LeaveCriticalSection(&m_criticalSection);
	output = true;

	return output;
}