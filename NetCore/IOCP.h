#pragma once
#include <WinSock2.h>

class CIocp
{
public:
	static CIocp* GetInstance()
	{
		static CIocp  instance;
		return &instance;
	}

private:
	CIocp();
	~CIocp();

private:
	HANDLE m_iocp;

public:
	HANDLE Associate(SOCKET _socket);
	HANDLE GetHandle() { return m_iocp; }
};