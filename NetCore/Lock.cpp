#include "Lock.h"

CLock::CLock(CRITICAL_SECTION _cs) : m_cs(_cs)
{
	EnterCriticalSection(&m_cs);
}

CLock::~CLock()
{
	LeaveCriticalSection(&m_cs);
}