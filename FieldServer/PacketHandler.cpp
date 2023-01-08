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
		함수포인터 구현

		원래는 switch 사용 했었음
	*/

	return size;
}
