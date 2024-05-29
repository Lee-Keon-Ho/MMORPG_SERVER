#pragma once

class CSpinLock
{
private:
	volatile long m_lock;
public:
	CSpinLock();
	~CSpinLock();

	void Lock();
	void unLock();
};
