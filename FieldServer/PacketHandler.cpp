#include "PacketHandler.h"
#include "PacketStruct.h"
#include "UserManager.h"
#include "MonsterManager.h"
#include "FieldManager.h"
#include "PacketType.h"
#include <algorithm>
#include <iostream>

CPacketHandler::CPacketHandler()
{
	srand(time(NULL));
}

CPacketHandler::~CPacketHandler()
{
	
}

int CPacketHandler::Handle(CUser* _pUser, char* _buffer)
{
	u_short size = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	u_short type = *(u_short*)_buffer;
	_buffer += sizeof(u_short);

	if (size <= 0) return 0;

	switch (type)
	{
	case 0:
		Latency(_pUser, _buffer);
		break;
	case CS_PT_LOGIN:
		PT_C2FS_LogIn(_pUser, _buffer);
		break;
	case CS_PT_PLYAER_INFIELD:
		PT_C2FS_PlayerInField(_pUser, _buffer);
		break;
	case CS_PT_NEXT_FIELD:
		PT_C2FS_NextField(_pUser, _buffer);
		break;
	case CS_PT_NEWUSERENTRY:
		PT_C2FS_PlayerNowPosition(_pUser, _buffer);
		break;
	case CS_PT_PLYAER_MOVE:
		PT_C2FS_PlayerMove(_pUser, _buffer);
		break;
	case CS_PT_PLYAER_ARRIVE:
		PT_C2FS_PlayerArrive(_pUser, _buffer);
		break;
	case CS_PT_PLYAER_LOGOUT:
		PT_C2FS_PlayerLogOut(_pUser, _buffer);
		break;
	case CS_PT_IDLE_PLAYER_ATTACK:
		PT_C2FS_IDLE_PlayerAttack(_pUser, _buffer);
		break;
	case CS_PT_MOVE_PLAYER_ATTACK:
		PT_C2FS_MOVE_PlayerAttack(_pUser, _buffer);
		break;
	case CS_PT_HIT_TARGET_MONSTER:
		PT_C2FS_TargetMonster(_pUser, _buffer);
		break;
	case CS_PT_PLAYER_CHATTING:
		PT_C2FS_Chatting(_pUser, _buffer);
		break;
	case CS_PT_IDLE_ARCHER_ATTACK:
		PT_C2FS_IDLE_ArcherAttack(_pUser, _buffer);
		break;
	case CS_PT_MOVE_ARCHER_ATTACK:
		PT_C2FS_MOVE_ArcherAttack(_pUser, _buffer);
		break;
	case CS_PT_SINGLE_HIT_MONSTER:
		PT_C2FS_HIT_Single_monster(_pUser, _buffer);
		break;
	case 98:
		test(_pUser, _buffer);
		break;
	case 99:
		GetUserCount(_pUser);
		break;
	default:
		break;
	}

	return size;
}

void CPacketHandler::Latency(CUser* _pUser, char* _buffer)
{
	PACKET_LATENCY packet(sizeof(PACKET_LATENCY), 0, *(float*)_buffer);

	_pUser->Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_LATENCY));
}

void CPacketHandler::PT_C2FS_LogIn(CUser* _pUser, char* _buffer)
{
	CUserManager* userManager = CUserManager::GetInstance();
	int len = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	wchar_t key[30] = { 0, };
	memcpy(key, _buffer, len);

	if (userManager->Find(key, _pUser))
	{
		userManager->Add(_pUser);
		_pUser->SetIndex(userManager->AddUserNumber());
		_pUser->SendPacket_LogIn();
	}
}

void CPacketHandler::PT_C2FS_PlayerInField(CUser* _pUser, char* _buffer)
{
	CUserManager* userManager = CUserManager::GetInstance();

	_pUser->SendPacket_Infield();
	_pUser->SendPacket_MonsterInfo();

	if(_pUser->GetUserCountInSector() != 1) _pUser->SendPacket_NewUserEntry();
}

void CPacketHandler::PT_C2FS_NextField(CUser* _pUser, char* _buffer)
{
	int fieldIndex = *(u_short*)_buffer;
	
	_pUser->SetField(fieldIndex);

	if (fieldIndex == 0)
	{
		VECTOR3 position({ 73.0f,1.0f,70.0f });
		_pUser->SetInfo(position);
		_pUser->SendPacket_NextField();
	}
	else 
	{
		VECTOR3 position({ 35.0f,1.0f,215.0f });
		_pUser->SetInfo(position);
		_pUser->SendPacket_NextField();
		//_pUser->CheckSectorUpdates();
	}
}

