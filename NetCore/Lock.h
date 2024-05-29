#pragma once
#include <windows.h>

enum class eLockType : u_short
{
	SHARED = 0,
	EXCLUSIVE
};

class CLock
{
private:
	PSRWLOCK m_SRWLock;
	eLockType m_type;
public:
	CLock(PSRWLOCK _srwLock, eLockType _type = eLockType::EXCLUSIVE);
	~CLock();
};