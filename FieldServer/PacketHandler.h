#pragma once

class CUser;

class CPacketHandler
{
public:
	static CPacketHandler* GetInstance()
	{
		static CPacketHandler instance;
		return &instance;
	}

public:
	typedef void (*tFunc) (CUser* _user, char* _buffer);
		
	tFunc m_lpfp;
private:
	CPacketHandler();
	~CPacketHandler();


public:
	int Handle(CUser* _user);

	void test(CUser* _user, char* _buffer);
	void LogIn(CUser* _user, char* _buffer);
	void InField(CUser* _user, char* _buffer);
	void NewUser(CUser* _user);
	void MoveUser(CUser* _user, char* _buffer);
	void NowPosition(CUser* _user, char* _buffer);
	void MoveUser2(CUser* _user, char* _buffer);
	void Arrive(CUser* _user, char* _buffer);
};