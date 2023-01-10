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
	m_userList.push_back(_pUser);
}

void CUserManager::Del(CUser* _pUser)
{
	
}

void CUserManager::OnPacket() // 이전 구조가 더 좋다
{
	EnterCriticalSection(&m_cs_user);
	std::list<CUser*>::iterator iter = m_userList.begin();
	std::list<CUser*>::iterator iterEnd = m_userList.end();

	CUser* pUser = nullptr;

	for (; iter != iterEnd; iter++)
	{
		pUser = *iter;
		while (true) // 구조를 조금 더 생각해 보자, Packet이 있는지 확인하는 법 생각해보자
		{
			//if( pUser->GetPacketBuffer() != nullptr )

			if (pUser->PacketHandle() <= 0) break; //HandlePacket

			//if (pUser->GetReadSize() > 0) pUser->PacketHandle(); // ReadSize 조절
			//else break;
		}
	}
	LeaveCriticalSection(&m_cs_user);
}