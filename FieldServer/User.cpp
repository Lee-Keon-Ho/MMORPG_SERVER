#include "User.h"
#include "PacketHandler.h"
#include "Map.h"
#include "UserManager.h"
#include <stdio.h>
#include <stdlib.h>

#define NAME_MAX 9

CUser::CUser() : m_position({ 0,0,0 }), m_endPosition({ 0,0,0 })
{
}

CUser::CUser(ACCEPT_SOCKET_INFO _socketInfo) : CSession(_socketInfo), m_position({ 0,0,0 }), m_endPosition({ 0,0,0 })
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

void CUser::SetPosition(VECTOR3 _position)
{
	m_position = _position;
}

void CUser::SetEndPosition(VECTOR3 _EndPosition)
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

void CUser::SetNumber(int _num)
{
	m_myNum = _num;
	memcpy(m_name, "User0000", sizeof("User0000"));
	m_name[4] = '0' + _num / 1000;
	m_name[5] = '0' + (_num % 1000) / 100;
	m_name[6] = '0' + (_num % 10) / 10;
	m_name[7] = '0' + _num % 10;
	m_name[8] = '\0';
	
	m_character = rand() % 3;
}

void CUser::SetState(int _state)
{
	m_state = _state;
}

void CUser::SetPrevSector()
{
	m_prevSector = (static_cast<int>(m_position.x) / 6) + (static_cast<int>(m_position.z) / 6) * 43;
}

void CUser::SetNowSector(VECTOR3 _vector)
{
	m_nowSector = (static_cast<int>(_vector.x) / 6) + (static_cast<int>(_vector.z) / 6) * 43;
}

void CUser::SetUser(VECTOR3 _position)
{
	CMap* map = CMap::GetInstance();

	m_position = _position;
	m_endPosition = _position;
	m_prevSector = (static_cast<int>(_position.x) / 6) + (static_cast<int>(_position.z) / 6) * 43;
	m_nowSector = (static_cast<int>(_position.x) / 6) + (static_cast<int>(_position.z) / 6) * 43;
	m_pSector = map->GetSector(m_prevSector);
	map->Add(this, m_prevSector);
}

void CUser::SetUser(VECTOR3 _current, VECTOR3 _end, int _state)
{
	m_position = _current;
	m_endPosition = _end;
	m_state = _state;

	if ((static_cast<int>(m_position.x) / 6) + (static_cast<int>(m_position.z) / 6) * 43 != m_prevSector)
	{
		CMap::GetInstance()->CheckSectorUpdates(this);
	}
}

void CUser::SetUser(VECTOR3 _current, VECTOR3 _end, float _rotationY, int _state)
{
	m_position = _current;
	m_endPosition = _end;
	m_rotationY = _rotationY;
	m_state = _state;
}

void CUser::SetSector()
{
	m_pSector = CMap::GetInstance()->GetSector(m_prevSector);
}

int CUser::GetNumber()
{
	return m_myNum;
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
	return m_nowSector;
}

bool CUser::checkSector()
{
	return m_prevSector == m_nowSector;
}

int CUser::GetUserCountInSector()
{	
	return m_pSector->GetSectorUserCount();
}

void CUser::SendSector(char* _buffer, int _size)
{
	m_pSector->SendAll(_buffer, _size);
}

void CUser::Infield()
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
		userCount = userList_t.size();

		if (userCount == 0) continue;

		std::map<SOCKET, CUser*>::iterator iter = userList_t.begin();
		std::map<SOCKET, CUser*>::iterator iterEnd = userList_t.end();

		while (iter != iterEnd)
		{
			if (userCount > 10) userCount = 10;

			tempBuffer = sendBuffer;

			*(u_short*)tempBuffer = 6 + (43 * userCount);
			tempBuffer += sizeof(u_short);
			*(u_short*)tempBuffer = CS_PT_INFIELD;
			tempBuffer += sizeof(u_short);
			*(u_short*)tempBuffer = userCount;
			tempBuffer += sizeof(u_short);

			for (int n = 0; n < userCount; n++, iter++)
			{
				pUser = (*iter).second;

				*(u_short*)tempBuffer = pUser->GetNumber();
				tempBuffer += sizeof(u_short);
				*(u_short*)tempBuffer = pUser->GetState();
				tempBuffer += sizeof(u_short);
				*(u_short*)tempBuffer = pUser->GetCharacter();
				tempBuffer += sizeof(u_short);
				*(float*)tempBuffer = pUser->GetRotationY();
				tempBuffer += sizeof(float);
				memcpy(tempBuffer, pUser->GetName(), 9);
				tempBuffer += 9;
				memcpy(tempBuffer, pUser->GetPosition(), sizeof(VECTOR3));
				tempBuffer += sizeof(VECTOR3);
				memcpy(tempBuffer, pUser->GetEndPosition(), sizeof(VECTOR3));
				tempBuffer += sizeof(VECTOR3);
			}

			Send(sendBuffer, tempBuffer - sendBuffer);
			count++;
			userCount = userList_t.size() - (10 * count);
		}
		count = 0;
	}
}

void CUser::LogIn()
{
	PACKET_NEW_LOGIN packet(sizeof(PACKET_NEW_LOGIN), CS_PT_LOGIN, m_myNum, m_character, 9, m_name);
	printf("%d %d\n", m_myNum, m_socket_info.socket);
	Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_NEW_LOGIN));
}

void CUser::NewUser()
{
	PACKET_NEWUSER packet(sizeof(PACKET_NEWUSER), CS_PT_NOWPOSITION, m_myNum, m_character, m_name, m_position);
	CUserManager::GetInstance()->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_NEWUSER));

	char sendBuffer[10];
	char* tempBuffer = sendBuffer;
	*(u_short*)tempBuffer = 6;
	tempBuffer += sizeof(u_short);
	*(u_short*)tempBuffer = 19;
	tempBuffer += sizeof(u_short);
	*(u_short*)tempBuffer = m_myNum;
	tempBuffer += sizeof(u_short);

	SendSector(sendBuffer, tempBuffer - sendBuffer);
	//SendSector(reinterpret_cast<char*>(&packet), sizeof(PACKET_NEWUSER));
}

void CUser::NowPosition()
{
	PACKET_NOWPOSITION packet(sizeof(PACKET_NOWPOSITION), 18, m_myNum, m_position);

	SendSector(reinterpret_cast<char*>(&packet), sizeof(PACKET_NOWPOSITION));
}

void CUser::MoveUser()
{
	PACKET_MOVEUSER packet(sizeof(PACKET_MOVEUSER), CS_PT_MOVEUSER, m_myNum, m_position, m_endPosition);

	SendSector(reinterpret_cast<char*>(&packet), sizeof(PACKET_MOVEUSER));
}

void CUser::Arrive()
{
	PACKET_ARRIVE packet(sizeof(PACKET_ARRIVE), CS_PT_ARRIVE, m_myNum, m_rotationY, m_position);

	SendSector(reinterpret_cast<char*>(&packet), sizeof(PACKET_ARRIVE));
}

const char* CUser::GetName()
{
	return m_name;
}

int CUser::GetCharacter()
{
	return m_character;
}
