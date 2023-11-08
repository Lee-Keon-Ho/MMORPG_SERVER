#include "Sector.h"
#include <algorithm>
#include <iostream>

CSector::CSector() : nSector(0)
{
	InitializeCriticalSection(&m_cs_user);
	InitializeCriticalSection(&m_cs_monster);
}

CSector::CSector(int _x, int _y)
{
	nSector = _y * 15 + _x;
	InitializeCriticalSection(&m_cs_user);
	InitializeCriticalSection(&m_cs_monster);
}

CSector::~CSector()
{
	DeleteCriticalSection(&m_cs_user);
	DeleteCriticalSection(&m_cs_monster);
}

void CSector::SetAdjacentSector(CSector* _pSector)
{
	m_AdjacentSector.push_back(_pSector);
}

std::vector<CSector*> CSector::Difference(sector_t _b)
{
	vector<CSector*> result;
	vector<CSector*> a = m_AdjacentSector;

	sort(a.begin(), a.end());
	sort(_b.begin(), _b.end());
	set_difference(a.begin(), a.end(), _b.begin(), _b.end(), std::back_inserter(result));

	return result;
}

void CSector::WrapUser(CUser& _pUser)
{
	for (auto s : m_AdjacentSector)
	{
		s->WrapDeleteUser(_pUser);
	}
}

void CSector::WrapDeleteUser(CUser& _pUser)
{
	CLock lcok(m_cs_user);

	char buffer[10];
	char* tempBuffer = buffer;

	*(u_short*)tempBuffer = 6;
	tempBuffer += sizeof(u_short);
	*(u_short*)tempBuffer = 97;
	tempBuffer += sizeof(u_short);
	*(u_short*)tempBuffer = _pUser.GetIndex();
	tempBuffer += sizeof(u_short);

	for (auto u : m_userList)
	{
		u.second->Send(buffer, tempBuffer - buffer);
	}
}

void CSector::DeleteUsersOutOfSector(CUser& _user)
{
	EnterCriticalSection(&m_cs_user);
	userMap_t userList = m_userList;
	LeaveCriticalSection(&m_cs_user);
	
	int size = static_cast<int>(userList.size());
	
	if (size <= 0) return;

	char sendBuffer[100];
	char* tempBuffer = sendBuffer;

	map<SOCKET, CUser*>::iterator iter = userList.begin();
	map<SOCKET, CUser*>::iterator iterEnd = userList.end();

	CUser* pUser;
	int count = 0;
	while (iter != iterEnd)
	{
		if (size > 10) size = 10;

		tempBuffer = sendBuffer;

		*(u_short*)tempBuffer = 6 + (2 * size);
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = CS_PT_DELETE_USER_OUT_OF_SECTOR;
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = size;
		tempBuffer += sizeof(u_short);

		for (int i = 0; i < size; i++, iter++)
		{
			pUser = (*iter).second;

			*(u_short*)tempBuffer = pUser->GetIndex();
			tempBuffer += sizeof(u_short);
		}
		_user.Send(sendBuffer, static_cast<int>(tempBuffer - sendBuffer));

		count++;
		size = static_cast<int>(userList.size()) - (10 * count);
	}
}

void CSector::FetchUserInfoInNewSector(CUser& _user)
{
	EnterCriticalSection(&m_cs_user);
	map<SOCKET, CUser*> userList = m_userList;
	LeaveCriticalSection(&m_cs_user);

	int size = static_cast<int>(userList.size());

	if (size <= 0) return;

	char sendBuffer[1000];
	char* tempBuffer;

	map<SOCKET, CUser*>::iterator iter = userList.begin();
	map<SOCKET, CUser*>::iterator iterEnd = userList.end();

	CUser* pUser;
	int count = 0;

	while (iter != iterEnd)
	{
		if (size > 10) size = 10;

		tempBuffer = sendBuffer;

		*(u_short*)tempBuffer = 6 + (32 * size);
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = CS_PT_NEWSECTOR_USER;
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = size;
		tempBuffer += sizeof(u_short);

		for (int i = 0; i < size; i++, iter++)
		{
			pUser = (*iter).second;

			*(u_short*)tempBuffer = pUser->GetIndex();
			tempBuffer += sizeof(u_short);
			*(u_short*)tempBuffer = pUser->GetState();
			tempBuffer += sizeof(u_short);
			*(float*)tempBuffer = pUser->GetRotationY();
			tempBuffer += sizeof(float);
			memcpy(tempBuffer, pUser->GetPosition(), sizeof(VECTOR3));
			tempBuffer += sizeof(VECTOR3);
			memcpy(tempBuffer, pUser->GetEndPosition(), sizeof(VECTOR3));
			tempBuffer += sizeof(VECTOR3);
		}
		_user.Send(sendBuffer, static_cast<int>(tempBuffer - sendBuffer));

		count++;
		size = static_cast<int>(userList.size()) - (10 * count);
	}
}

