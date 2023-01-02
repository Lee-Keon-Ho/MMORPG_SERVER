#include "CUser.h"
#include "PacketHandler.h"

CUser::CUser()
{
}

CUser::CUser(ACCEPT_SOCKET_INFO _socketInfo) : CSession(_socketInfo)
{
	m_overlapped.session = this;
}

CUser::~CUser()
{
}

bool CUser::Recv()
{
	DWORD recvBytes = 0;
	DWORD flags = 0;
	DWORD err;

	if (WSARecv(m_socket_info.socket, &m_dataBuf, 1, &recvBytes, &flags, &m_overlapped, NULL) == SOCKET_ERROR)
	{
		if (err = WSAGetLastError() != WSA_IO_PENDING)
		{
			//printf("Error WSARecv : %d \n", err);
			return false;
		}
	}
	return true;
}

int CUser::PacketHandle()
{
	return CPacketHandler::GetInstance()->Handle(this);
}
