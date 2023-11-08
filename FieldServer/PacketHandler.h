#pragma once
#include "Sector.h"
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

private:
	CPacketHandler();
	~CPacketHandler();


public:
	int Handle(CUser* _pUser, char* _buffer);

	void Latency(CUser* _pUser, char* _buffer);
	void PT_C2FS_LogIn(CUser* _pUser, char* _buffer);
	void PT_C2FS_PlayerInField(CUser* _pUser, char* _buffer);
	void PT_C2FS_NextField(CUser* _pUser, char* _buffer);
	void PT_C2FS_PlayerNowPosition(CUser* _pUser, char* _buffer);
	void PT_C2FS_PlayerMove(CUser* _pUser, char* _buffer);
	void PT_C2FS_PlayerArrive(CUser* _pUser, char* _buffer);
	void PT_C2FS_PlayerLogOut(CUser* _pUser, char* _buffer);
	void PT_C2FS_IDLE_PlayerAttack(CUser* _pUser, char* _buffer);
	void PT_C2FS_MOVE_PlayerAttack(CUser* _pUser, char* _buffer);
	void PT_C2FS_TargetMonster(CUser* _pUser, char* _buffer);
	void PT_C2FS_Chatting(CUser* _pUser, char* _buffer);
	void PT_C2FS_IDLE_ArcherAttack(CUser* _pUser, char* _buffer);
	void PT_C2FS_MOVE_ArcherAttack(CUser* _pUser, char* _buffer);
	void PT_C2FS_HIT_Single_monster(CUser* _pUser, char* _buffer);
	void GetUserCount(CUser*_pUser);

	void test(CUser* _pUser, char* _buffer);
};