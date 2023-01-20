#include "session.h"
#include "IOCP.h"
#include <stdio.h>

#define BUFFER_MAX 1000

CSession::CSession()
{
	
}

CSession::CSession(ACCEPT_SOCKET_INFO _socketInfo)
{
	m_overlapped.session = this;
	m_socket_info = _socketInfo;
	m_ringBuffer = new CRingBuffer(BUFFER_MAX);
	m_dataBuf.buf = m_ringBuffer->GetWriteBuffer();
	m_dataBuf.len = m_ringBuffer->GetWriteBufferSize();
	memset(&m_socket_info.addr, 0, sizeof(SOCKADDR_IN));

	CIocp::GetInstance()->Associate(m_socket_info.socket);

	Recv();
}

CSession::~CSession()
{
	if (m_ringBuffer) { delete m_ringBuffer; m_ringBuffer = nullptr; }
	closesocket(m_socket_info.socket);
}

bool CSession::Send(char* _buffer, int _size)
{
	int sendSize = send(m_socket_info.socket, _buffer, _size, 0);

	if (sendSize < 0) return false;

	return true;
}

bool CSession::Recv()
{
	DWORD recvBytes = 0;
	DWORD flags = 0;
	DWORD err;

	if (WSARecv(m_socket_info.socket, &m_dataBuf, 1, &recvBytes, &flags, &m_overlapped, NULL) == SOCKET_ERROR)
	{
		if (err = WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("Error WSARecv : %d \n", err);
			return false;
		}
	}
	return true;
}

void CSession::RecvHandle(DWORD _size)
{
	int size = *(USHORT*)m_ringBuffer->GetWriteBuffer();

	if (_size > size)
	{
		printf("bytesTrans : %d // recvSize : %d\n", _size, size);
	}

	m_ringBuffer->Write(_size);

	PacketHandle();

	m_dataBuf.len = m_ringBuffer->GetWriteBufferSize();
	m_dataBuf.buf = m_ringBuffer->GetWriteBuffer();

	Recv();

}

SOCKET CSession::GetSocket()
{
	return m_socket_info.socket;
}

SOCKADDR_IN CSession::GetAddr()
{
	return m_socket_info.addr;
}

char* CSession::GetPacketBuffer()
{
	return m_ringBuffer->GetPacketBuffer();
}

int CSession::GetReadSize()
{
	return m_ringBuffer->GetReadSize();
}