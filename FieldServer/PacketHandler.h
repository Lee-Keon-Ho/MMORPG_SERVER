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
private:
	using tFunc = void (CPacketHandler::*)(CUser* _pUser, PACKET* _packet);

private:
	std::vector<tFunc> m_lpfp;
	int i = 0;
private:
	CPacketHandler();
	~CPacketHandler();

	void PT_C2FS_Latency(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_LogIn(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_PlayerInField(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_NextField(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_PlayerNowPosition(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_PlayerMove(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_PlayerArrive(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_PlayerLogOut(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_IDLE_PlayerAttack(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_MOVE_PlayerAttack(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_TargetMonster(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_Chatting(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_IDLE_ArcherAttack(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_MOVE_ArcherAttack(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_HIT_Single_monster(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_HeartBeat(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_Warp(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_ChannelChange(CUser* _pUser, PACKET* _packet);
	void PT_C2FS_Dummy_NextField(CUser* _pUser, PACKET* _packet);

public:
	int Handle(CUser* _pUser, PACKET* _packet);
};