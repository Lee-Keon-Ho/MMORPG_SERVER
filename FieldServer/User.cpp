#include "User.h"
#include "../NetCore/RingBuffer.h"
#include "../NetCore/sharedPtr.h"
#include "PacketHandler.h"
#include "FieldManager.h"
#include "UserManager.h"
#include "PacketStruct.h"
#include "ODBCManager.h"
#include <stdio.h>
#include <stdlib.h>

CUser::CUser() :
	m_endPosition({ 0.0f,0.0f,0.0f }),
	m_rotationY(0.0f),
	m_index(0),
	m_state(0),
	m_prevSector(0),
	m_currentSector(0),
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
	m_pMap(nullptr),
	m_pSector(nullptr),
	m_maxExp(20)
{
	LKH::sharedPtr<PACKET> packet = new PACKET(sizeof(PACKET), 0);
	Send(packet, sizeof(PACKET));
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

	int readSize = CPacketHandler::GetInstance()->Handle(this, reinterpret_cast<PACKET*>(readBuffer));

	if (readSize > 0) return readSize;

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
	m_prevSector = (static_cast<int>(m_characterInfo.position.x) / sector_size) + (static_cast<int>(m_characterInfo.position.z) / sector_size) * sector_line;
}

void CUser::SetCurrentSector(VECTOR3& _vector)
{
	m_currentSector = (static_cast<int>(_vector.x) / sector_size) + (static_cast<int>(_vector.z) / sector_size) * sector_line;
}

void CUser::SetField(int _field)
{
	m_characterInfo.field = _field;
}

void CUser::SetInfo(sCharacterInfo& _info)
{
	m_characterInfo = _info;
	m_endPosition = _info.position;
	CODBCManager::GetInstance()->MAX_EXP(*this, m_maxExp);
}

void CUser::SetInfo(float _rotationY, int _state)
{
	m_rotationY = _rotationY;
	m_state = _state;
}

void CUser::SetInfo()
{
	if (m_pMap != nullptr)
	{
		m_pSector->WrapUser(*this);
		m_pSector->Del(this);
	}

	m_pMap = CFieldManager::GetInstance()->GetMap(m_characterInfo.field);

	m_prevSector = (static_cast<int>(m_characterInfo.position.x) / sector_size) + (static_cast<int>(m_characterInfo.position.z) / sector_size) * sector_line;
	m_currentSector = (static_cast<int>(m_characterInfo.position.x) / sector_size) + (static_cast<int>(m_characterInfo.position.z) / sector_size) * sector_line;
	m_pSector = m_pMap->GetSector(m_prevSector);
	m_pSector->Add(this);
}

void CUser::SetInfo(VECTOR3& _current, VECTOR3& _end, int _state)
{
	m_characterInfo.position = _current;
	m_endPosition = _end;
	m_state = _state;

	if ((static_cast<int>(m_characterInfo.position.x) / sector_size) + (static_cast<int>(m_characterInfo.position.z) / sector_size) * sector_line != m_prevSector)
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

void CUser::Delete()
{
	CUserManager::GetInstance()->Del(m_socket);
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

void CUser::SendSector(LKH::sharedPtr<PACKET> _buffer, int _size)
{
	m_pSector->SendAll(_buffer, _size);
}

void CUser::SendPacket_Infield()
{
	int userCount;
	CUser* pUser;
	int count = 0;

	std::vector<CSector*> adjacentSector = m_pSector->GetAdjacentSector();
	int size = adjacentSector.size();
	for (int i = 0; i < size; ++i)
	{
		// 2023-12-26 추가 수정필요
		std::map<SOCKET, CUser*> userList_t = adjacentSector[i]->GetUserList();
		userCount = static_cast<int>(userList_t.size());

		if (userCount == 0) continue;

		std::map<SOCKET, CUser*>::iterator iter = userList_t.begin();
		std::map<SOCKET, CUser*>::iterator iterEnd = userList_t.end();

		while (iter != iterEnd)
		{
			if (userCount > Infield_max_count) userCount = Infield_max_count;

			sInfield info[Infield_max_count];

			for (int n = 0; n < userCount && iter != iterEnd; n++, iter++)
			{
				pUser = (*iter).second;

				info[n].index = pUser->GetIndex();
				info[n].state = pUser->GetState();
				info[n].character = pUser->GetCharacter();
				info[n].rotationY = pUser->GetRotationY();
				memcpy(info[n].name, pUser->GetName(), sizeof(wchar_t) * name_max);
				memcpy(&info[n].startPosition, pUser->GetPosition(), sizeof(VECTOR3));
				memcpy(&info[n].goalPosition, pUser->GetEndPosition(), sizeof(VECTOR3));
			}

			LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_INFIELD((6 + sizeof(sInfield) * userCount), static_cast<u_short>(eFS2C_PT::PLAYER_INFIELD), userCount, info);

			Send(packet, static_cast<int>(6 + sizeof(sInfield) * userCount));
			count++;
			userCount = static_cast<int>(userList_t.size()) - (Infield_max_count * count);
		}
		count = 0;
	}
}

void CUser::SendPacket_LogIn()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_NEW_LOGIN(m_index, m_characterInfo);

	Send(packet, sizeof(FS2C_PACKET_NEW_LOGIN));
}

void CUser::SendPacket_NextField()
{
	LKH::sharedPtr<PACKET> packet = new PACKET_NEXT_FIELD(m_characterInfo.position);

	Send(packet, sizeof(PACKET_NEXT_FIELD));
}

void CUser::SendPacket_NewUserEntry()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_NEWUSERENTRY(m_index, static_cast<u_short>(m_characterInfo.type), m_characterInfo.name, m_characterInfo.position);

	CUserManager::GetInstance()->SendAll(packet, sizeof(FS2C_PACKET_NEWUSERENTRY));

	SendPacket_AdjacentSector_NewUserEntry();
}

