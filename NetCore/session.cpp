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

int CSession::RecvHandle(DWORD _size)
{
	int size;

	m_ringBuffer->Write(_size);

	while (true)
	{
		size = PacketHandle();

		if (size < 0) return size;

		m_ringBuffer->Read(size);

		if (m_ringBuffer->GetReadSize() == 0) break;
	}

	m_dataBuf.len = m_ringBuffer->GetWriteBufferSize();
	m_dataBuf.buf = m_ringBuffer->GetWriteBuffer();

	Recv();

	return 0;
}

int CSession::PacketHandle()
{
	// Player을 만들어서 
	// PacketHandle을 싱글톤을 만들어서 사용 override 할 부분
	// player에서 PacketHandle을 싱글톤으로 만들어서 사용
	return 0; 
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
