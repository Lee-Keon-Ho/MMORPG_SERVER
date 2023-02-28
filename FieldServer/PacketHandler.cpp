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
		LogIn(_user, readBuffer);
		break;
	case 2:
		InField(_user, readBuffer);
		break;
	case 3:
		MoveUser(_user, readBuffer);
	case 4:
		NowPosition(_user, readBuffer);
		break;
	case 5:
		MoveUser2(_user, readBuffer);
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

void CPacketHandler::LogIn(CUser* _user, char* _buffer)
{
	int UserCount = CUserManager::GetInstance()->GetUserCount();
	_user->SetNumber(UserCount);

	char sendBuffer[100];
	char* tempBuffer = sendBuffer;

	*(USHORT*)tempBuffer = 6;
	tempBuffer += 2;
	*(USHORT*)tempBuffer = 1;
	tempBuffer += 2;
	*(USHORT*)tempBuffer = UserCount;
	tempBuffer += 2;

	_user->Send(sendBuffer, tempBuffer - sendBuffer);
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
	char sendBuffer[500];
	char* tempBuffer = sendBuffer;
	int userCount = CUserManager::GetInstance()->GetUserCount();
	*(USHORT*)tempBuffer = 6 + ( 28 * (userCount - 1));
	tempBuffer += 2;
	*(USHORT*)tempBuffer = 2;
	tempBuffer += 2;
	*(USHORT*)tempBuffer = userCount;
	tempBuffer += 2;

	std::list<CUser*> userList_t = CUserManager::GetInstance()->GetUserList();

	std::list<CUser*>::iterator iter = userList_t.begin();
	std::list<CUser*>::iterator iterEnd = userList_t.end();

	if (iter == iterEnd) return;

	CUser* pUser;

	for (; iter != iterEnd; iter++)
	{
		pUser = *iter;

		if (pUser->GetNumber() != _user->GetNumber())
		{
			*(USHORT*)tempBuffer = pUser->GetNumber();
			tempBuffer += 2;
			*(USHORT*)tempBuffer = pUser->GetState();
			tempBuffer += 2;
			memcpy(tempBuffer, pUser->GetPosition(), sizeof(POSITION));
			tempBuffer += sizeof(POSITION);
			memcpy(tempBuffer, pUser->GetEndPosition(), sizeof(POSITION));
			tempBuffer += sizeof(POSITION);
		}
	}

	_user->Send(sendBuffer, tempBuffer - sendBuffer);

	if(userCount != 1) NewUser(_user);
}

void CPacketHandler::NewUser(CUser* _user)
{
	char sendBuffer[100];
	char* tempBuffer = sendBuffer;

	*(USHORT*)tempBuffer = 6;
	tempBuffer += 2;
	*(USHORT*)tempBuffer = 4;
	tempBuffer += 2;
	*(USHORT*)tempBuffer = _user->GetNumber();
	tempBuffer += 2;

	std::list<CUser*> userList_t = CUserManager::GetInstance()->GetUserList();

	std::list<CUser*>::iterator iter = userList_t.begin();
	std::list<CUser*>::iterator iterEnd = userList_t.end();

	if (iter == iterEnd) return;

	CUser* pUser;

	for (; iter != iterEnd; iter++)
	{
		pUser = *iter;

		if (pUser->GetNumber() != _user->GetNumber())
		{
			pUser->Send(sendBuffer, tempBuffer - sendBuffer);
		}
	}
}

void CPacketHandler::MoveUser(CUser* _user, char* _buffer)
{
	POSITION endPosition;
	POSITION startPosition;
	float rotationY;
	int num = *(USHORT*)_buffer;
	_buffer += sizeof(USHORT);
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
	rotationY = *(float*)_buffer;
	_buffer += sizeof(float);

	std::list<CUser*> userList_t = CUserManager::GetInstance()->GetUserList();

	std::list<CUser*>::iterator iter = userList_t.begin();
	std::list<CUser*>::iterator iterEnd = userList_t.end();

	CUser* pUser;

	char sendBuffer[100];
	char* tempBuffer = sendBuffer;

	*(USHORT*)tempBuffer = 6 + 12 + 12 + 4;
	tempBuffer += 2;
	*(USHORT*)tempBuffer = 3;
	tempBuffer += 2;
	*(USHORT*)tempBuffer = _user->GetNumber();
	tempBuffer += 2;
	*(POSITION*)tempBuffer = startPosition;
	tempBuffer += sizeof(POSITION);
	*(POSITION*)tempBuffer = endPosition;
	tempBuffer += sizeof(POSITION);
	*(float*)tempBuffer = rotationY;
	tempBuffer += 4;

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

void CPacketHandler::NowPosition(CUser* _user, char* _buffer)
{
	POSITION position;
	int num = *(USHORT*)_buffer;
	_buffer += 2;
	int state = *(USHORT*)_buffer;
	_buffer += 2;

	position.x = *(float*)_buffer;
	_buffer += sizeof(float);
	position.y = *(float*)_buffer;
	_buffer += sizeof(float);
	position.z = *(float*)_buffer;
	_buffer += sizeof(float);

	_user->SetPosition(position);
	_user->SetState(state);
}

void CPacketHandler::MoveUser2(CUser* _user, char* _buffer)
{
	int number = *(USHORT*)_buffer;
	_buffer += 2;

	char sendBuffer[200];
	char* tempBuffer = sendBuffer;

	*(USHORT*)tempBuffer = 6 + sizeof(POSITION) + sizeof(POSITION);
	tempBuffer += 2;
	*(USHORT*)tempBuffer = 5;
	tempBuffer += 2;
	*(USHORT*)tempBuffer = number;
	tempBuffer += 2;

	_user->SetState(1);

	memcpy(tempBuffer, _buffer, sizeof(POSITION));
	_user->SetPosition((*(POSITION*)_buffer));
	_buffer += sizeof(POSITION);
	tempBuffer += sizeof(POSITION);
	
	memcpy(tempBuffer, _buffer, sizeof(POSITION));
	_user->SetEndPosition((*(POSITION*)_buffer));
	_buffer += sizeof(POSITION);
	tempBuffer += sizeof(POSITION);

	std::list<CUser*> userList_t = CUserManager::GetInstance()->GetUserList();

	std::list<CUser*>::iterator iter = userList_t.begin();
	std::list<CUser*>::iterator iterEnd = userList_t.end();

	if (iter == iterEnd) return;

	CUser* pUser;

	for (; iter != iterEnd; iter++)
	{
		pUser = *iter;

		if (pUser->GetSocket() != _user->GetSocket())
		{
			pUser->Send(sendBuffer, tempBuffer - sendBuffer);
		}
	}
}