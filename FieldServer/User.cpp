#include "User.h"
#include "../NetCore/RingBuffer.h"
#include "PacketHandler.h"
#include "FieldManager.h"
#include "UserManager.h"
#include <stdio.h>
#include <stdlib.h>

#define NAME_MAX 8

CUser::CUser() :
	m_position({ 0.0f,0.0f,0.0f }),
	m_endPosition({ 0.0f,0.0f,0.0f }),
	m_rotationY(0.0f),
	m_index(0),
	m_state(0),
	m_prevSector(0),
	m_currentSector(0),
	m_name(nullptr),
	m_character(0),
	m_curHp(0),
	m_maxHp(0),
	m_damageMin(0),
	m_damageMax(0),
	m_level(0),
	m_pMap(nullptr),
	m_pSector(nullptr),
	m_curExp(0.0f),
	m_maxExp(0.0f)
{
}

CUser::CUser(ACCEPT_SOCKET_INFO _socketInfo) : 
	CSession(_socketInfo), 
	m_position({ 0.0f,0.0f,0.0f }), 
	m_endPosition({ 0.0f,0.0f,0.0f }),
	m_rotationY(0.0f),
	m_index(0),
	m_state(0),
	m_prevSector(0),
	m_currentSector(0),
	m_name(nullptr),
	m_character(0),
	m_curHp(100),
	m_maxHp(100),
	m_damageMin(3),
	m_damageMax(5),
	m_level(1),
	m_pMap(nullptr),
	m_pSector(nullptr),
	m_curExp(0),
	m_maxExp(20)
{
	m_name = new char[NAME_MAX];
	memset(m_name, 0, NAME_MAX);
	send(m_socket_info.socket, m_name, 4, 0);
}

CUser::~CUser()
{
}

int CUser::PacketHandle()
{
	char* readBuffer = GetPacketBuffer();
	if (readBuffer == nullptr) return 0;

	int readSize = CPacketHandler::GetInstance()->Handle(this, readBuffer);

	if (readSize > 0)
	{
		m_ringBuffer->Read(readSize);
		return readSize;
	}
	
	return 0;
}

void CUser::AddExp(int _exp)
{
	float exp = m_curExp + _exp;

	if (exp >= m_maxExp)
	{
		m_level++;
		m_curExp = exp - m_maxExp;
		m_maxExp = m_maxExp + m_level;
		m_curHp = 100;
		// max Hp Mp 처리 추가
		SendPacket_LevelUp();
	}
	else
	{
		m_curExp += _exp;
		SendPacket_EXP();
	}
}

void CUser::SetPosition(VECTOR3& _position)
{
	m_position = _position;
}

void CUser::SetEndPosition(VECTOR3& _EndPosition)
{
	m_endPosition = _EndPosition;
}

void CUser::SetRotationY(float _rotationY)
{
	m_rotationY = _rotationY;
}

VECTOR3* CUser::GetPosition()
{
	return &m_position;
}

VECTOR3* CUser::GetEndPosition()
{
	return &m_endPosition;
}

float CUser::GetRotationY()
{
	return m_rotationY;
}

void CUser::SetIndex(int _index)
{
	m_index = _index;

	m_name[0] = 'P';
	m_name[2] = '0' + (m_socket_info.socket % 1000) / 100;;
	m_name[4] = '0' + (m_socket_info.socket % 100) / 10;
	m_name[6] = '0' + m_socket_info.socket % 10;
	m_name[8] = '\0';
	
	m_character = rand() % 3;
}

void CUser::SetPrevSector()
{
	m_prevSector = (static_cast<int>(m_position.x) / SECTOR_SIZE) + (static_cast<int>(m_position.z) / SECTOR_SIZE) * SECTOR_LINE;
}

void CUser::SetCurrentSector(VECTOR3 _vector)
{
	m_currentSector = (static_cast<int>(_vector.x) / SECTOR_SIZE) + (static_cast<int>(_vector.z) / SECTOR_SIZE) * SECTOR_LINE;
}

void CUser::SetInfo(float _rotationY, int _state)
{
	m_rotationY = _rotationY;
	m_state = _state;
}

void CUser::SetInfo(VECTOR3& _position)
{
	m_pMap = CFieldManager::GetInstance()->GetMap(FOREST_HUNT); // 나중에 맵정보를 변경하자

	m_position = _position;
	m_endPosition = _position;
	m_prevSector = (static_cast<int>(_position.x) / SECTOR_SIZE) + (static_cast<int>(_position.z) / SECTOR_SIZE) * SECTOR_LINE;
	m_currentSector = (static_cast<int>(_position.x) / SECTOR_SIZE) + (static_cast<int>(_position.z) / SECTOR_SIZE) * SECTOR_LINE;
	m_pSector = m_pMap->GetSector(m_prevSector);
	m_pMap->Add(this, m_prevSector);
}

