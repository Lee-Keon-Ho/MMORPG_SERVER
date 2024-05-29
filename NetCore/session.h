#pragma once
#include <WinSock2.h>
#include "LStruct.h"
#include "RingBuffer.h"
#include "sharedPtr.h"
#include "Reader.h"
#include "Writer.h"

class CSession;

constexpr int buffer_max = 1000;
constexpr int spin_count = 4000;
constexpr int heartBeat_max = 3;

enum class eFlag : int
{
	RECV = 0,
	SEND
};

struct overlapped_ex : WSAOVERLAPPED
{
	CSession* session;
	int ioType;
};

class CSession
{
protected:
	SOCKET								m_socket;
	SOCKADDR_IN							m_addr;
	
	overlapped_ex						m_overlapped_recv;
	overlapped_ex						m_overlapped_send;

	CRingBuffer							m_ringBuffer;
	WSABUF								m_dataBuf;

	std::unique_ptr<CReader>			m_pReader;
	std::unique_ptr<CWriter>			m_pWriter;
	std::unique_ptr<CRingBuffer>		m_pRingBuffer;

	int									m_heartBeatCount; // User쪽으로 넘기자
public:
	CSession();
	CSession(ACCEPT_SOCKET_INFO& _socketInfo);
	virtual ~CSession();

	bool Send(LKH::sharedPtr<PACKET> _packet, int _size);
	bool Recv();

	void OnRecv(DWORD _size);
	void OnSend();

	bool checkHeartBeat();
	void recvHeartBeat();
	virtual int PacketHandle() = 0;
	virtual void Delete() = 0;

	SOCKET GetSocket();
	SOCKADDR_IN GetAddr();
	char* GetPacketBuffer();
	int GetReadSize();
};