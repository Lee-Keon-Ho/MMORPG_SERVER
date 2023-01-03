#pragma once
#include <WinSock2.h>

class CAcceptThread
{
private:
	HANDLE m_threadId;
	SOCKET m_tcpSocket;

public:
	CAcceptThread();
	CAcceptThread(SOCKET _socket);
	~CAcceptThread();

	bool Start();

	static unsigned int _stdcall AcceptFunc(void* _pArgs);
	void RunLoop();
};