void CUser::SetInfo(VECTOR3& _current, VECTOR3& _end, int _state)
{
	m_position = _current;
	m_endPosition = _end;
	m_state = _state;

	if ((static_cast<int>(m_position.x) / SECTOR_SIZE) + (static_cast<int>(m_position.z) / SECTOR_SIZE) * SECTOR_LINE != m_prevSector)
	{
		CheckSectorUpdates();
	}
}

void CUser::SetInfo(VECTOR3& _current, VECTOR3& _end, float _rotationY, int _state)
{
	m_position = _current;
	m_endPosition = _end;
	m_rotationY = _rotationY;
	m_state = _state;
}

void CUser::SetSector()
{
	m_pSector = m_pMap->GetSector(m_prevSector);
}

int CUser::GetIndex()
{
	return m_index;
}

int CUser::GetState()
{
	return m_state;
}

int CUser::GetPrevSector()
{
	return m_prevSector;
}

int CUser::GetNowSector()
{
	return m_currentSector;
}

void CUser::LogOut()
{
	m_pMap->Del(this, m_currentSector);
}

void CUser::CheckSectorUpdates()
{
	if (m_prevSector != m_currentSector)
	{
		m_pMap->Del(this, m_prevSector);
		m_pMap->Add(this, m_currentSector);
		m_pMap->OutSector(*this);
		m_pMap->InSector(*this);
		SetPrevSector();
		SetSector();
	}
}

int CUser::GetUserCountInSector()
{	
	return m_pSector->GetSectorUserCount();
}

void CUser::SendSector(char* _buffer, int _size)
{
	m_pSector->SendAll(_buffer, _size);
}

void CUser::SendPacket_Infield()
{
	char sendBuffer[1000];
	char* tempBuffer;
	int userCount;
	CUser* pUser;
	int count = 0;

	std::vector<CSector*> adjacentSecotor = m_pSector->GetAdjacentSector();
	
	for (int i = 0; i < adjacentSecotor.size(); i++)
	{
		std::map<SOCKET, CUser*> userList_t = adjacentSecotor[i]->GetMap();
		userCount = static_cast<int>(userList_t.size());

		if (userCount == 0) continue;

		std::map<SOCKET, CUser*>::iterator iter = userList_t.begin();
		std::map<SOCKET, CUser*>::iterator iterEnd = userList_t.end();

		while (iter != iterEnd)
		{
			if (userCount > 10) userCount = 10;

			tempBuffer = sendBuffer;

			*(u_short*)tempBuffer = 6 + (43 * userCount);
			tempBuffer += sizeof(u_short);
			*(u_short*)tempBuffer = CS_PT_PLYAER_INFIELD;
			tempBuffer += sizeof(u_short);
			*(u_short*)tempBuffer = userCount;
			tempBuffer += sizeof(u_short);

			for (int n = 0; n < userCount; n++, iter++)
			{
				pUser = (*iter).second;

				*(u_short*)tempBuffer = pUser->GetIndex();
				tempBuffer += sizeof(u_short);
				*(u_short*)tempBuffer = pUser->GetState();
				tempBuffer += sizeof(u_short);
				*(u_short*)tempBuffer = pUser->GetCharacter();
				tempBuffer += sizeof(u_short);
				*(float*)tempBuffer = pUser->GetRotationY();
				tempBuffer += sizeof(float);
				memcpy(tempBuffer, pUser->GetName(), NAME_MAX);
				tempBuffer += NAME_MAX;
				memcpy(tempBuffer, pUser->GetPosition(), sizeof(VECTOR3));
				tempBuffer += sizeof(VECTOR3);
				memcpy(tempBuffer, pUser->GetEndPosition(), sizeof(VECTOR3));
				tempBuffer += sizeof(VECTOR3);
			}

			Send(sendBuffer, static_cast<int>(tempBuffer - sendBuffer));
			count++;
			userCount = static_cast<int>(userList_t.size()) - (10 * count);
		}
		count = 0;
	}
}

void CUser::SendPacket_LogIn()
{
	PACKET_NEW_LOGIN packet(sizeof(PACKET_NEW_LOGIN), CS_PT_LOGIN, m_index, m_character, m_level, NAME_MAX, m_name);
	printf("%ld %ld\n", m_index, m_socket_info.socket);
	Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_NEW_LOGIN));
}

