#include "Sector.h"
#include <algorithm>
#include <iostream>

CSector::CSector()
{
	InitializeCriticalSection(&m_cs_user);
}

CSector::CSector(int _x, int _y)
{
	nSector = _y * 43 + _x;
	InitializeCriticalSection(&m_cs_user);
}

CSector::~CSector()
{
	DeleteCriticalSection(&m_cs_user);
}

void CSector::SetAdjacentSector(CSector* _pSector)
{
	m_AdjacentSector.push_back(_pSector);
}

std::vector<CSector*> CSector::Difference(sector_t _b)
{
	std::vector<CSector*> result;
	std::vector<CSector*> a = m_AdjacentSector;

	std::sort(a.begin(), a.end());
	std::sort(_b.begin(), _b.end());
	std::set_difference(a.begin(), a.end(), _b.begin(), _b.end(), std::back_inserter(result));

	return result;
}

void CSector::DeleteUsersOutOfSector(CUser& _user)
{
	EnterCriticalSection(&m_cs_user);
	std::map<SOCKET, CUser*> userList = m_userList;
	LeaveCriticalSection(&m_cs_user);
	
	int size = userList.size();
	
	if (size <= 0) return;

	char sendBuffer[100];
	char* tempBuffer = sendBuffer;

	std::map<SOCKET, CUser*>::iterator iter = userList.begin();
	std::map<SOCKET, CUser*>::iterator iterEnd = userList.end();

	CUser* pUser;
	int count = 0;
	while (iter != iterEnd)
	{
		if (size > 10) size = 10;

		tempBuffer = sendBuffer;

		*(u_short*)tempBuffer = 6 + (2 * size);
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = 8;
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = size;
		tempBuffer += sizeof(u_short);

		for (int i = 0; i < size; i++, iter++)
		{
			pUser = (*iter).second;

			*(u_short*)tempBuffer = pUser->GetNumber();
			tempBuffer += sizeof(u_short);
		}
		_user.Send(sendBuffer, tempBuffer - sendBuffer);

		count++;
		size = userList.size() - (10 * count);
	}
}

void CSector::FetchUserInfoInNewSector(CUser& _user)
{
	EnterCriticalSection(&m_cs_user);
	std::map<SOCKET, CUser*> userList = m_userList;
	LeaveCriticalSection(&m_cs_user);

	int size = userList.size();

	if (size <= 0) return;

	char sendBuffer[1000];
	char* tempBuffer;

	std::map<SOCKET, CUser*>::iterator iter = userList.begin();
	std::map<SOCKET, CUser*>::iterator iterEnd = userList.end();

	CUser* pUser;
	int count = 0;
	while (iter != iterEnd)
	{
		if (size > 10) size = 10;

		tempBuffer = sendBuffer;

		*(u_short*)tempBuffer = 6 + (32 * size);
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = CS_PT_NEWSECTORUSER;
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = size;
		tempBuffer += sizeof(u_short);

		for (int i = 0; i < size; i++, iter++)
		{
			pUser = (*iter).second;

			*(u_short*)tempBuffer = pUser->GetNumber();
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
		_user.Send(sendBuffer, tempBuffer - sendBuffer);

		count++;
		size = userList.size() - (10 * count);
	}
}

void CSector::Add(CUser* _user)
{
	EnterCriticalSection(&m_cs_user);
	m_userList.insert(std::pair<SOCKET, CUser*>(_user->GetSocket(), _user));
	LeaveCriticalSection(&m_cs_user);
}

void CSector::Del(CUser* _user)
{
	EnterCriticalSection(&m_cs_user);
	if (m_userList.find(_user->GetSocket()) != m_userList.end())
	{
		m_userList.erase(_user->GetSocket());
	}
	else
	{
		printf("%d Error\n", _user->GetNumber());
	}
	LeaveCriticalSection(&m_cs_user);
}

void CSector::Send(char* _buffer, int _size)
{
	EnterCriticalSection(&m_cs_user);
	std::map<SOCKET, CUser*> userList = m_userList;
	LeaveCriticalSection(&m_cs_user);

	std::map<SOCKET, CUser*>::iterator iter = userList.begin();
	std::map<SOCKET, CUser*>::iterator iterEnd = userList.end();

	if (iter == iterEnd)
	{
		return;
	}

	CUser* pUser;

	for (; iter != iterEnd; iter++)
	{
		pUser = (*iter).second;
		pUser->Send(_buffer, _size);
	}

/*

	EnterCriticalSection(&m_cs_user);
	std::map<SOCKET, CUser*>::iterator iter = m_userList.begin();
	std::map<SOCKET, CUser*>::iterator iterEnd = m_userList.end();

	if (iter == iterEnd)
	{
		LeaveCriticalSection(&m_cs_user);
		return;
	}

	CUser* pUser;

	for (; iter != iterEnd; iter++)
	{
		pUser = (*iter).second;
		pUser->Send(_buffer, _size);
	}
	LeaveCriticalSection(&m_cs_user);
	*/
}

void CSector::SendAll(char* _buffer, int _size)
{
	std::vector<CSector*>::iterator iter = m_AdjacentSector.begin();
	std::vector<CSector*>::iterator iterEnd = m_AdjacentSector.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->Send(_buffer, _size);
	}
}

int CSector::Size()
{
	return m_userList.size();
}

int CSector::GetSectorUserCount()
{
	if (this == nullptr) return 0;
	int count = 0;
	int size = m_AdjacentSector.size();
	for (int i = 0; i < size; i++)
	{
		count += m_AdjacentSector[i]->Size();
	}
	return count;
}

std::map<SOCKET, CUser*> CSector::GetMap()
{
	return m_userList;
}

std::map<SOCKET, CUser*> CSector::GetAdjacentGetMap(int _index)
{
	return m_AdjacentSector[_index]->GetMap();
}

std::vector<CSector*> CSector::GetAdjacentSector()
{
	return m_AdjacentSector;
}

int CSector::GetAdjacentUserCount(int _index)
{
	return m_AdjacentSector[_index]->Size();
}
