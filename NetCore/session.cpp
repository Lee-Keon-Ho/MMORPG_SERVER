#include "session.h"

#define BUFFER_MAX 1000

CSession::CSession()
{
	
}

CSession::CSession(ACCEPT_SOCKET_INFO _socketInfo)
{
	m_socket_info = _socketInfo;
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

bool CSession::Send(char* _buffer, int _size)
{
	int sendSize = send(m_socket_info.socket, _buffer, _size, 0);

	if (sendSize < 0) return false;

	return true;
}

int CSession::RecvHandle(DWORD _size)
{
	int size;

	m_ringBuffer->Write(_size);

	//while (true)
	//{
	//	// 사라지고 바로 Recv 다시 걸고

	//	//size = PacketHandle();

	//	//if (size < 0) return size;

	//	//m_ringBuffer->Read(size);

	//	if (m_ringBuffer->GetReadSize() == 0) break;
	//}

	m_dataBuf.len = m_ringBuffer->GetWriteBufferSize();
	m_dataBuf.buf = m_ringBuffer->GetWriteBuffer();

	Recv();

	return 0;
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
