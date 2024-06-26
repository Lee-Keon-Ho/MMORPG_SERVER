#pragma once

enum ePacketType
{
	CS_PT_LATENCY = 0,
	CS_PT_LOGIN = 1,
	CS_PT_PLYAER_INFIELD,
	CS_PT_NEXT_FIELD,
	CS_PT_NEWUSERENTRY,
	CS_PT_PLAYER_MOVE,
	CS_PT_PLAYER_ARRIVE,
	CS_PT_PLAYER_LOGOUT,
	CS_PT_DELETE_USER_OUT_OF_SECTOR,
	CS_PT_INSECTOR,
	CS_PT_OUTSECTOR,
	CS_PT_NEWSECTOR_USER,
	CS_PT_IDLE_PLAYER_ATTACK,
	CS_PT_MOVE_PLAYER_ATTACK,
	CS_PT_HIT_TARGET_MONSTER,
	CS_PT_PLAYER_CHATTING,
	CS_PT_NOTICE,
	CS_PT_IDLE_ARCHER_ATTACK,
	CS_PT_MOVE_ARCHER_ATTACK,
	CS_PT_SENDSECTOR_NEWUSERENTRY,
	CS_PT_MOVE_MONSTER,
	CS_PT_CREATE_MONSTER,
	CS_PT_EXIT_SECTOR_MONSTER,
	CS_PT_ENTER_SECTOR_MONSTER,
	CS_PT_INSECTOR_MONSTER,
	CS_PT_OUTSECTOR_MONSTER,
	CS_PT_HIT_MONSTER,
	CS_PT_ATTACK_MONSTER,
	CS_PT_DIE_MONSTER,
	CS_PT_REGEN_MONSTER,
	CS_PT_EXP,
	CS_PT_PLAYER_HIT,
	CS_PT_LEVEL_UP,
	CS_PT_SINGLE_HIT_MONSTER,
	CS_PT_HEART_BEAT
};

enum class eC2FS_PT : u_short
{
	LATENCY = 0,
	LOGINE,
	PLYAER_INFIELD,
	NEXT_FIELD,
	NOW_POSITION,
	PLAYER_MOVE,
	PLAYER_ARRIVE,
	PLAYER_LOGOUT,
	IDLE_PLAYER_ATTACK,
	MOVE_PLAYER_ATTACK,
	HIT_TARGET_MONSTER,
	PLAYER_CHATTING,
	IDLE_ARCHER_ATTACK,
	MOVE_ARCHER_ATTACK,
	SINGLE_HIT_MONSTER,
	HEART_BEAT,
	WARP,
	DUMMY,
	CHANNEL_CHANGE,
	MAX
};

enum class eFS2C_PT : u_short
{
	LATENCY = 0,
	LOGIN,
	PLAYER_INFIELD,
	NEXT_FIELD,
	NEW_USER,
	PLAYER_MOVE,
	PLAYER_ARRIVE,
	PLAYER_LOGOUT,
	DELETE_USER_OUT_OF_SECTOR,
	IN_SECTOR,
	OUT_SECTOR,
	NEW_SECTOR_USER,
	PLAYER_IDLE_ATTACK,
	PLAYER_MOVE_ATTACK,
	CHATTING,
	NOTICE,
	PLAYER_IDLE_ARCHER_ATTACK,
	PLAYER_MOVE_ARCHER_ATTACK,
	SECTOR_NEW_USER_ENTRY,
	MOVE_MONSTER,
	CREATE_MONSTER,
	EXIT_SECTOR_MONSTER,
	ENTER_SECTOR_MONSTER,
	OUT_SECTOR_MONSTER,
	IN_SECTOR_MONSTER,
	HIT_MONSTER,
	ATTACK_MONSTER,
	DIE_MONSTER,
	REGEN_MONSTER,
	EXP,
	PLAYER_HIT,
	LEVEL_UP,
	HEART_BEAT,
	WRAP_DELETE_USER,
	WRAP,
	CHANNEL_CHANGE
};