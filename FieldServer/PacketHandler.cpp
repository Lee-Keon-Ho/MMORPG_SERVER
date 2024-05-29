#include "PacketHandler.h"
#include "PacketStruct.h"
#include "UserManager.h"
#include "MonsterManager.h"
#include "FieldManager.h"
#include "PacketType.h"
#include "ODBCManager.h"
#include <algorithm>
#include <iostream>

CPacketHandler::CPacketHandler()
{
	m_lpfp.reserve(static_cast<u_short>(eC2FS_PT::MAX));
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_Latency);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_LogIn);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_PlayerInField);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_NextField);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_PlayerNowPosition);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_PlayerMove);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_PlayerArrive);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_PlayerLogOut);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_IDLE_PlayerAttack);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_MOVE_PlayerAttack);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_TargetMonster);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_Chatting);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_IDLE_ArcherAttack);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_MOVE_ArcherAttack);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_HIT_Single_monster);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_HeartBeat);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_Warp);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_Dummy_NextField);
	m_lpfp.push_back(&CPacketHandler::PT_C2FS_ChannelChange);
}

CPacketHandler::~CPacketHandler()
{
	
}

int CPacketHandler::Handle(CUser* _pUser, PACKET* _packet)
{
	(this->*m_lpfp[_packet->type])(_pUser, _packet);
	assert(_packet->size > 0);

	return _packet->size;
}

void CPacketHandler::PT_C2FS_Latency(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_LATENCY* recvPacket = reinterpret_cast<C2FS_PACKET_LATENCY*>(_packet);

	LKH::sharedPtr<PACKET> sendPacket = new FS2C_PACKET_LATENCY(recvPacket->time);

	_pUser->Send(sendPacket, sizeof(FS2C_PACKET_LATENCY));
}

void CPacketHandler::PT_C2FS_LogIn(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_LOGIN* recvPacket = reinterpret_cast<C2FS_PACKET_LOGIN*>(_packet);

	CUserManager* userManager = CUserManager::GetInstance();

	if (userManager->Find(recvPacket->key, _pUser))
	{
		_pUser->SetIndex(userManager->AddUserNumber());
		_pUser->SetInfo();
		_pUser->SendPacket_LogIn();
	}
}

void CPacketHandler::PT_C2FS_PlayerInField(CUser* _pUser, PACKET* _packet)
{
	_pUser->SendPacket_Infield();
	if (_pUser->GetField() != 0)_pUser->SendPacket_MonsterInfo();

	if(_pUser->GetUserCountInSector() != 1) _pUser->SendPacket_NewUserEntry();
}

void CPacketHandler::PT_C2FS_NextField(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_NEXT_FIELD* recvPacket = reinterpret_cast<C2FS_PACKET_NEXT_FIELD*>(_packet);

	_pUser->SetField(recvPacket->fieldIndex);

	VECTOR3 position{ 0.0f, 0.0f, 0.0f };

	if (recvPacket->fieldIndex == 0) position = { 73.0f, 1.0f, 70.0f };
	else position = { 35.0f, 1.0f, 215.0f };
	_pUser->SetPosition(position);
	_pUser->SetInfo();

	_pUser->SendPacket_NextField();
}

void CPacketHandler::PT_C2FS_Warp(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_WARP* recvPacket = reinterpret_cast<C2FS_PACKET_WARP*>(_packet);

	VECTOR3 position{ 0.0f, 0.0f, 0.0f };
	if (recvPacket->field == 0) position = { 73.0f, 1.0f, 70.0f };
	else if(recvPacket->field == 1) position = { 35.0f, 1.0f, 215.0f };
	_pUser->SetPosition(position);

	CODBCManager::GetInstance()->Update_Field(*_pUser, recvPacket->field);

	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_WARP(recvPacket->field);

	_pUser->Send(packet, sizeof(FS2C_PACKET_WARP));
}

void CPacketHandler::PT_C2FS_ChannelChange(CUser* _pUser, PACKET* _packet)
{
	CUserManager* pUserManager = CUserManager::GetInstance();

	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_LOGOUT(_pUser->GetIndex());

	_pUser->LogOut();

	pUserManager->SendAll(packet, packet.get()->size);

	_pUser->SendPacket_ChannelChange();

	//pUserManager->Del(_pUser->GetSocket());
}

void CPacketHandler::PT_C2FS_Dummy_NextField(CUser* _pUser, PACKET* _packet)
{
	FS2C_PACKET_DUMMY_NEXT_FIELD* recvPacket = reinterpret_cast<FS2C_PACKET_DUMMY_NEXT_FIELD*>(_packet);

	_pUser->SetField(recvPacket->fieldIndex);

	//_pUser->SetInfo(recvPacket->position);
	_pUser->SendPacket_NextField();
}

