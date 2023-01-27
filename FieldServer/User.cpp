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
	// 바로 처리
	int readSize = CPacketHandler::GetInstance()->Handle(this);

	if (readSize > 100) // readsize -1 Eerro 표시
	{
		printf("Eerro\n");
	}

	if (readSize > 0)
	{
		m_ringBuffer->Read(readSize);
		return readSize;
	}

	return 0;
}