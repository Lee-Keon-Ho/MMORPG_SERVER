
#include "User.h"
#include "../NetCore/RingBuffer.h"
#include "PacketHandler.h"
#include "FieldManager.h"
#include "UserManager.h"
#include <stdio.h>
#include <stdlib.h>

#define NAME_MAX 32

CUser::CUser() :
	m_endPosition({ 0.0f,0.0f,0.0f }),
	m_rotationY(0.0f),
	m_index(0),
	m_state(0),
	m_prevSector(0),
	m_currentSector(0),
	m_field(0),
	m_pMap(nullptr),
	m_pSector(nullptr),
	m_maxExp(0.0f)
{
}

CUser::CUser(ACCEPT_SOCKET_INFO _socketInfo) :
	CSession(_socketInfo),
	m_endPosition({ 0.0f,0.0f,0.0f }),
	m_characterInfo(),
	m_rotationY(0.0f),
	m_index(0),
	m_state(0),
	m_prevSector(0),
	m_currentSector(0),
	m_field(0),
	m_pMap(nullptr),
	m_pSector(nullptr),
	m_maxExp(20)
{
	char buffer[4] = { 0, };
	Send(buffer, sizeof(buffer));
}

CUser::CUser(sCharacterInfo& _info) : m_characterInfo(_info), m_endPosition({ 0.0f,0.0f,0.0f })
{
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
	float exp = m_characterInfo.curExp + _exp;

	if (exp >= m_maxExp)
	{
		m_characterInfo.level++;
		m_characterInfo.curExp = exp - m_maxExp;
		m_maxExp = m_maxExp + m_characterInfo.level;
		m_characterInfo.curHp = 100;
		m_characterInfo.damageMax++;
		m_characterInfo.damageMin++;
		// max Hp Mp 처리 추가
		SendPacket_LevelUp();
	}
	else
	{
		m_characterInfo.curExp += _exp;
		SendPacket_EXP();
	}
}

void CUser::SetPosition(VECTOR3& _position)
{
	m_characterInfo.position = _position;
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
	return &m_characterInfo.position;
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
}

void CUser::SetPrevSector()
{
	m_prevSector = (static_cast<int>(m_characterInfo.position.x) / SECTOR_SIZE) + (static_cast<int>(m_characterInfo.position.z) / SECTOR_SIZE) * SECTOR_LINE;
}

void CUser::SetCurrentSector(VECTOR3& _vector)
{
	m_currentSector = (static_cast<int>(_vector.x) / SECTOR_SIZE) + (static_cast<int>(_vector.z) / SECTOR_SIZE) * SECTOR_LINE;
}

void CUser::SetField(int _field)
{
	m_field = _field;
}

void CUser::SetInfo(sCharacterInfo& _info)
{
	m_characterInfo = _info;
	m_endPosition = _info.position;
}

void CUser::SetInfo(float _rotationY, int _state)
{
	m_rotationY = _rotationY;
	m_state = _state;
}

void CUser::SetInfo(VECTOR3& _position)
{
	if (m_pMap != nullptr)
	{
		m_pSector->WrapUser(*this);
		m_pSector->Del(this);
	}

	m_pMap = CFieldManager::GetInstance()->GetMap(m_field); // 나중에 맵정보를 변경하자

	m_characterInfo.position = _position;
	m_endPosition = _position;
	m_prevSector = (static_cast<int>(_position.x) / SECTOR_SIZE) + (static_cast<int>(_position.z) / SECTOR_SIZE) * SECTOR_LINE;
	m_currentSector = (static_cast<int>(_position.x) / SECTOR_SIZE) + (static_cast<int>(_position.z) / SECTOR_SIZE) * SECTOR_LINE;
	m_pSector = m_pMap->GetSector(m_prevSector);
	m_pSector->Add(this);
}

void CUser::SetInfo(VECTOR3& _current, VECTOR3& _end, int _state)
{
	m_characterInfo.position = _current;
	m_endPosition = _end;
	m_state = _state;

	if ((static_cast<int>(m_characterInfo.position.x) / SECTOR_SIZE) + (static_cast<int>(m_characterInfo.position.z) / SECTOR_SIZE) * SECTOR_LINE != m_prevSector)
	{
		CheckSectorUpdates();
	}
}