void CUser::SendPacket_AdjacentSector_NewUserEntry()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_ADJACENTSECTOR_NEWUSERENTRY(m_index);

	SendSector(packet, sizeof(FS2C_PACKET_ADJACENTSECTOR_NEWUSERENTRY));
}

void CUser::SendPacket_Move()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_MOVE_USER(m_index, m_characterInfo.position, m_endPosition);

	SendSector(packet, sizeof(FS2C_PACKET_MOVE_USER));
}

void CUser::SendPacket_Arrive()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_ARRIVE(m_index, m_rotationY, m_characterInfo.position);

	SendSector(packet, sizeof(FS2C_PACKET_ARRIVE));
}

void CUser::SendPacket_ChannelChange()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_CHANNEL_CHANGE();
	Send(packet, packet.get()->size);
}

void CUser::SendPacket_LogOut()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_CLOSE_SOCKET();
	Send(packet, sizeof(FS2C_PACKET_CLOSE_SOCKET));
}

void CUser::SendPacket_Idle_Attack()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_IDLE_ATTACK(m_index, m_rotationY);

	SendSector(packet, sizeof(FS2C_PACKET_IDLE_ATTACK));
}

void CUser::SendPacket_Move_Attack()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_MOVE_ATTACK(m_index, m_rotationY, m_characterInfo.position);

	SendSector(packet, sizeof(FS2C_PACKET_MOVE_ATTACK));
}

void CUser::SendPacket_Idle_ArcherAttack()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_IDLE_ARCHER_ATTACK(m_index, m_rotationY);

	SendSector(packet, sizeof(FS2C_PACKET_IDLE_ARCHER_ATTACK));
}

void CUser::SendPacket_Move_ArcherAttack()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_MOVE_ARCHER_ATTACK(m_index, m_rotationY, m_characterInfo.position);

	SendSector(packet, sizeof(FS2C_PACKET_MOVE_ARCHER_ATTACK));
}

void CUser::SendPacket_EXP()
{
	float percent = m_characterInfo.curExp / m_maxExp * 100.0f;
	
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_EXP(percent);

	Send(packet, sizeof(FS2C_PACKET_EXP));
}

void CUser::SendPacket_LevelUp()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_PLAYER_LEVEL_UP(m_characterInfo.level, m_characterInfo.curExp, m_maxExp);

	Send(packet, sizeof(FS2C_PACKET_PLAYER_LEVEL_UP));
}

void CUser::SendPacket_Hit(int _damage)
{
	m_characterInfo.curHp -= _damage;

	if (m_characterInfo.curHp <= 0)
	{

	}
	else
	{
		LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_PLAYER_HIT(m_characterInfo.curHp);
		Send(packet, sizeof(FS2C_PACKET_PLAYER_HIT));
	}
}

void CUser::SendPacket_Chatting(wchar_t* _str)
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_CHATTING(m_index, m_characterInfo.name, _str);

	m_pSector->SendAll(packet, sizeof(FS2C_PACKET_CHATTING));
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
	return m_characterInfo.field;
}

sCharacterInfo& CUser::GetCharacterInfo()
{
	return m_characterInfo;
}
