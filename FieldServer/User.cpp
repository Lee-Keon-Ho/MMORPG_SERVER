#include "User.h"
#include "PacketHandler.h"
#include <stdio.h>

CUser::CUser()
{
}

CUser::CUser(ACCEPT_SOCKET_INFO _socketInfo) : CSession(_socketInfo), m_bLogOut(false)
{
}

CUser::~CUser()
{
}

int CUser::PacketHandle()
{
	int readSize = CPacketHandler::GetInstance()->Handle(this);

	if(readSize > 0) m_ringBuffer->Read(readSize);

	return m_ringBuffer->GetReadSize();
	//return CPacketHandler::GetInstance()->Handle(this);
}