void CPacketHandler::PT_C2FS_PlayerNowPosition(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_NOW_POSITION* recvPacket = reinterpret_cast<C2FS_PACKET_NOW_POSITION*>(_packet);

	VECTOR3 position = *(VECTOR3*)_packet;

	_pUser->SetPosition(recvPacket->position);
	_pUser->SetCurrentSector(recvPacket->position);

	_pUser->CheckSectorUpdates();
}

void CPacketHandler::PT_C2FS_PlayerMove(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_PLYAER_MOVE* recvPacket = reinterpret_cast<C2FS_PACKET_PLYAER_MOVE*>(_packet);
	_pUser->SetInfo(recvPacket->currentPosition, recvPacket->goalPosition, recvPacket->state);
	_pUser->SendPacket_Move();
}

void CPacketHandler::PT_C2FS_PlayerArrive(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_PLAYER_ARRIVE* recvPacket = reinterpret_cast<C2FS_PACKET_PLAYER_ARRIVE*>(_packet);

	_pUser->SetInfo(recvPacket->position, recvPacket->position, recvPacket->rotation_y, recvPacket->state);

	_pUser->SetCurrentSector(recvPacket->position);
	_pUser->CheckSectorUpdates();
	_pUser->SendPacket_Arrive();

	CODBCManager::GetInstance()->Update_Position(*_pUser);
}

void CPacketHandler::PT_C2FS_PlayerLogOut(CUser* _pUser, PACKET* _packet)
{
	CUserManager* pUserManager = CUserManager::GetInstance();

	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_LOGOUT(_pUser->GetIndex());

	_pUser->LogOut();

	pUserManager->SendAll(packet, sizeof(FS2C_PACKET_LOGOUT));

	_pUser->SendPacket_LogOut();
}

void CPacketHandler::PT_C2FS_IDLE_PlayerAttack(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_PLAYER_IDLE_ATTACK* recvPacket = reinterpret_cast<C2FS_PACKET_PLAYER_IDLE_ATTACK*>(_packet);
	
	_pUser->SetInfo(recvPacket->rotation_y, attack);
	_pUser->SendPacket_Idle_Attack();
}

void CPacketHandler::PT_C2FS_MOVE_PlayerAttack(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_PLAYER_MOVE_ATTACK* recvPacket = reinterpret_cast<C2FS_PACKET_PLAYER_MOVE_ATTACK*>(_packet);

	_pUser->SetInfo(recvPacket->position, recvPacket->position, recvPacket->rotation_y, attack);
	_pUser->SendPacket_Move_Attack();
}

void CPacketHandler::PT_C2FS_TargetMonster(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_TARGET_MONSTER* recvPacket = reinterpret_cast<C2FS_PACKET_TARGET_MONSTER*>(_packet);
	int count = recvPacket->count;

	CMonsterManager* mobManager = CMonsterManager::GetInstance();
	
	for (int i = 0; i < count; ++i)
	{
		mobManager->Hit(_pUser, recvPacket->indexs[i]);
	}
}

void CPacketHandler::PT_C2FS_Chatting(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_CHATTING* recvPacket = reinterpret_cast<C2FS_PACKET_CHATTING*>(_packet);

	_pUser->SendPacket_Chatting(recvPacket->str);
}

void CPacketHandler::PT_C2FS_IDLE_ArcherAttack(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_IDLE_ARCHER_ATTACK* recvPacket = reinterpret_cast<C2FS_PACKET_IDLE_ARCHER_ATTACK*>(_packet);

	_pUser->SetInfo(recvPacket->rotation_y, attack);

	_pUser->SendPacket_Idle_ArcherAttack();
}

void CPacketHandler::PT_C2FS_MOVE_ArcherAttack(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_MOVE_ARCHER_ATTACK* recvPacket = reinterpret_cast<C2FS_PACKET_MOVE_ARCHER_ATTACK*>(_packet);

	_pUser->SetInfo(recvPacket->position, recvPacket->position, recvPacket->rotation_y, attack);

	_pUser->SendPacket_Move_Attack();
}

void CPacketHandler::PT_C2FS_HIT_Single_monster(CUser* _pUser, PACKET* _packet)
{
	C2FS_PACKET_HIT_SINGLE_MONSTER* recvPacket = reinterpret_cast<C2FS_PACKET_HIT_SINGLE_MONSTER*>(_packet);

	CMonsterManager::GetInstance()->Hit(_pUser, recvPacket->index);

	CODBCManager::GetInstance()->Update_Level_and_Exp(*_pUser);
}

void CPacketHandler::PT_C2FS_HeartBeat(CUser* _pUser, PACKET* _packet)
{
	_pUser->recvHeartBeat();
}