void CPacketHandler::test(CUser* _pUser, char* _buffer)
{
	int field = *(u_short*)_buffer;

	PACKET_LOGIN packet(sizeof(PACKET_LOGIN), 98, field);// InForestHunt
	_pUser->Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_LOGIN));
}

void CPacketHandler::PT_C2FS_PlayerNowPosition(CUser* _pUser, char* _buffer)
{
	int num = *(u_short*)_buffer;
	_buffer += sizeof(u_short);

	VECTOR3 position = *(VECTOR3*)_buffer;

	_pUser->SetPosition(position);
	_pUser->SetCurrentSector(position);

	//_user->NowPosition();
	_pUser->CheckSectorUpdates();
}

void CPacketHandler::PT_C2FS_PlayerMove(CUser* _pUser, char* _buffer)
{
	int index = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	int state = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	VECTOR3 current = *(VECTOR3*)_buffer;
	_buffer += sizeof(VECTOR3);
	VECTOR3 end = *(VECTOR3*)_buffer;
	_buffer += sizeof(VECTOR3);
	
	_pUser->SetInfo(current, end, state);
	_pUser->SendPacket_Move();
}

void CPacketHandler::PT_C2FS_PlayerArrive(CUser* _pUser, char* _buffer) // stop
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

	_pUser->SetInfo(position, position, y, state);
	_pUser->SetCurrentSector(position);

	_pUser->CheckSectorUpdates();

	_pUser->SendPacket_Arrive();
}

void CPacketHandler::PT_C2FS_PlayerLogOut(CUser* _pUser, char* _buffer)
{
	CUserManager* pUserManager = CUserManager::GetInstance();

	PACKET_LOGOUT packet(sizeof(PACKET_LOGOUT), CS_PT_PLYAER_LOGOUT, _pUser->GetIndex());

	pUserManager->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_LOGOUT));
	pUserManager->Del(_pUser);
	
	_pUser->LogOut();
}

void CPacketHandler::PT_C2FS_IDLE_PlayerAttack(CUser* _pUser, char* _buffer)
{
	float rotationY = *(float*)_buffer;
	_buffer += sizeof(float);
	
	_pUser->SetInfo(rotationY, 2);

	_pUser->SendPacket_Idle_Attack();
}

void CPacketHandler::PT_C2FS_MOVE_PlayerAttack(CUser* _pUser, char* _buffer)
{
	VECTOR3 position = *(VECTOR3*)_buffer;
	_buffer += sizeof(VECTOR3);
	float y = *(float*)_buffer;
	_buffer += sizeof(float);

	_pUser->SetInfo(position, position, y, 2);

	_pUser->SendPacket_Move_Attack();
}

void CPacketHandler::PT_C2FS_TargetMonster(CUser* _pUser, char* _buffer)
{
	int count = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	int index = 0;

	CMonsterManager* mobManager = CMonsterManager::GetInstance();
	
	for (int i = 0; i < count; i++)
	{
		index = *(u_short*)_buffer;
		_buffer += sizeof(u_short);

		mobManager->Hit(_pUser, index);
	}
}

void CPacketHandler::PT_C2FS_Chatting(CUser* _pUser, char* _buffer)
{
	int len = *(u_short*)_buffer;
	_buffer += sizeof(u_short);
	char chat[100];
	memcpy(chat, _buffer, len);

	_pUser->SendPacket_Chatting(chat, len);
}

void CPacketHandler::PT_C2FS_IDLE_ArcherAttack(CUser* _pUser, char* _buffer)
{
	float rotationY = *(float*)_buffer;
	_buffer += sizeof(float);

	_pUser->SetInfo(rotationY, 2);

	_pUser->SendPacket_Idle_Attack();
}

void CPacketHandler::PT_C2FS_MOVE_ArcherAttack(CUser* _pUser, char* _buffer)
{
	VECTOR3 position = *(VECTOR3*)_buffer;
	_buffer += sizeof(VECTOR3);
	float y = *(float*)_buffer;
	_buffer += sizeof(float);

	_pUser->SetInfo(position, position, y, 2);

	_pUser->SendPacket_Move_Attack();
}

void CPacketHandler::PT_C2FS_HIT_Single_monster(CUser* _pUser, char* _buffer)
{
	int index = *(u_short*)_buffer;

	CMonsterManager::GetInstance()->Hit(_pUser, index);
}

void CPacketHandler::GetUserCount(CUser* _user)
{
	CUserManager::GetInstance()->SendUserCount(*_user);
}