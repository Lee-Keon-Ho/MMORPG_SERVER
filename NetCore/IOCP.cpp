#include "IOCP.h"

CIocp::CIocp()
{
	m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
}

CIocp::~CIocp()
{
	CloseHandle(m_iocp);
}

HANDLE CIocp::Associate(SOCKET _socket)
{
	return CreateIoCompletionPort((HANDLE)_socket, m_iocp, NULL, 0);
}