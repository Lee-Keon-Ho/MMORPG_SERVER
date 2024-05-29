#include "SpinLock.h"
#include <Windows.h>

CSpinLock::CSpinLock() : m_lock(0)
{
	/*while (true)
	{
		if (_InterlockedCompareExchange((LPLONG)&m_lock, 1, 0) == 0) break;
		Sleep(0);
	}*/
}

CSpinLock::~CSpinLock()
{
	//_InterlockedExchange(&m_lock, 0);
}

void CSpinLock::Lock()
{
	while (true)
	{
		if (_InterlockedCompareExchange((LPLONG)&m_lock, 1, 0) == 0) break;
		Sleep(0);
	}
}

void CSpinLock::unLock()
{
	_InterlockedExchange(&m_lock, 0);
}
