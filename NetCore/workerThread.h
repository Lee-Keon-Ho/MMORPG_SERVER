#pragma once
#include <WinSock2.h>

#define ALL 0
#define FAST 1

class CWorkerThread
{
private:
	HANDLE m_threadId;

	int m_type;
public:
	CWorkerThread();
	~CWorkerThread();

	bool Start(int _type);
	static unsigned int _stdcall ThreadFunc(void* _pArgs);
	void RunLoop();

	int GetType();
};