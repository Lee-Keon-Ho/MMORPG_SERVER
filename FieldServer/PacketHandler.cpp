#include "PacketHandler.h"
#include "User.h"

CPacketHandler::CPacketHandler()
{
}

CPacketHandler::~CPacketHandler()
{
}

int CPacketHandler::Handle(CUser* _user)
{
	char* readBuffer = _user->GetPacketBuffer();

	if (readBuffer == nullptr) return -1;

	USHORT size = *(USHORT*)readBuffer;
	readBuffer += sizeof(USHORT);
	USHORT type = *(USHORT*)readBuffer;
	readBuffer += sizeof(USHORT);

	/*
		�Լ������� ����

		������ switch ��� �߾���
	*/

	return size;
}
