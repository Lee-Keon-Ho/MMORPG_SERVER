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
	return CPacketHandler::GetInstance()->Handle(this);
}