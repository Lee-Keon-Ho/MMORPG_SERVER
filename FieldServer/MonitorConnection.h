#pragma once
#include "../NetCore/session.h"
#include "../NetCore/Thread.h"

class CMoniterConnection;

struct overlapped_monitor : WSAOVERLAPPED
{
	CMoniterConnection* Connector;
	int flag;
};

class CMoniterConnection : public CThread
{
public:
	static CMoniterConnection* GetInstance()
	{
		static CMoniterConnection instance;
		return &instance;
	}

private:
	CMoniterConnection();
	~CMoniterConnection();

private:
	ACCEPT_SOCKET_INFO m_info;
	std::deque<LKH::sharedPtr<PACKET>>	m_sendQue;
	WSABUF m_dataBuf_send;
	WSABUF m_dataBuf_recv;

	PSRWLOCK m_lock;

	HANDLE m_iocp;

	overlapped_monitor m_overlapped_send;
	overlapped_monitor m_overlapped_recv;

	std::unique_ptr<CRingBuffer>	m_pRingBuffer;
public:
	bool Start();
	void RunLoop() override;

	void OnSend();
	void OnRecv(DWORD _size);

	void SetFieldSockeInfo(ACCEPT_SOCKET_INFO& _socket_info);

	void Send(LKH::sharedPtr<PACKET> _buffer, int _size);
	void Recv();

private:
	void PacketHandle();
	char* GetPacketBuffer();
};