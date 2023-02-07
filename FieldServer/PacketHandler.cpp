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
		InField(_user, readBuffer);
		break;
	case 2:
		MoveUser(_user, readBuffer);
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

void CPacketHandler::InField(CUser* _user, char* _buffer)
{
	char sendBuffer[100];
	char* tempBuffer = sendBuffer;

	int UserCount = CUserManager::GetInstance()->GetUserCount();

	if (UserCount > 1)
	{
		*(USHORT*)tempBuffer = 6;
		tempBuffer += 2;
		*(USHORT*)tempBuffer = 1;
		tempBuffer += 2;
		*(USHORT*)tempBuffer = UserCount;
		tempBuffer += 2;

		CUserManager::GetInstance()->SendAll(sendBuffer, tempBuffer - sendBuffer);
	}
}

void CPacketHandler::MoveUser(CUser* _user, char* _buffer)
{
	POSITION startPosition;
	POSITION endPosition;

	startPosition.x = *(float*)_buffer;
	_buffer += sizeof(float);
	startPosition.y = *(float*)_buffer;
	_buffer += sizeof(float);
	startPosition.z = *(float*)_buffer;
	_buffer += sizeof(float);

	endPosition.x = *(float*)_buffer;
	_buffer += sizeof(float);
	endPosition.y = *(float*)_buffer;
	_buffer += sizeof(float);
	endPosition.z = *(float*)_buffer;
	_buffer += sizeof(float);

	_user->SetPosition(startPosition);

	std::list<CUser*> userList_t = CUserManager::GetInstance()->GetUserList();

	std::list<CUser*>::iterator iter = userList_t.begin();
	std::list<CUser*>::iterator iterEnd = userList_t.end();

	CUser* pUser;

	char sendBuffer[100];
	char* tempBuffer = sendBuffer;

	*(USHORT*)tempBuffer = 4 + 24;
	tempBuffer += 2;
	*(USHORT*)tempBuffer = 2;
	tempBuffer += 2;
	*(POSITION*)tempBuffer = startPosition;
	tempBuffer += sizeof(POSITION);
	*(POSITION*)tempBuffer = endPosition;
	tempBuffer += sizeof(POSITION);

	if (iter == iterEnd) return;

	for (; iter != iterEnd; iter++)
	{
		pUser = *iter;

		if (pUser->GetSocket() != _user->GetSocket())
		{
			pUser->Send(sendBuffer, tempBuffer - sendBuffer);
		}
	}
}
