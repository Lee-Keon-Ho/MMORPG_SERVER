#pragma once
#include "User.h"
#include "Sector.h"
#include "../NetCore/SpinLock.h"
#include <list>
#include <vector>
#include <map>

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
	typedef std::vector<CSector*> Sector_t;
	using userList = std::map<SOCKET, std::unique_ptr<CUser>>;
	typedef std::map<wstring, CUser*> loginUserList;

private:
	userList			m_userList;
	loginUserList		m_loginUserList;
	PSRWLOCK			m_userListLock;
	PSRWLOCK			m_loginUserSRWLock;
	int					m_userNumber;
public:
	void Add(ACCEPT_SOCKET_INFO _pUser);
	void Add(sCharacterInfo& _info, CUser* _pUser);
	void Del(SOCKET _socket);
	bool Find(wchar_t* _key, CUser* _pUser);

	int AddUserNumber();

	void SendAll(LKH::sharedPtr<PACKET> _buffer, int _size);

	std::map<SOCKET, std::unique_ptr<CUser>> GetUserList();
	int GetUserSize();
	void retUser(std::map<SOCKET, std::unique_ptr<CUser>> _pUser);
};