void CUser::SendPacket_NewUserEntry()
{
	PACKET_NEWUSERENTRY packet(sizeof(PACKET_NEWUSERENTRY), CS_PT_NEWUSERENTRY, m_index, m_character, m_name, m_position);
	CUserManager::GetInstance()->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_NEWUSERENTRY));

	SendPacket_AdjacentSector_NewUserEntry();
}

void CUser::SendPacket_AdjacentSector_NewUserEntry()
{
	PACKET_ADJACENTSECTOR_NEWUSERENTRY adjacentPacket(sizeof(PACKET_ADJACENTSECTOR_NEWUSERENTRY), CS_PT_SENDSECTOR_NEWUSERENTRY, m_index);

	SendSector(reinterpret_cast<char*>(&adjacentPacket), sizeof(PACKET_ADJACENTSECTOR_NEWUSERENTRY));
}

//void CUser::NowPosition()
//{
//	PACKET_NOWPOSITION packet(sizeof(PACKET_NOWPOSITION), 18, m_index, m_position);
//
//	SendSector(reinterpret_cast<char*>(&packet), sizeof(PACKET_NOWPOSITION));
//}

void CUser::SendPacket_Move()
{
	PACKET_MOVE_USER packet(sizeof(PACKET_MOVE_USER), CS_PT_PLYAER_MOVE, m_index, m_position, m_endPosition);

	CUserManager::GetInstance()->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_MOVE_USER));
	//SendSector(reinterpret_cast<char*>(&packet), sizeof(PACKET_MOVE_USER));
}

void CUser::SendPacket_Arrive()
{
	PACKET_ARRIVE packet(sizeof(PACKET_ARRIVE), CS_PT_PLYAER_ARRIVE, m_index, m_rotationY, m_position);

	CUserManager::GetInstance()->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_ARRIVE));
	SendSector(reinterpret_cast<char*>(&packet), sizeof(PACKET_ARRIVE));
}

void CUser::SendPacket_Idle_Attack()
{
	PACKET_IDLE_ATTACK packet(sizeof(PACKET_IDLE_ATTACK), CS_PT_IDLE_PLAYER_ATTACK, m_index, m_rotationY);

	SendSector(reinterpret_cast<char*>(&packet), sizeof(PACKET_IDLE_ATTACK));
}

void CUser::SendPacket_Move_Attack()
{
	PACKET_MOVE_ATTACK packet(sizeof(PACKET_MOVE_ATTACK), CS_PT_MOVE_PLAYER_ATTACK, m_index, m_rotationY, m_position);

	SendSector(reinterpret_cast<char*>(&packet), sizeof(PACKET_MOVE_ATTACK));
}

void CUser::SendPacket_EXP()
{
	float percent = m_curExp / m_maxExp * 100.0f;
	PACKET_EXP packet(sizeof(PACKET_EXP), CS_PT_EXP, percent);

	Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_EXP));
}

void CUser::SendPacket_LevelUp()
{
	PACKET_PLAYER_LEVEL_UP packet(sizeof(PACKET_PLAYER_LEVEL_UP), CS_PT_LEVEL_UP, m_level, m_curExp, m_maxExp);

	Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_PLAYER_LEVEL_UP));
}

void CUser::SendPacket_Hit(int _damage)
{
	m_curHp -= _damage;

	if (m_curHp <= 0)
	{

	}
	else
	{
		PACKET_PLAYER_HIT packet(sizeof(PACKET_PLAYER_HIT), CS_PT_PLAYER_HIT, m_curHp);

		Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_PLAYER_HIT));
	}
}

void CUser::SendPacket_Chatting(char* _str, int _chatLen)
{
	char sendBuffer[100];
	char* tempBuffer = sendBuffer;
	*(u_short*)tempBuffer = 8 + NAME_MAX + _chatLen;
	tempBuffer += sizeof(u_short);
	*(u_short*)tempBuffer = CS_PT_PLAYER_CHATTING;
	tempBuffer += sizeof(u_short);
	*(u_short*)tempBuffer = NAME_MAX;
	tempBuffer += sizeof(u_short);
	memcpy(tempBuffer, m_name, NAME_MAX);
	tempBuffer += NAME_MAX;
	*(u_short*)tempBuffer = _chatLen;
	tempBuffer += sizeof(u_short);
	memcpy(tempBuffer, _str, _chatLen);
	tempBuffer += _chatLen;

	m_pSector->SendAll(sendBuffer, tempBuffer - sendBuffer);
}

void CUser::SendPacket_MonsterInfo()
{
	m_pSector->SendAdjacentSectorMonsterInfo(this);
}

const char* CUser::GetName()
{
	return m_name;
}

int CUser::GetCharacter()
{
	return m_character;
}

int CUser::GetDamage()
{
	int damage = Random(m_damageMax, m_damageMin);
	return damage;
}