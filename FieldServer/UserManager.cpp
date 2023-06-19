#include "UserManager.h"

CUserManager::CUserManager() : m_userNumber(0), currentSectorcount(0)
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
	return m_userList.size();
}

void CUserManager::SendAll(char* _buffer, int _size)
{
	EnterCriticalSection(&m_cs_user);
	std::list<CUser*> userList = m_userList;
	LeaveCriticalSection(&m_cs_user);

	std::list<CUser*>::iterator iter = userList.begin();
	std::list<CUser*>::iterator iterEnd = userList.end();

	if (iter == iterEnd)
	{
		return;
	}

	CUser* pUser;

	for (; iter != iterEnd; iter++)
	{
		pUser = *iter;
		pUser->Send(_buffer, _size);
		break;
	}
}

void CUserManager::SendAll(char* _buffer, int _size, SOCKET _socket)
{
	EnterCriticalSection(&m_cs_user);
	std::list<CUser*> userList = m_userList;
	LeaveCriticalSection(&m_cs_user);

	std::list<CUser*>::iterator iter = userList.begin();
	std::list<CUser*>::iterator iterEnd = userList.end();

	if (iter == iterEnd)
	{
		return;
	}

	CUser* pUser;

	for (; iter != iterEnd; iter++)
	{
		pUser = *iter;
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
	currentSectorcount = _user.GetUserCountInSector();
	*(u_short*)tempBuffer = currentSectorcount;
	tempBuffer += sizeof(u_short);

	_user.Send(sendBuffer, tempBuffer - sendBuffer);
}
