#include "UserManager.h"

CUserManager::CUserManager()
{
}

CUserManager::~CUserManager()
{
}

void CUserManager::Add(CUser* _user)
{
	m_userList.push_back(_user);
}
