#include "UserManager.h"
#include "../NetCore/Lock.h"

CUserManager::CUserManager() : 
	m_userNumber(0),
	m_userListLock(new SRWLOCK()), 
	m_loginUserSRWLock(new SRWLOCK())
{
	InitializeSRWLock(m_userListLock);
	InitializeSRWLock(m_loginUserSRWLock);
}

CUserManager::~CUserManager()
{
	if (m_loginUserSRWLock) { delete m_loginUserSRWLock; m_loginUserSRWLock = nullptr; }
	if (m_userListLock) { delete m_userListLock; m_userListLock = nullptr; }
}

void CUserManager::Add(ACCEPT_SOCKET_INFO _info)
{
	CLock lock(m_userListLock, eLockType::EXCLUSIVE);
	m_userList.insert(std::make_pair(_info.socket, std::make_unique<CUser>(_info)));
}

void CUserManager::Add(sCharacterInfo& _info, CUser* _pUser)
{
	CLock lock(m_loginUserSRWLock, eLockType::EXCLUSIVE);
	m_loginUserList.insert(make_pair(_info.name, _pUser));
}

void CUserManager::Del(SOCKET _socket)
{
	CLock lock(m_userListLock, eLockType::EXCLUSIVE);
	auto iter = m_userList.find(_socket);
	if (iter != m_userList.end()) m_userList.erase(iter);
}

bool CUserManager::Find(wchar_t* _key, CUser* _pUser)
{
	CLock lock(m_loginUserSRWLock, eLockType::EXCLUSIVE);
	auto pUser = m_loginUserList.find(_key);
	if (pUser == m_loginUserList.end()) return false;

	_pUser->SetInfo(pUser->second->GetCharacterInfo());
	m_loginUserList.erase(_key);

	return true;
}

int CUserManager::AddUserNumber()
{
	return ++m_userNumber;
}

void CUserManager::SendAll(LKH::sharedPtr<PACKET> _buffer, int _size)
{
	CLock lock(m_userListLock, eLockType::SHARED);

	for (auto& pUser : m_userList)
	{
		pUser.second->Send(_buffer, _size);
	}
}

std::map<SOCKET, std::unique_ptr<CUser>> CUserManager::GetUserList()
{
	CLock lock(m_userListLock, eLockType::SHARED);
	return std::move(m_userList);
}

int CUserManager::GetUserSize()
{
	return m_userList.size();
}

void CUserManager::retUser(std::map<SOCKET, std::unique_ptr<CUser>> _pUser)
{
	m_userList = std::move(_pUser);
}
