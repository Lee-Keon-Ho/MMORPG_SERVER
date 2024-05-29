#include "Sector.h"
#include <algorithm>
#include <iostream>

CSector::CSector() : nSector(0)
{
	InitializeSRWLock(m_userLock);
	InitializeSRWLock(m_monsterLock);
}

CSector::CSector(int _x, int _y) : m_userLock(new SRWLOCK()), m_monsterLock(new SRWLOCK())
{
	nSector = _y * sector_line + _x;
	InitializeSRWLock(m_userLock);
	InitializeSRWLock(m_monsterLock);
}

CSector::~CSector()
{
	if (m_monsterLock) { delete m_monsterLock; m_monsterLock = nullptr; }
	if (m_userLock) { delete m_userLock; m_userLock = nullptr; }
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
	CLock lock(m_userLock, eLockType::SHARED);

	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_WRAP_DELETE_USER(_pUser.GetIndex());

	for (auto u : m_userList)
	{
		u.second->Send(packet, sizeof(FS2C_PACKET_WRAP_DELETE_USER));
	}
}

void CSector::DeleteUsersOutOfSector(CUser& _user)
{
	CLock lock(m_userLock, eLockType::SHARED);
	
	int size = static_cast<int>(m_userList.size());
	
	if (size <= 0) return;

	map<SOCKET, CUser*>::iterator iter = m_userList.begin();
	map<SOCKET, CUser*>::iterator iterEnd = m_userList.end();
	
	CUser* pUser;
	int count = 0;
	while (iter != iterEnd)
	{
		if (size > packet_user_max_size) size = packet_user_max_size;

		u_short indexArr[packet_user_max_size];

		for (int i = 0; i < size; ++i, ++iter)
		{
			pUser = (*iter).second;
			indexArr[i] = pUser->GetIndex();
		}

		LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_DELETE_USER_OUT_OF_SECTOR(6 + (2 * size), static_cast<u_short>(eFS2C_PT::DELETE_USER_OUT_OF_SECTOR), size, indexArr);

		// 2024-01-16 고민하고 수정
		_user.Send(packet, static_cast <int>(6 + (2 * size)));

		count++;
		size = static_cast<int>(m_userList.size()) - (packet_user_max_size * count);
	}
}

void CSector::FetchUserInfoInNewSector(CUser& _user)
{
	CLock lock(m_userLock, eLockType::SHARED);

	int size = static_cast<int>(m_userList.size());

	if (size <= 0) return;

	std::map<SOCKET, CUser*>::iterator iter = m_userList.begin();
	std::map<SOCKET, CUser*>::iterator iterEnd = m_userList.end();

	CUser* pUser;
	int count = 0;

	while (iter != iterEnd)
	{
		if (size > packet_user_max_size) size = packet_user_max_size;

		sUserInfoNewSector info[packet_user_max_size];

		for (int i = 0; i < size; ++i, ++iter)
		{
			pUser = (*iter).second;

			info[i].index = pUser->GetIndex();
			info[i].state = pUser->GetState();
			info[i].rotationY = pUser->GetRotationY();
			info[i].startPosition = *pUser->GetPosition();
			info[i].goalPosition = *pUser->GetEndPosition();
		}

		LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_FECTCH_USER_INFO_IN_NEWSECTOR(6 + (sizeof(sUserInfoNewSector) * size), static_cast<u_short>(eFS2C_PT::NEW_SECTOR_USER), size, info);

		_user.Send(packet, static_cast <int>(6 + (sizeof(sUserInfoNewSector) * size)));

		count++;
		size = static_cast<int>(m_userList.size()) - (packet_user_max_size * count);
	}
}

