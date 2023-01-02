#pragma once
#include "workerThread.h"
#include <vector>
class CThreadManager
{
public:
	typedef std::vector<CWorkerThread*> thread_t;

private:
	thread_t m_thread;

public:
	CThreadManager();
	~CThreadManager();

	bool Init(int _size);
};