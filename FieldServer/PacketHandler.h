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
	/*typedef void(*tFunc) (CUser* _user, char* _buffer);
		함수 포인터 사용시 인자를 통일해야한다.
	tFunc m_lpfp;*/
private:
	CPacketHandler();
	~CPacketHandler();


public:
	int Handle(CUser* _user);
};