void CSector::DeleteMonstersOutOfSector(CUser& _user)
{
	CLock lock(m_monsterLock, eLockType::SHARED);

	int size = static_cast<int>(m_monsterList.size());

	if (size <= 0) return;

	monster_t::iterator iter = m_monsterList.begin();
	monster_t::iterator iterEnd = m_monsterList.end();

	CMonster* pMonster;
	int count = 0;
	
	// 2024-01-07 수정이 필요함
	while (iter != iterEnd)
	{
		if (size > packet_user_max_size) size = packet_user_max_size;

		u_short indexArr[packet_user_max_size];

		for (int i = 0; i < size; ++i, ++iter)
		{
			pMonster = (*iter).second;

			indexArr[i] = pMonster->GetIndex();
		}

		LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_DELETE_MONSTER_OUT_OF_SECTOR(6 + (2 * size), static_cast<u_short>(eFS2C_PT::OUT_SECTOR_MONSTER), size, indexArr);

		_user.Send(packet, static_cast<int>(6 + (2 * size)));

		count++;
		size = static_cast<int>(m_monsterList.size()) - (packet_user_max_size * count);
	}
}

void CSector::FetchMonsterInfoInNewSector(CUser& _user)
{
	CLock lock(m_monsterLock, eLockType::SHARED);

	int size = static_cast<int>(m_monsterList.size());

	if (size <= 0) return;

	monster_t::iterator iter = m_monsterList.begin();
	monster_t::iterator iterEnd = m_monsterList.end();


	CMonster* pMonster;
	int count = 0;
	// 2024-01-07 수정필요
	while (iter != iterEnd)
	{
		if (size > packet_user_max_size) size = packet_user_max_size;

		sMonsterInfoNewSector info[packet_user_max_size];

		for (int i = 0; i < size; ++i, ++iter)
		{
			pMonster = (*iter).second;

			info[i].index = pMonster->GetIndex();
			info[i].type = pMonster->GetType();
			info[i].startPosition = *pMonster->GetPosition();
			info[i].goalPosition = *pMonster->GetDestinationPosition();
		}

		LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_FECTCH_MONSTER_INFO_IN_NEWSECTOR(6 + (sizeof(sMonsterInfoNewSector) * size), static_cast<u_short>(eFS2C_PT::IN_SECTOR_MONSTER), size, info);

		_user.Send(packet, static_cast<int>(6 + (sizeof(sMonsterInfoNewSector) * size)));

		count++;
		size = static_cast<int>(m_monsterList.size()) - (packet_user_max_size * count);
	}
}

void CSector::Add(CUser* _user)
{
	CLock lock(m_userLock, eLockType::EXCLUSIVE);
	m_userList.insert(pair<SOCKET, CUser*>(_user->GetSocket(), _user));
}

void CSector::Add(CMonster* _pMonster)
{
	CLock lock(m_monsterLock, eLockType::EXCLUSIVE);
	m_monsterList.insert(pair<int, CMonster*>(_pMonster->GetIndex(), _pMonster));
}

void CSector::Del(CUser* _user)
{
	CLock lock(m_userLock, eLockType::EXCLUSIVE);
	if (m_userList.find(_user->GetSocket()) != m_userList.end())
	{
		m_userList.erase(_user->GetSocket());
	}
}

void CSector::Del(CMonster* _pMonster)
{
	CLock lock(m_monsterLock, eLockType::EXCLUSIVE);
	if (m_monsterList.find(_pMonster->GetIndex()) != m_monsterList.end())
	{
		m_monsterList.erase(_pMonster->GetIndex());
	}
}

void CSector::Send(LKH::sharedPtr<PACKET> _packet, int _size)
{
	CLock lock(m_userLock, eLockType::SHARED);
	
	for (auto pUser : m_userList)
	{
		pUser.second->Send(_packet, _size);
	}
}

void CSector::SendAll(LKH::sharedPtr<PACKET> _packet, int _size)
{
	for (CSector* pSector : m_AdjacentSector)
	{
		pSector->Send(_packet, _size);
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
	for (int i = 0; i < size; ++i)
	{
		count += m_AdjacentSector[i]->Size();
	}
	return count;
}

map<SOCKET, CUser*>& CSector::GetUserList()
{
	return m_userList;
}

map<SOCKET, CUser*> CSector::GetAdjacentGetMap(int _index)
{
	return m_AdjacentSector[_index]->GetUserList();
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
