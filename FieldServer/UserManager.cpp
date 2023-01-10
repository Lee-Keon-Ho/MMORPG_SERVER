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

void CUserManager::OnPacket() // ���� ������ �� ����
{
	EnterCriticalSection(&m_cs_user);
	std::list<CUser*>::iterator iter = m_userList.begin();
	std::list<CUser*>::iterator iterEnd = m_userList.end();

	CUser* pUser = nullptr;

	for (; iter != iterEnd; iter++)
	{
		pUser = *iter;
		while (true) // ������ ���� �� ������ ����, Packet�� �ִ��� Ȯ���ϴ� �� �����غ���
		{
			//if( pUser->GetPacketBuffer() != nullptr )

			if (pUser->PacketHandle() <= 0) break; //HandlePacket

			//if (pUser->GetReadSize() > 0) pUser->PacketHandle(); // ReadSize ����
			//else break;
		}
	}
	LeaveCriticalSection(&m_cs_user);
}