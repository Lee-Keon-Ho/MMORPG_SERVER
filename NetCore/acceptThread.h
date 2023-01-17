#pragma once
#include <WinSock2.h>

class CAcceptThread
{
protected:
	HANDLE m_threadId;

public:
	CAcceptThread();
	~CAcceptThread();

	virtual bool Start() = 0;
	static unsigned int _stdcall AcceptFunc(void* _pArgs);
	virtual void RunLoop() = 0;
};