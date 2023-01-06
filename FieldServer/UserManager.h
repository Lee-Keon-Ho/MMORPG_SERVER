#pragma once
#include "User.h"
#include <list>

class CUserManager
{
public:
	static CUserManager* GetInstatnce()
	{
		static CUserManager instance;
		return &instance;
	}

private:
	CUserManager();
	~CUserManager();

public:
	typedef std::list<CUser*> userList_t;

private:
	userList_t m_userList;

public:
	void Add(CUser* _user);
};

