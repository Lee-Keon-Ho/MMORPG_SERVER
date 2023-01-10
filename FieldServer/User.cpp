#include "User.h"
#include "PacketHandler.h"
#include <stdio.h>

CUser::CUser()
{
}

CUser::CUser(ACCEPT_SOCKET_INFO _socketInfo) : CSession(_socketInfo)
{
}

CUser::~CUser()
{
}

int CUser::PacketHandle()
{
	int readSize = CPacketHandler::GetInstance()->Handle(this);

	m_ringBuffer->Read(readSize);

	return m_ringBuffer->GetReadSize();
	//return CPacketHandler::GetInstance()->Handle(this);
}