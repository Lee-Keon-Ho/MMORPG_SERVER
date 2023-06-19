#include "PacketHandler.h"
#include "PacketStruct.h"
#include "UserManager.h"
#include "Map.h"
#include "PacketType.h"
#include <algorithm>
#include <iostream>

CPacketHandler::CPacketHandler()
{
	srand(time(NULL));
	m_lpfp = new tFunc[11];

	m_lpfp[0] = &CPacketHandler::Latency;
	m_lpfp[1] = &CPacketHandler::LogIn;
	m_lpfp[2] = &CPacketHandler::InField;
	m_lpfp[3] = &CPacketHandler::Dummy;
	m_lpfp[4] = &CPacketHandler::NowPosition;
	m_lpfp[5] = &CPacketHandler::MoveUser2;
	m_lpfp[6] = &CPacketHandler::Arrive;
	m_lpfp[7] = &CPacketHandler::LogOut;
	//m_lpfp[8] = &CPacketHandler::MoveSector;
}

CPacketHandler::~CPacketHandler()
{
}

int CPacketHandler::Handle(CUser* _user, char* _buffer)
{
	u_short size = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	u_short type = *(u_short*)_buffer;
	_buffer += sizeof(u_short);

	if (size <= 0) return 0;

	switch (type)
	{
	case 0:
		Latency(_user, _buffer);
		break;
	case CS_PT_LOGIN:
		LogIn(_user, _buffer);
		break;
	case CS_PT_INFIELD:
		InField(_user, _buffer);
		break;
	case CS_PT_DUMMY:
		Dummy(_user, _buffer);
		break;
	case CS_PT_NOWPOSITION:
		NowPosition(_user, _buffer);
		break;
	case CS_PT_MOVEUSER:
		MoveUser2(_user, _buffer);
		break;
	case CS_PT_ARRIVE:
		Arrive(_user, _buffer);
		break;
	case CS_PT_LOGOUT:
		LogOut(_user, _buffer);
		break;
	case CS_PT_MOVESECTOR:
		break;
	case 9:
		GetUserCount(_user);
		break;
	default:
		break;
	}
	//(this->*m_lpfp[type])(_user, _buffer);
	/*
	* m_lpfp = test;
		함수포인터 구현

		원래는 switch 사용 했었음
	*/

	return size;
}

void CPacketHandler::Latency(CUser* _user, char* _buffer)
{
	PACKET_LATENCY packet(sizeof(PACKET_LATENCY), 0, *(float*)_buffer);

	_user->Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_LATENCY));
}

void CPacketHandler::LogIn(CUser* _user, char* _buffer)
{
	CUserManager* userManager = CUserManager::GetInstance();
	userManager->Add(_user);
	_user->SetNumber(userManager->AddUserNumber());
	_user->LogIn();
}

void CPacketHandler::InField(CUser* _user, char* _buffer)
{
	CUserManager* userManager = CUserManager::GetInstance();

	VECTOR3 position = *(VECTOR3*)_buffer;
	_buffer += sizeof(VECTOR3);

	_user->SetUser(position);
	_user->Infield();

	if(userManager->GetUserCount() != 1) NewUser(_user);
}

void CPacketHandler::NewUser(CUser* _user)
{
	_user->NewUser();
}

void CPacketHandler::Dummy(CUser* _user, char* _buffer)
{
	/*CMap* pMap = CMap::GetInstance();

	VECTOR3 position;
	int num = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	int state = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	position = *(VECTOR3*)_buffer;
	_buffer += sizeof(VECTOR3);

	_user->SetUser(position, position, state);
	_user->SetNowSector(position);

	pMap->CheckSectorUpdates(_user);*/
}

void CPacketHandler::NowPosition(CUser* _user, char* _buffer)
{
	int num = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	/*int state = *(u_short*)_buffer;
	_buffer += sizeof(u_short);*/

	VECTOR3 position = *(VECTOR3*)_buffer;

	_user->SetPosition(position);
	_user->SetNowSector(position);

	CMap::GetInstance()->CheckSectorUpdates(_user);

	_user->NowPosition();
}

void CPacketHandler::MoveUser2(CUser* _user, char* _buffer)
{
	int number = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	int state = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	VECTOR3 current = *(VECTOR3*)_buffer;
	_buffer += sizeof(VECTOR3);
	VECTOR3 end = *(VECTOR3*)_buffer;
	_buffer += sizeof(VECTOR3);
	
	_user->SetUser(current, end, state);
	_user->MoveUser();
}

void CPacketHandler::Arrive(CUser* _user, char* _buffer) // stop
{
	VECTOR3 position;
	int num = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	int state = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	float y = *(float*)_buffer;
	_buffer += sizeof(float);
	position = *(VECTOR3*)_buffer;
	_buffer += sizeof(VECTOR3);

	_user->SetUser(position, position, y, state);
	_user->SetNowSector(position);

	CMap::GetInstance()->CheckSectorUpdates(_user);

	_user->Arrive();
}

void CPacketHandler::LogOut(CUser* _user, char* _buffer)
{
	CUserManager* pUserManager = CUserManager::GetInstance();

	PACKET_LOGOUT packet;

	packet.size = sizeof(PACKET_LOGOUT);
	packet.type = CS_PT_LOGOUT;
	packet.number = _user->GetNumber();

	pUserManager->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_LOGOUT));

	pUserManager->Del(_user);
	CMap::GetInstance()->Del(_user, _user->GetNowSector());
}

void CPacketHandler::GetUserCount(CUser* _user)
{
	CUserManager::GetInstance()->SendUserCount(*_user);
}