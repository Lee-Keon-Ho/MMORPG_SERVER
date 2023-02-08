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
	void InField(CUser* _user, char* _buffer);
	void MoveUser(CUser* _user, char* _buffer);
	void NowPosition(CUser* _user, char* _buffer);
};