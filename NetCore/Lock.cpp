#include "Lock.h"

CLock::CLock(PSRWLOCK _srwLock, eLockType _type) : m_SRWLock(_srwLock), m_type(_type)
{
	switch (m_type)
	{
	case eLockType::SHARED:
		AcquireSRWLockShared(m_SRWLock);
		break;
	case eLockType::EXCLUSIVE:
		AcquireSRWLockExclusive(m_SRWLock);
		break;
	}
}

CLock::~CLock()
{
	switch (m_type)
	{
	case eLockType::SHARED:
		ReleaseSRWLockShared(m_SRWLock);
		break;
	case eLockType::EXCLUSIVE:
		ReleaseSRWLockExclusive(m_SRWLock);
		break;
	}
}