#include "UserManager.h"

CUserManager::CUserManager()
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

void CUserManager::Del()
{
	/*EnterCriticalSection(&m_cs_user);
	std::list<CUser*>::iterator iter = m_userList.begin();
	std::list<CUser*>::iterator iterEnd = m_userList.end();
	CUser* pUser;
	for (; iter != iterEnd; )
	{
		pUser = *iter;
		
		if (pUser->GetLogOut())
		{
			iter = m_userList.erase(iter);
		}
		else iter++;
	}
	LeaveCriticalSection(&m_cs_user);*/
}

void CUserManager::OnPacket() // 이전 구조가 더 좋다
{
	//EnterCriticalSection(&m_cs_user);
	std::list<CUser*>::iterator iter = m_userList.begin();
	std::list<CUser*>::iterator iterEnd = m_userList.end();

	if (iter == iterEnd) return;

	CUser* pUser = nullptr;

	pUser = *iter;
	pUser->PacketHandle(); // 누적으로 처리할 경우 다른 이름의 함수가 필요하다

	//for (; iter != iterEnd; iter++) // 지우고 나면 다음 값을 확인할 수 없다
	//{
	//	pUser = *iter;
	//	while (true) // 구조를 조금 더 생각해 보자, Packet이 있는지 확인하는 법 생각해보자
	//	{
	//		if (pUser->PacketHandle() <= 0) break; //HandlePacket
	//	}
	//}
	//LeaveCriticalSection(&m_cs_user);
}

int CUserManager::GetUserCount()
{
	return m_userList.size();
}

void CUserManager::SendAll(char* _buffer, int _size)
{
	EnterCriticalSection(&m_cs_user);
	std::list<CUser*>::iterator iter = m_userList.begin();
	std::list<CUser*>::iterator iterEnd = m_userList.end();

	if (iter == iterEnd) return;

	CUser* pUser;

	for (; iter != iterEnd; iter++) // 지우고 나면 다음 값을 확인할 수 없다
	{
		pUser = *iter;
		pUser->Send(_buffer, _size);
	}
	LeaveCriticalSection(&m_cs_user);
}
