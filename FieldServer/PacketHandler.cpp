#include "PacketHandler.h"
#include "PacketStruct.h"
#include "UserManager.h"
#include "MonsterManager.h"
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
	m_lpfp[4] = &CPacketHandler::NowPosition;
	m_lpfp[5] = &CPacketHandler::MoveUser;
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
		break;
	case CS_PT_NEWUSERENTRY:
		NowPosition(_user, _buffer);
		break;
	case CS_PT_MOVEUSER:
		MoveUser(_user, _buffer);
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
	_user->SetIndex(userManager->AddUserNumber());
	_user->SendPacket_LogIn();
}

void CPacketHandler::InField(CUser* _user, char* _buffer)
{
	CUserManager* userManager = CUserManager::GetInstance();

	VECTOR3 position({ 30.0f,1.0f,220.0f });

	_user->SetInfo(position);
	_user->SendPacket_Infield();

	if(userManager->GetUserCount() != 1) NewUser(_user);
	CMonsterManager::GetInstance()->CreateMonster();
}

void CPacketHandler::NewUser(CUser* _user)
{
	_user->SendPacket_NewUserEntry();
}

void CPacketHandler::NowPosition(CUser* _user, char* _buffer)
{
	int num = *(u_short*)_buffer;
	_buffer += sizeof(u_short);

	VECTOR3 position = *(VECTOR3*)_buffer;

	_user->SetPosition(position);
	_user->SetCurrentSector(position);

	//_user->NowPosition();
	CMap::GetInstance()->CheckSectorUpdates(_user);
}

void CPacketHandler::MoveUser(CUser* _user, char* _buffer)
{
	int number = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	int state = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	VECTOR3 current = *(VECTOR3*)_buffer;
	_buffer += sizeof(VECTOR3);
	VECTOR3 end = *(VECTOR3*)_buffer;
	_buffer += sizeof(VECTOR3);
	
	_user->SetInfo(current, end, state);
	_user->SendPacket_Move();
}

void CPacketHandler::Arrive(CUser* _user, char* _buffer) // stop
{
	VECTOR3 position({ 0,0,0 });
	int num = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	int state = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	float y = *(float*)_buffer;
	_buffer += sizeof(float);
	position = *(VECTOR3*)_buffer;
	_buffer += sizeof(VECTOR3);

	_user->SetInfo(position, position, y, state);
	_user->SetCurrentSector(position);

	CMap::GetInstance()->CheckSectorUpdates(_user);

	_user->SendPacket_Arrive();
}

void CPacketHandler::LogOut(CUser* _user, char* _buffer)
{
	CUserManager* pUserManager = CUserManager::GetInstance();

	PACKET_LOGOUT packet(sizeof(PACKET_LOGOUT), CS_PT_LOGOUT, _user->GetIndex());

	pUserManager->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_LOGOUT));
	pUserManager->Del(_user);
	CMap::GetInstance()->Del(_user, _user->GetNowSector());
}

void CPacketHandler::GetUserCount(CUser* _user)
{
	CUserManager::GetInstance()->SendUserCount(*_user);
}