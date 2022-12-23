#pragma once
#include <WinSock2.h>
#include "RingBuffer.h"

struct ACCEPT_SOCKET_INFO
{
	SOCKET		socket;
	SOCKADDR_IN	addr;
};

class CSession
{
protected:
	WSABUF m_dataBuf;
	ACCEPT_SOCKET_INFO m_socket_info;

	WSAOVERLAPPED m_overlapped;
	
	CRingBuffer* m_ringBuffer;
public:
	CSession();
	CSession(SOCKET _socket);
	virtual ~CSession();

	bool Recv();
	bool Send(char* _buffer, int _size);

	int RecvHandle(DWORD _size);
	virtual int PacketHandle();

	void SetAddr(SOCKADDR_IN _addr);
	SOCKET GetSocket();
	CRingBuffer* GetRingBuffer();
	SOCKADDR_IN GetAddr();
};