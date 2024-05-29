#include <WS2tcpip.h>
#include "UserManager.h"
#include "LoginConnector.h"
#include "struct.h"
#include <iostream>

CLoginConnector::CLoginConnector(PCSTR _ip, u_short _port) : 
	m_pRingBuffer(nullptr),
	m_lock(new SRWLOCK())
{
	m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	
	try
	{
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		memset(&m_servAddr, 0, sizeof(m_servAddr));
		m_servAddr.sin_family = AF_INET;
		inet_pton(AF_INET, _ip, &m_servAddr.sin_addr);
		m_servAddr.sin_port = htons(_port);

		connect(m_socket, (SOCKADDR*)&m_servAddr, sizeof(m_servAddr));
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	if (m_pRingBuffer == nullptr) m_pRingBuffer = new CRingBuffer();
	if (m_pRingBuffer == nullptr)
	{
		printf("Create RingBuffer Error\n");
	}

	m_overlapped_send.loginConnector = this;
	m_overlapped_send.flag = static_cast<int>(eFlag::SEND);
	m_overlapped_recv.loginConnector = this;
	m_overlapped_recv.flag = static_cast<int>(eFlag::RECV);

	m_dataBuf_recv.buf = m_pRingBuffer->GetWriteBuffer();
	m_dataBuf_recv.len = m_pRingBuffer->GetWriteBufferSize();

	CreateIoCompletionPort((HANDLE)m_socket, m_iocp, (ULONG_PTR)this, 0);

	Recv();

	LKH::sharedPtr<PACKET> packet = new FS2WS_PACKET_IP_PORT(m_servAddr.sin_addr.S_un.S_addr, 30005);

	Send(packet, packet.get()->size);
}

CLoginConnector::~CLoginConnector()
{
	if (m_lock) { delete m_lock; m_lock = nullptr; }
	if (m_pRingBuffer) { delete m_pRingBuffer; m_pRingBuffer = nullptr; }
	CloseHandle(m_iocp);
}

bool CLoginConnector::Start()
{
	if (!CThread::Start()) return false;
	return true;
}

void CLoginConnector::RunLoop()
{
	HANDLE hIOCP = m_iocp;
	DWORD bytesTrans;
	overlapped_login* overlapped;
	int bin;

	int recvSize = 0;
	while (true)
	{
		if (!GetQueuedCompletionStatus(hIOCP, &bytesTrans, (PULONG_PTR)&bin, (LPOVERLAPPED*)&overlapped, INFINITE))
		{
			printf("Error : %d\n", GetLastError());

			continue;
		}

		switch (overlapped->flag)
		{
		case static_cast<int>(eFlag::RECV):
			overlapped->loginConnector->OnRecv(bytesTrans);
			break;
		case static_cast<int>(eFlag::SEND):
			overlapped->loginConnector->OnSend();
			break;
		}
	}
}

void CLoginConnector::Recv()
{
	DWORD recvBytes = 0;
	DWORD flags = 0;
	DWORD err;

	if (WSARecv(m_socket, &m_dataBuf_recv, 1, &recvBytes, &flags, &m_overlapped_recv, NULL) == SOCKET_ERROR)
	{
		if (err = WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("Error WSARecv : %d \n", err);
		}
	}
}

void CLoginConnector::Send(LKH::sharedPtr<PACKET> _packet, int _size)
{
	CLock lock(m_lock, eLockType::EXCLUSIVE);

	m_sendQue.push_back(_packet);

	if (m_dataBuf_send.len == 0)
	{
		DWORD sendSize = 0;
		DWORD err = 0;

		m_dataBuf_send.len = _packet.get()->size;
		m_dataBuf_send.buf = reinterpret_cast<char*>(m_sendQue.front().get());

		if (WSASend(m_socket, &m_dataBuf_send, 1, &sendSize, 0, &m_overlapped_send, NULL) == SOCKET_ERROR)
		{
			err = GetLastError();
			if (err != WSAEWOULDBLOCK)
			{
				std::cout << "WSASend Error : " << err << std::endl;
			}
		}

		assert(sendSize > 0);
	}
}

void CLoginConnector::OnRecv(DWORD _size)
{
	m_pRingBuffer->Write(_size);
	PacketHandle();

	m_dataBuf_recv.len = m_pRingBuffer->GetWriteBufferSize();
	m_dataBuf_recv.buf = m_pRingBuffer->GetWriteBuffer();

	Recv();
}

void CLoginConnector::OnSend()
{
	CLock lock(m_lock, eLockType::EXCLUSIVE);

	m_sendQue.pop_front();

	if (m_sendQue.size() > 0)
	{
		DWORD sendSize = 0;
		DWORD err = 0;

		m_dataBuf_send.len = m_sendQue.front().get()->size;
		m_dataBuf_send.buf = reinterpret_cast<char*>(m_sendQue.front().get());

		if (WSASend(m_socket, &m_dataBuf_send, 1, &sendSize, 0, &m_overlapped_send, NULL) == SOCKET_ERROR)
		{
			err = GetLastError();
			if (err != WSAEWOULDBLOCK)
			{
				std::cout << "WSASend Error : " << err << std::endl;
			}
		}

		assert(sendSize > 0);
	}
	else m_dataBuf_send.len = 0;
}

void CLoginConnector::PacketHandle()
{
	char* readBuffer = GetPacketBuffer();
	if (readBuffer == nullptr)
	{
		printf("readBuffer NULL\n");
		return;
	}

	LS2FS_PACKET* packet = reinterpret_cast<LS2FS_PACKET*>(readBuffer); // ¼öÁ¤

	CUserManager::GetInstance()->Add(packet->characterInfo, new CUser(packet->characterInfo));
	
	if (packet->size > 0)
	{
		m_pRingBuffer->Read(packet->size);
	}
	else
	{
		printf("readSize %d\n", packet->size);
	}
}

char* CLoginConnector::GetPacketBuffer()
{
	return m_pRingBuffer->GetPacketBuffer();
}
