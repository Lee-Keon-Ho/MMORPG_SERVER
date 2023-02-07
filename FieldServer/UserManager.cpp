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

void CUserManager::OnPacket() // ���� ������ �� ����
{
	//EnterCriticalSection(&m_cs_user);
	std::list<CUser*>::iterator iter = m_userList.begin();
	std::list<CUser*>::iterator iterEnd = m_userList.end();

	if (iter == iterEnd) return;

	CUser* pUser = nullptr;

	pUser = *iter;
	pUser->PacketHandle(); // �������� ó���� ��� �ٸ� �̸��� �Լ��� �ʿ��ϴ�

	//for (; iter != iterEnd; iter++) // ����� ���� ���� ���� Ȯ���� �� ����
	//{
	//	pUser = *iter;
	//	while (true) // ������ ���� �� ������ ����, Packet�� �ִ��� Ȯ���ϴ� �� �����غ���
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

	for (; iter != iterEnd; iter++) // ����� ���� ���� ���� Ȯ���� �� ����
	{
		pUser = *iter;
		pUser->Send(_buffer, _size);
	}
	LeaveCriticalSection(&m_cs_user);
}
