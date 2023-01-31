#pragma once
#include <WinSock2.h>

class CWorkerThread
{
private:
	HANDLE m_threadId;
public:
	CWorkerThread();
	~CWorkerThread();

	bool Start();
	static unsigned int _stdcall WorkerThreadFunc(void* _pArgs);
	void RunLoop();
};