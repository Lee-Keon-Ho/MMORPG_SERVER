#pragma once
#include <WinSock2.h>

class CAcceptThread
{
protected:
	HANDLE m_threadId;
	SOCKET m_tcpSocket; // 필요없음

public:
	CAcceptThread();
	CAcceptThread(SOCKET _socket);
	~CAcceptThread();

	virtual bool Start() = 0;
	static unsigned int _stdcall AcceptFunc(void* _pArgs);
	virtual void RunLoop() = 0;
};