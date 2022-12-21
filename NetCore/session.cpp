#include "session.h"

#define BUFFER_MAX 1000

CSession::CSession()
{
	
}

CSession::CSession(SOCKET _socket)
{
	m_socket_info.socket = _socket;
	m_ringBuffer = new CRingBuffer(BUFFER_MAX);
	m_dataBuf.buf = m_ringBuffer->GetWriteBuffer();
	m_dataBuf.len = m_ringBuffer->GetWriteBufferSize();
	memset(&m_socket_info.addr, 0, sizeof(SOCKADDR_IN));
}

CSession::~CSession()
{
	if (m_ringBuffer) { delete m_ringBuffer; m_ringBuffer = nullptr; }
	closesocket(m_socket_info.socket);
}

bool CSession::Recv()
{
	DWORD recvBytes = 0;
	DWORD flags = 0;
	DWORD err;

	if (WSARecv(m_socket_info.socket, &m_dataBuf, 1, &recvBytes, &flags, &m_overlapped, NULL) == SOCKET_ERROR)
	{
		if (err = WSAGetLastError() != WSA_IO_PENDING) // trow
		{
			return false;
		}
	}
	return true;
}

bool CSession::Send(char* _buffer, int _size)
{
	int sendSize = send(m_socket_info.socket, _buffer, _size, 0);

	if (sendSize < 0) return false; // try?? 

	return true;
}

void CSession::SetAddr(SOCKADDR_IN _addr)
{
	m_socket_info.addr = _addr;
}

SOCKET CSession::GetSocket()
{
	return m_socket_info.socket;
}

CRingBuffer* CSession::GetRingBuffer()
{
	return m_ringBuffer;
}

SOCKADDR_IN CSession::GetAddr()
{
	return m_socket_info.addr;
}
