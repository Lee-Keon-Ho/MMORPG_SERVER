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
	
	// overlapped_ex m_overlapped;

	CRingBuffer* m_ringBuffer;
public:
	CSession();
	CSession(ACCEPT_SOCKET_INFO _socketInfo);
	virtual ~CSession();

	bool Send(char* _buffer, int _size);
	int RecvHandle(DWORD _size);

	virtual bool Recv() = 0; // 가상함수로 만들 필요없이 session
	virtual int PacketHandle() = 0;

	SOCKET GetSocket();
	SOCKADDR_IN GetAddr();
	char* GetPacketBuffer();
	int GetReadSize();
};