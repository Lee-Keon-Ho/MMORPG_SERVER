#pragma once
#include "User.h"
#include "Sector.h"
#include <list>
#include <vector>

class CMap;

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
	typedef std::vector<CSector*> Sector_t;
	typedef std::map<wstring, CUser*> loginUserList;

private:
	userList_t			m_userList;
	loginUserList		m_loginUserList;
	CRITICAL_SECTION	m_cs_user;
	CRITICAL_SECTION	m_cs_loginUser;
	Sector_t			m_sector;
	int					m_userNumber;
	CMap*				m_pMap;
public:
	void Add(CUser* _pUser);
	void Add(sCharacterInfo& _info, CUser* _pUser);
	void Del(CUser* _pUser);
	bool Find(wchar_t* _key, CUser* _pUser);

	int AddUserNumber();
	int GetUserCount();

	void SendAll(char* _buffer, int _size);
	void SendAll(char* _buffer, int _size, SOCKET _socket);
	userList_t GetUserList() { return m_userList; }

	void SendUserCount(CUser& _user);
	void SetMap(CMap* _map);
};