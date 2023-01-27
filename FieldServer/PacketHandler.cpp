#include "PacketHandler.h"
#include "UserManager.h"

CPacketHandler::CPacketHandler()
{
	
}

CPacketHandler::~CPacketHandler()
{
}

int CPacketHandler::Handle(CUser* _user) // buffer
{
	char* readBuffer = _user->GetPacketBuffer(); // 밖에서 차단이 더 좋을거 같다

	if (readBuffer == nullptr) return 0;

	USHORT size = *(USHORT*)readBuffer;
	readBuffer += sizeof(USHORT);
	USHORT type = *(USHORT*)readBuffer;
	readBuffer += sizeof(USHORT);

	switch (type)
	{
	case 0:
		test(_user, readBuffer);
		break;
	case 1:
		test1(_user, readBuffer);
		break;
	default:
		break;
	}
	/*
	* m_lpfp = test;
		함수포인터 구현

		원래는 switch 사용 했었음
	*/

	return size;
}

void CPacketHandler::test(CUser* _user, char* _buffer)
{
	DWORD time = *(DWORD*)_buffer;

	char sendBuffer[100];
	char* tempBuffer = sendBuffer;

	*(USHORT*)tempBuffer = 4 + 4 + 2;
	tempBuffer += 2;
	*(USHORT*)tempBuffer = 0;
	tempBuffer += 2;
	*(DWORD*)tempBuffer = time;
	tempBuffer += 4;
	*(USHORT*)tempBuffer = CUserManager::GetInstance()->GetUserCount();
	tempBuffer += 2;

	_user->Send(sendBuffer, tempBuffer - sendBuffer);
}

void CPacketHandler::test1(CUser* _user, char* _buffer)
{
	DWORD time = *(DWORD*)_buffer;
	_user->SetLogOut(true);
	char sendBuffer[100];
	char* tempBuffer = sendBuffer;

	*(USHORT*)tempBuffer = 4 + sizeof(DWORD);
	tempBuffer += 2;
	*(USHORT*)tempBuffer = 1;
	tempBuffer += 2;
	*(DWORD*)tempBuffer = time;
	tempBuffer += sizeof(DWORD);

	_user->Send(sendBuffer, tempBuffer - sendBuffer);
}