void CSector::DeleteMonstersOutOfSector(CUser& _user)
{
	EnterCriticalSection(&m_cs_monster);
	monster_t monsterList = m_monsterList;
	LeaveCriticalSection(&m_cs_monster);

	int size = static_cast<int>(monsterList.size());

	if (size <= 0) return;

	char sendBuffer[1000];
	char* tempBuffer;

	monster_t::iterator iter = monsterList.begin();
	monster_t::iterator iterEnd = monsterList.end();

	CMonster* pMonster;
	int count = 0;
	
	while (iter != iterEnd)
	{
		if (size > 10) size = 10;

		tempBuffer = sendBuffer;

		*(u_short*)tempBuffer = 6 + (2 * size);
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = CS_PT_INSECTOR_MONSTER;
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = size;
		tempBuffer += sizeof(u_short);

		for (int i = 0; i < size; i++, iter++)
		{
			pMonster = (*iter).second;

			*(u_short*)tempBuffer = pMonster->GetIndex();
			tempBuffer += sizeof(u_short);
		}
		_user.Send(sendBuffer, static_cast<int>(tempBuffer - sendBuffer));

		count++;
		size = static_cast<int>(monsterList.size()) - (10 * count);
	}
}

void CSector::FetchMonsterInfoInNewSector(CUser& _user)
{
	EnterCriticalSection(&m_cs_monster);
	monster_t monsterList = m_monsterList;
	LeaveCriticalSection(&m_cs_monster);

	int size = static_cast<int>(monsterList.size());

	if (size <= 0) return;

	char sendBuffer[1000];
	char* tempBuffer;

	monster_t::iterator iter = monsterList.begin();
	monster_t::iterator iterEnd = monsterList.end();


	CMonster* pMonster;
	int count = 0;
	
	while (iter != iterEnd)
	{
		if (size > 10) size = 10;

		tempBuffer = sendBuffer;

		*(u_short*)tempBuffer = 6 + (28 * size);
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = CS_PT_OUTSECTOR_MONSTER;
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = size;
		tempBuffer += sizeof(u_short);

		for (int i = 0; i < size; i++, iter++)
		{
			pMonster = (*iter).second;

			*(u_short*)tempBuffer = pMonster->GetIndex();
			tempBuffer += sizeof(u_short);
			*(u_short*)tempBuffer = pMonster->GetType();
			tempBuffer += sizeof(u_short);
			memcpy(tempBuffer, pMonster->GetPosition(), sizeof(VECTOR3));
			tempBuffer += sizeof(VECTOR3);
			memcpy(tempBuffer, pMonster->GetDestinationPosition(), sizeof(VECTOR3));
			tempBuffer += sizeof(VECTOR3);
		}
		_user.Send(sendBuffer, static_cast<int>(tempBuffer - sendBuffer));

		count++;
		size = static_cast<int>(monsterList.size()) - (10 * count);
	}
}

void CSector::Add(CUser* _user)
{
	CLock lock(m_cs_user);
	m_userList.insert(pair<SOCKET, CUser*>(_user->GetSocket(), _user));
}

void CSector::Add(CMonster* _pMonster)
{
	CLock lock(m_cs_monster);
	m_monsterList.insert(pair<int, CMonster*>(_pMonster->GetIndex(), _pMonster));
}

void CSector::Del(CUser* _user)
{
	CLock lock(m_cs_user);
	if (m_userList.find(_user->GetSocket()) != m_userList.end())
	{
		m_userList.erase(_user->GetSocket());
	}
}

void CSector::Del(CMonster* _pMonster)
{
	CLock lock(m_cs_monster);
	if (m_monsterList.find(_pMonster->GetIndex()) != m_monsterList.end())
	{
		m_monsterList.erase(_pMonster->GetIndex());
	}
}

void CSector::Send(char* _buffer, int _size)
{
	CLock lock(m_cs_user);
	map<SOCKET, CUser*> userList = m_userList;
	
	for (auto pUser : userList)
	{
		pUser.second->Send(_buffer, _size);
	}
}

void CSector::SendAll(char* _buffer, int _size)
{
	for (CSector* pSector : m_AdjacentSector)
	{
		pSector->Send(_buffer, _size);
	}
}

int CSector::Size()
{
	return static_cast<int>(m_userList.size());
}

int CSector::GetSectorUserCount()
{
	if (this == nullptr) return 0;
	int count = 0;
	int size = static_cast<int>(m_AdjacentSector.size());
	for (int i = 0; i < size; i++)
	{
		count += m_AdjacentSector[i]->Size();
	}
	return count;
}

map<SOCKET, CUser*> CSector::GetMap()
{
	return m_userList;
}

map<SOCKET, CUser*> CSector::GetAdjacentGetMap(int _index)
{
	return m_AdjacentSector[_index]->GetMap();
}

vector<CSector*> CSector::GetAdjacentSector()
{
	return m_AdjacentSector;
}

int CSector::GetAdjacentUserCount(int _index)
{
	return m_AdjacentSector[_index]->Size();
}

void CSector::SendMonsterInfo(CUser* _pUser)
{
	for (auto monster : m_monsterList)
	{
		monster.second->SendPacketCreate(_pUser);
	}
}

void CSector::SendAdjacentSectorMonsterInfo(CUser* _pUser)
{
	for (auto sector : m_AdjacentSector)
	{
		sector->SendMonsterInfo(_pUser);
	}
}
