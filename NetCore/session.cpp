#include "session.h"
#include "RingBuffer.h"
#include "Lock.h"
#include <stdio.h>
#include <iostream>
#include <exception>
#include <assert.h>
#include <thread>

CSession::CSession()
{
	
}

CSession::CSession(ACCEPT_SOCKET_INFO& _socketInfo) : 
	m_socket(_socketInfo.socket),
	m_addr(_socketInfo.addr),
	m_ringBuffer(buffer_max),
	m_heartBeatCount(0)
{
	m_overlapped_recv.session = this;
	m_overlapped_recv.ioType = static_cast<int>(eFlag::RECV);
	m_overlapped_send.session = this;
	m_overlapped_send.ioType = static_cast<int>(eFlag::SEND);

	m_dataBuf.buf = m_ringBuffer.GetWriteBuffer();
	m_dataBuf.len = m_ringBuffer.GetWriteBufferSize();


	m_pReader = std::make_unique<CReader>(buffer_max);
	assert(m_pReader != nullptr);
	m_pWriter = std::make_unique<CWriter>();
	assert(m_pWriter != nullptr);

	Recv();
}


CSession::~CSession()
{
	closesocket(m_socket);
}

bool CSession::Send(LKH::sharedPtr<PACKET> _packet, int _size) // LSend
{
	if (m_pWriter == nullptr) return false;
	m_pWriter.get()->Write(_packet);

	if (m_pWriter.get()->IsWriteLenZero())
	{
		DWORD sendSize = 0;
		DWORD err = 0;

		m_pWriter.get()->SetBuffer();

		if (WSASend(m_socket, m_pWriter.get()->GetWSABUF(), 1, &sendSize, 0, &m_overlapped_send, NULL) == SOCKET_ERROR)
		{
			err = GetLastError();
			if (err != WSAEWOULDBLOCK)
			{
				std::cout << "WSASend Error : " << err << std::endl;
			}
		}

		if(sendSize > 0);
		{
			// 수정 오류 처리
		}
	}

	return false;
}

bool CSession::Recv()
{
	DWORD recvBytes = 0;
	DWORD flags = 0;
	DWORD err;

	if (WSARecv(m_socket, m_pReader.get()->GetWSABUF(), 1, &recvBytes, &flags, &m_overlapped_recv, NULL) == SOCKET_ERROR)
	{
		if (err = WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("Error WSARecv : %d \n", err);
			return false;
		}
	}
	return true;
}

void CSession::OnRecv(DWORD _size)
{
	m_pReader.get()->ConsumeSize(_size);

	int size = PacketHandle();

	m_pReader.get()->Read(size);

	Recv();
}

void CSession::OnSend()
{
	if (m_pWriter == nullptr) return;
	m_pWriter.get()->HandleUsedBuffer();

	if (m_pWriter.get()->IsWriteBuffer()) // writeSize
	{
		DWORD sendSize = 0;
		DWORD err = 0;

		m_pWriter.get()->SetBuffer();

		WSASend(m_socket, m_pWriter.get()->GetWSABUF(), 1, &sendSize, 0, &m_overlapped_send, NULL);

		assert(sendSize > 0);
	}
	else m_pWriter.get()->SetWriteLenZero();
}

bool CSession::checkHeartBeat()
{
	++m_heartBeatCount;
	if (m_heartBeatCount <= heartBeat_max) return true;
	return false;
}

void CSession::recvHeartBeat()
{
	m_heartBeatCount = 0;
}

SOCKET CSession::GetSocket()
{
	return m_socket;
}

SOCKADDR_IN CSession::GetAddr()
{
	return m_addr;
}

char* CSession::GetPacketBuffer()
{
	return m_pReader.get()->GetBuffer();
}

int CSession::GetReadSize()
{
	return m_pReader.get()->GetReadSize();
}