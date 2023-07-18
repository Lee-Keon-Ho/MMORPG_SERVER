#include "UserManager.h"
#include "Map.h"

CUserManager::CUserManager() : m_userNumber(0), m_pMap(nullptr)
{
	InitializeCriticalSection(&m_cs_user);
}

CUserManager::~CUserManager()
{
	DeleteCriticalSection(&m_cs_user);
}

void CUserManager::Add(CUser* _pUser)
{
	EnterCriticalSection(&m_cs_user);
	m_userList.push_back(_pUser);
	LeaveCriticalSection(&m_cs_user);
}

void CUserManager::Del(CUser* _pUser)
{
	EnterCriticalSection(&m_cs_user);
	std::list<CUser*>::iterator iter = m_userList.begin();
	std::list<CUser*>::iterator iterEnd = m_userList.end();
	CUser* pUser;
	for (; iter != iterEnd; )
	{
		pUser = *iter;
		
		if (pUser->GetSocket() == _pUser->GetSocket())
		{
			iter = m_userList.erase(iter);
		}
		else iter++;
	}
	LeaveCriticalSection(&m_cs_user);
}

int CUserManager::AddUserNumber()
{
	m_userNumber++;
	return m_userNumber;
}

int CUserManager::GetUserCount()
{
	return static_cast<int>(m_userList.size());
}

void CUserManager::SendAll(char* _buffer, int _size)
{
	EnterCriticalSection(&m_cs_user);
	std::list<CUser*> userList = m_userList;
	LeaveCriticalSection(&m_cs_user);

	for (CUser* pUser : userList)
	{
		pUser->Send(_buffer, _size);
	}
}

void CUserManager::SendAll(char* _buffer, int _size, SOCKET _socket)
{
	EnterCriticalSection(&m_cs_user);
	std::list<CUser*> userList = m_userList;
	LeaveCriticalSection(&m_cs_user);

	for (CUser* pUser : m_userList)
	{
		if (pUser->GetSocket() != _socket)
		{
			pUser->Send(_buffer, _size);
		}
	}
}

void CUserManager::SendUserCount(CUser& _user)
{
	char sendBuffer[100];
	char* tempBuffer;

	tempBuffer = sendBuffer;

	*(u_short*)tempBuffer = 8;
	tempBuffer += sizeof(u_short);
	*(u_short*)tempBuffer = 12;
	tempBuffer += sizeof(u_short);
	*(u_short*)tempBuffer = GetUserCount();
	tempBuffer += sizeof(u_short);
	*(u_short*)tempBuffer = _user.GetUserCountInSector();
	tempBuffer += sizeof(u_short);

	_user.Send(sendBuffer, static_cast<int>(tempBuffer - sendBuffer));
}

void CUserManager::SetMap(CMap* _map)
{
	m_pMap = _map;
}
