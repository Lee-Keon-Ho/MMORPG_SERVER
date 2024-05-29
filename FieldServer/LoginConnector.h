#pragma once
#include "../NetCore/Thread.h"
#include "../NetCore/RingBuffer.h"

class CLoginConnector;

/*
struct Overlappedex : WSAOVERLAPPED
{
	CConnection* loginConnector;
	int flag;
};
*/

struct overlapped_login : WSAOVERLAPPED
{
	CLoginConnector* loginConnector;
	int flag;
};

class CLoginConnector : public CThread // 이름 변경 수정
{
private:
	SOCKET								m_socket;
	SOCKADDR_IN							m_servAddr;
	std::deque<LKH::sharedPtr<PACKET>>	m_sendQue;
	CRingBuffer*						m_pRingBuffer;
	WSABUF								m_dataBuf_recv;
	WSABUF								m_dataBuf_send;
	HANDLE								m_iocp;
	overlapped_login					m_overlapped_send;
	overlapped_login					m_overlapped_recv;
	//Overlappedex						m_ovsend;
	//Overlappedex						m_ovRecv;
	PSRWLOCK							m_lock;

public:
	CLoginConnector(PCSTR _ip, u_short _port);
	~CLoginConnector();

	bool Start();
	void RunLoop() override;

	void Recv();
	void Send(LKH::sharedPtr<PACKET> _packet, int _size);

	void OnRecv(DWORD _size);
	void OnSend();

	void PacketHandle();

private:
	char* GetPacketBuffer();
};