void CUser::SetInfo(VECTOR3& _current, VECTOR3& _end, float _rotationY, int _state)
{
	m_characterInfo.position = _current;
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

			*(u_short*)tempBuffer = 6 + (67 * userCount);
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
	PACKET_NEW_LOGIN packet(sizeof(PACKET_NEW_LOGIN), CS_PT_LOGIN, m_index, m_characterInfo);
	printf("%ld %ld\n", m_index, m_socket_info.socket);
	Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_NEW_LOGIN));
}

void CUser::SendPacket_NextField()
{
	PACKET_NEXT_FIELD packet(sizeof(PACKET_NEXT_FIELD), CS_PT_NEXT_FIELD, m_characterInfo.position);

	Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_NEXT_FIELD));
}

void CUser::SendPacket_NewUserEntry()
{
	PACKET_NEWUSERENTRY packet(sizeof(PACKET_NEWUSERENTRY), CS_PT_NEWUSERENTRY, m_index, static_cast<u_short>(m_characterInfo.type), m_characterInfo.name, m_characterInfo.position);
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
	PACKET_MOVE_USER packet(sizeof(PACKET_MOVE_USER), CS_PT_PLYAER_MOVE, m_index, m_characterInfo.position, m_endPosition);

	/*CUserManager::GetInstance()->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_MOVE_USER));*/
	SendSector(reinterpret_cast<char*>(&packet), sizeof(PACKET_MOVE_USER));
}

void CUser::SendPacket_Arrive()
{
	PACKET_ARRIVE packet(sizeof(PACKET_ARRIVE), CS_PT_PLYAER_ARRIVE, m_index, m_rotationY, m_characterInfo.position);

	/*CUserManager::GetInstance()->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_ARRIVE));*/
	SendSector(reinterpret_cast<char*>(&packet), sizeof(PACKET_ARRIVE));
}

void CUser::SendPacket_Idle_Attack()
{
	PACKET_IDLE_ATTACK packet(sizeof(PACKET_IDLE_ATTACK), CS_PT_IDLE_PLAYER_ATTACK, m_index, m_rotationY);

	SendSector(reinterpret_cast<char*>(&packet), sizeof(PACKET_IDLE_ATTACK));
}

void CUser::SendPacket_Move_Attack()
{
	PACKET_MOVE_ATTACK packet(sizeof(PACKET_MOVE_ATTACK), CS_PT_MOVE_PLAYER_ATTACK, m_index, m_rotationY, m_characterInfo.position);

	SendSector(reinterpret_cast<char*>(&packet), sizeof(PACKET_MOVE_ATTACK));
}

void CUser::SendPacket_EXP()
{
	float percent = m_characterInfo.curExp / m_maxExp * 100.0f;
	PACKET_EXP packet(sizeof(PACKET_EXP), CS_PT_EXP, percent);

	Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_EXP));
}

void CUser::SendPacket_LevelUp()
{
	PACKET_PLAYER_LEVEL_UP packet(sizeof(PACKET_PLAYER_LEVEL_UP), CS_PT_LEVEL_UP, m_characterInfo.level, m_characterInfo.curExp, m_maxExp);

	Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_PLAYER_LEVEL_UP));
}

void CUser::SendPacket_Hit(int _damage)
{
	m_characterInfo.curHp -= _damage;

	if (m_characterInfo.curHp <= 0)
	{

	}
	else
	{
		PACKET_PLAYER_HIT packet(sizeof(PACKET_PLAYER_HIT), CS_PT_PLAYER_HIT, m_characterInfo.curHp);

		Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_PLAYER_HIT));
	}
}

void CUser::SendPacket_Chatting(char* _str, int _chatLen)
{
	char sendBuffer[100];
	char* tempBuffer = sendBuffer;
	*(u_short*)tempBuffer = 8 + 32 + _chatLen;
	tempBuffer += sizeof(u_short);
	*(u_short*)tempBuffer = CS_PT_PLAYER_CHATTING;
	tempBuffer += sizeof(u_short);
	*(u_short*)tempBuffer = 32;
	tempBuffer += sizeof(u_short);
	memcpy(tempBuffer, m_characterInfo.name, 32);
	tempBuffer += 32;
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

const wchar_t* CUser::GetName()
{
	return m_characterInfo.name;
}

int CUser::GetCharacter()
{
	return m_characterInfo.type;
}

int CUser::GetDamage()
{
	int damage = Random(m_characterInfo.damageMax, m_characterInfo.damageMin);
	return damage;
}

int CUser::GetField()
{
	return m_field;
}

sCharacterInfo& CUser::GetCharacterInfo()
{
	return m_characterInfo;
}
