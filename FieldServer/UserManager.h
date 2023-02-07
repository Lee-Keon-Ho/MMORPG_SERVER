#pragma once
#include "User.h"
#include <list>

class CUserManager
{
public:
	static CUserManager* GetInstance()
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

	CRITICAL_SECTION m_cs_user;
public:
	void Add(CUser* _pUser);
	void Del();
	void OnPacket();

	int GetUserCount();
	void SendAll(char* _buffer, int _size);
	userList_t GetUserList() { return m_userList; }
};