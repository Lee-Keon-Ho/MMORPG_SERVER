#pragma once
#include "../NetCore/LStruct.h"
#include <string>
#include "VECTOR.h"
#include "struct.h"
#include "constants.h"
#include "PacketType.h"

#pragma pack( push, 1)

//struct PACKET
//{
//	u_short size;
//	u_short type;
//
//	PACKET() : size(0), type(0) {}
//	PACKET(u_short _size, u_short _type) : size(_size), type(_type)	{}
//};

struct FS2C_PACKET_LATENCY : PACKET
{
	float time;

	FS2C_PACKET_LATENCY() : time(0) {}
	FS2C_PACKET_LATENCY(float _time) : time(_time) 
	{
		size = sizeof(FS2C_PACKET_LATENCY);
		type = static_cast<u_short>(eFS2C_PT::LATENCY);
	}
};

struct FS2C_PACKET_NEW_LOGIN : PACKET
{
	u_short index;
	sCharacterInfo characterInfo;

	FS2C_PACKET_NEW_LOGIN() : index(0), characterInfo() {}
	FS2C_PACKET_NEW_LOGIN(u_short _index, sCharacterInfo& _info)
		: index(_index) , characterInfo(_info) 
	{
		size = sizeof(FS2C_PACKET_NEW_LOGIN);
		type = static_cast<u_short>(eFS2C_PT::LOGIN);
	}
};

struct FS2C_PACKET_WARP : PACKET
{
	u_short index;

	FS2C_PACKET_WARP() : index(0) {}
	FS2C_PACKET_WARP(u_short _index) : index(_index) 
	{
		size = sizeof(FS2C_PACKET_WARP);
		type = static_cast<u_short>(eFS2C_PT::WRAP);
	}
};

struct FS2C_PACKET_NEWUSERENTRY : PACKET
{
	u_short index;
	u_short character;
	wchar_t name[name_max] = {0, };
	VECTOR3 position;

	FS2C_PACKET_NEWUSERENTRY() : index(0), character(0), position({0,0,0}), name(L"") {}
	FS2C_PACKET_NEWUSERENTRY(u_short _index, u_short _character, const wchar_t* _name, VECTOR3 _position) : index(_index), character(_character), position(_position)
	{
		size = sizeof(FS2C_PACKET_NEWUSERENTRY);
		type = static_cast<u_short>(eFS2C_PT::NEW_USER);
		memcpy(name, _name, 16);
	}
};

struct FS2C_PACKET_MOVE_USER : PACKET
{
	u_short index;
	VECTOR3 currentPosition;
	VECTOR3 endPosition;

	FS2C_PACKET_MOVE_USER() : index(0), currentPosition({ 0,0,0 }), endPosition({ 0,0,0 }) {}
	FS2C_PACKET_MOVE_USER(u_short _index, VECTOR3 _currentPosition, VECTOR3 _endPosition) : index(_index), currentPosition(_currentPosition), endPosition(_endPosition) 
	{
		size = sizeof(FS2C_PACKET_MOVE_USER);
		type = static_cast<u_short>(eFS2C_PT::PLAYER_MOVE);
	}
};

struct FS2C_PACKET_INSECTOR : PACKET
{
	u_short index;
	VECTOR3 currentPosition;
	VECTOR3 endPosition;

	FS2C_PACKET_INSECTOR() : index(0), currentPosition({ 0,0,0 }), endPosition({ 0,0,0 }) {}
	FS2C_PACKET_INSECTOR(u_short _index, VECTOR3 _currentPosition, VECTOR3 _endPosition)
		: index(_index), currentPosition(_currentPosition), endPosition(_endPosition) 
	{
		size = sizeof(FS2C_PACKET_INSECTOR);
		type = static_cast<u_short>(eFS2C_PT::IN_SECTOR);
	}
};

struct FS2C_PACKET_OUTSECTOR : PACKET
{
	u_short index;

	FS2C_PACKET_OUTSECTOR() : index(0) {}
	FS2C_PACKET_OUTSECTOR(u_short _index) : index(_index) 
	{
		size = sizeof(FS2C_PACKET_OUTSECTOR);
		type = static_cast<u_short>(eFS2C_PT::OUT_SECTOR);
	}
};

struct FS2C_PACKET_ARRIVE : PACKET
{
	u_short		index;
	float		rotationY;
	VECTOR3		endPosition;

	FS2C_PACKET_ARRIVE() : index(0), rotationY(0), endPosition({ 0,0,0 }) {};
	FS2C_PACKET_ARRIVE(u_short _index, float _rotationY, VECTOR3 _endPosition) :
		index(_index), rotationY(_rotationY), endPosition(_endPosition) 
	{
		size = sizeof(FS2C_PACKET_ARRIVE);
		type = static_cast<u_short>(eFS2C_PT::PLAYER_ARRIVE);
	}
};

struct FS2C_PACKET_LOGOUT : PACKET
{
	u_short index;

	FS2C_PACKET_LOGOUT() : index(0) {}
	FS2C_PACKET_LOGOUT(u_short _index) : index(_index) 
	{
		size = sizeof(FS2C_PACKET_LOGOUT);
		type = static_cast<u_short>(eFS2C_PT::PLAYER_LOGOUT);
	}
};

struct FS2C_PACKET_CHANNEL_CHANGE : PACKET
{
	FS2C_PACKET_CHANNEL_CHANGE()
	{
		size = sizeof(FS2C_PACKET_CHANNEL_CHANGE);
		type = static_cast<u_short>(eFS2C_PT::CHANNEL_CHANGE);;
	}
};

struct FS2C_PACKET_CLOSE_SOCKET : PACKET
{
	FS2C_PACKET_CLOSE_SOCKET()
	{
		size = sizeof(FS2C_PACKET_CLOSE_SOCKET);
		type = 98;
	}
};

struct FS2C_PACKET_IDLE_ATTACK : PACKET
{
	u_short index;
	float rotationY;

	FS2C_PACKET_IDLE_ATTACK() : index(0), rotationY(0) {};
	FS2C_PACKET_IDLE_ATTACK(u_short _index, float _rotationY) :
		index(_index), rotationY(_rotationY) 
	{
		size = sizeof(FS2C_PACKET_IDLE_ATTACK);
		type = static_cast<u_short>(eFS2C_PT::PLAYER_IDLE_ATTACK);
	}
};

struct FS2C_PACKET_MOVE_ATTACK : PACKET
{
	u_short index;
	float rotationY;
	VECTOR3 currentPosition;

	FS2C_PACKET_MOVE_ATTACK() : index(0), rotationY(0), currentPosition({ 0,0,0 }) {};
	FS2C_PACKET_MOVE_ATTACK(u_short _index, float _rotationY, VECTOR3 _currentPosition) :
		index(_index), rotationY(_rotationY), currentPosition(_currentPosition) 
	{
		size = sizeof(FS2C_PACKET_MOVE_ATTACK);
		type = static_cast<u_short>(eFS2C_PT::PLAYER_MOVE_ATTACK);
	}
};

struct FS2C_PACKET_IDLE_ARCHER_ATTACK : PACKET
{
	u_short index;
	float rotation_y;

	FS2C_PACKET_IDLE_ARCHER_ATTACK() : index(0), rotation_y(0.0f) {}
	FS2C_PACKET_IDLE_ARCHER_ATTACK(u_short _index, float _rotationY) : 
		index(_index), rotation_y(_rotationY) 
	{
		size = sizeof(FS2C_PACKET_IDLE_ARCHER_ATTACK);
		type = static_cast<u_short>(eFS2C_PT::PLAYER_IDLE_ARCHER_ATTACK);
	}
};

struct FS2C_PACKET_MOVE_ARCHER_ATTACK : PACKET
{
	u_short index;
	float rotation_y;
	VECTOR3 position;

	FS2C_PACKET_MOVE_ARCHER_ATTACK() : index(0), rotation_y(0.0f), position({0.0f, 0.0f, 0.0f}) {}
	FS2C_PACKET_MOVE_ARCHER_ATTACK(u_short _index, float _rotationY, VECTOR3 _position) : index(_index), rotation_y(_rotationY), position(_position) 
	{
		size = sizeof(FS2C_PACKET_MOVE_ARCHER_ATTACK);
		type = static_cast<u_short>(eFS2C_PT::PLAYER_MOVE_ARCHER_ATTACK);
	}
};

struct FS2C_PACKET_ADJACENTSECTOR_NEWUSERENTRY : PACKET
{
	u_short index;

	FS2C_PACKET_ADJACENTSECTOR_NEWUSERENTRY() : index(0) {}
	FS2C_PACKET_ADJACENTSECTOR_NEWUSERENTRY(u_short _index) : index(_index) 
	{
		size = sizeof(FS2C_PACKET_ADJACENTSECTOR_NEWUSERENTRY);
		type = static_cast<u_short>(eFS2C_PT::SECTOR_NEW_USER_ENTRY);
	}
};

struct FS2C_PACKET_CREATE_MONSTER : PACKET
{
	u_short index;
	u_short monsterType;
	u_short level;
	VECTOR3 currentPosition;
	VECTOR3 destinationPosition;

	FS2C_PACKET_CREATE_MONSTER() : index(0), monsterType(0), level(0), currentPosition({ 0,0,0 }), destinationPosition({ 0,0,0 }) {}
	FS2C_PACKET_CREATE_MONSTER(u_short _index, u_short _monsterType, u_short _level, VECTOR3 _currentPosition, VECTOR3 _destinationPosition) : 
		index(_index), monsterType(_monsterType), level(_level), currentPosition(_currentPosition), destinationPosition(_destinationPosition) 
	{
		size = sizeof(FS2C_PACKET_CREATE_MONSTER);
		type = static_cast<u_short>(eFS2C_PT::CREATE_MONSTER);
	}
};

struct FS2C_PACKET_MOVE_MONSTER : PACKET
{
	u_short index;
	u_short monsterType;
	VECTOR3 currentPosition;
	VECTOR3 destinationPosition;

	FS2C_PACKET_MOVE_MONSTER() : index(0), monsterType(0), currentPosition({ 0,0,0 }), destinationPosition({ 0,0,0 }) {}
	FS2C_PACKET_MOVE_MONSTER(u_short _index, u_short _monsterType, VECTOR3 _currentPosition, VECTOR3 _destinationPosition) : 
		index(_index), monsterType(_monsterType), currentPosition(_currentPosition), destinationPosition(_destinationPosition) 
	{
		size = sizeof(FS2C_PACKET_MOVE_MONSTER);
		type = static_cast<u_short>(eFS2C_PT::MOVE_MONSTER);
	}
};

struct FS2C_PACKET_EXIT_SECTOR_MONSTER : PACKET
{
	u_short index;
	u_short monsterType;

	FS2C_PACKET_EXIT_SECTOR_MONSTER() : index(0), monsterType(0) {}
	FS2C_PACKET_EXIT_SECTOR_MONSTER(u_short _index, u_short _monsterType) : 
		index(_index), monsterType(_monsterType) 
	{
		size = sizeof(FS2C_PACKET_EXIT_SECTOR_MONSTER);
		type = static_cast<u_short>(eFS2C_PT::EXIT_SECTOR_MONSTER);
	}
};

struct FS2C_PACKET_ENTER_SECTOR_MONSTER : PACKET
{
	u_short index;
	u_short monsterType;
	VECTOR3 currentPosition;
	VECTOR3 m_destinationPosition;

	FS2C_PACKET_ENTER_SECTOR_MONSTER() : index(0), monsterType(0), currentPosition({ 0,0,0 }), m_destinationPosition({0,0,0}) {}
	FS2C_PACKET_ENTER_SECTOR_MONSTER(u_short _index, u_short _monsterType, VECTOR3 _currentPosition, VECTOR3 _destinationPosition) : 
		index(_index), monsterType(_monsterType), currentPosition(_currentPosition), m_destinationPosition(_destinationPosition) 
	{
		size = sizeof(FS2C_PACKET_ENTER_SECTOR_MONSTER);
		type = static_cast<u_short>(eFS2C_PT::ENTER_SECTOR_MONSTER);
	}
};

struct FS2C_PACKET_HIT_MONSTER : PACKET
{
	u_short index;
	u_short curHp;
	u_short playerDamage;

	FS2C_PACKET_HIT_MONSTER() : index(0), curHp(0), playerDamage(0) {}
	FS2C_PACKET_HIT_MONSTER(u_short _index, u_short _curHp, u_short _damage) :
		index(_index), curHp(_curHp), playerDamage(_damage) 
	{
		size = sizeof(FS2C_PACKET_HIT_MONSTER);
		type = static_cast<u_short>(eFS2C_PT::HIT_MONSTER);
	}
};

struct FS2C_PACKET_ATTACK_MONSTER : PACKET
{
	u_short index;
	VECTOR3 currentPosition;

	FS2C_PACKET_ATTACK_MONSTER() : index(0), currentPosition({0,0,0}) {}
	FS2C_PACKET_ATTACK_MONSTER(u_short _index, VECTOR3 _position) : 
		index(_index), currentPosition(_position) 
	{
		size = sizeof(FS2C_PACKET_ATTACK_MONSTER);
		type = static_cast<u_short>(eFS2C_PT::ATTACK_MONSTER);
	}
};
struct FS2C_PACKET_DIE_MONSTER : PACKET
{
	u_short index;
	u_short playerDamage;

	FS2C_PACKET_DIE_MONSTER() : index(0), playerDamage(0) {}
	FS2C_PACKET_DIE_MONSTER(u_short _index, u_short _damage) : 
		index(_index), playerDamage(_damage) 
	{
		size = sizeof(FS2C_PACKET_DIE_MONSTER);
		type = static_cast<u_short>(eFS2C_PT::DIE_MONSTER);
	}
};
struct FS2C_PACKET_REGEN_MONSTER : PACKET
{
	u_short index;
	u_short monsterType;
	u_short level;
	VECTOR3 position;

	FS2C_PACKET_REGEN_MONSTER() : index(0), monsterType(0), level(0), position({ 0,0,0 }) {}
	FS2C_PACKET_REGEN_MONSTER(u_short _index, u_short _monsterType, u_short _level, VECTOR3 _position) : 
		index(_index), monsterType(_monsterType), level(_level), position(_position) 
	{
		size = sizeof(FS2C_PACKET_REGEN_MONSTER);
		type = static_cast<u_short>(eFS2C_PT::REGEN_MONSTER);
	}
};
struct FS2C_PACKET_EXP : PACKET
{
	float exp;

	FS2C_PACKET_EXP() : exp(0.0f) {}
	FS2C_PACKET_EXP(float _exp) : exp(_exp) 
	{
		size = sizeof(FS2C_PACKET_EXP);
		type = static_cast<u_short>(eFS2C_PT::EXP);
	}
};
struct FS2C_PACKET_PLAYER_HIT : PACKET
{
	u_short curHp;

	FS2C_PACKET_PLAYER_HIT() : curHp(0) {}
	FS2C_PACKET_PLAYER_HIT(u_short _curHp) : curHp(_curHp) 
	{
		size = sizeof(FS2C_PACKET_PLAYER_HIT);
		type = static_cast<u_short>(eFS2C_PT::PLAYER_HIT);
	}
};
struct FS2C_PACKET_PLAYER_LEVEL_UP : PACKET
{
	u_short level;
	float curExp;
	float maxExp;

	FS2C_PACKET_PLAYER_LEVEL_UP() : level(0), curExp(0), maxExp(0) {}
	FS2C_PACKET_PLAYER_LEVEL_UP(u_short _level, u_short _curExp, u_short _maxExp) : level(_level), curExp(_curExp), maxExp(_maxExp) 
	{
		size = sizeof(FS2C_PACKET_PLAYER_LEVEL_UP);
		type = static_cast<u_short>(eFS2C_PT::LEVEL_UP);
	}
};

struct PACKET_NEXT_FIELD : PACKET
{
	VECTOR3 position;

	PACKET_NEXT_FIELD() : position({ 0.0f, 0.0f, 0.0f }) {}
	PACKET_NEXT_FIELD(VECTOR3& _position) : position(_position) 
	{
		size = sizeof(PACKET_NEXT_FIELD);
		type = static_cast<u_short>(eFS2C_PT::NEXT_FIELD);
	}
};

struct FS2C_PACKET_NOTICE : PACKET
{
	wchar_t notice[chat_max_size] = { 0, };

	FS2C_PACKET_NOTICE() : notice(L"") {}
	FS2C_PACKET_NOTICE(u_short _noticeSize, const wchar_t* _notice)
	{
		size = sizeof(FS2C_PACKET_NOTICE);
		type = static_cast<u_short>(eFS2C_PT::NOTICE);
		memcpy(notice, _notice, sizeof(wchar_t) * chat_max_size);
	}
};

struct FS2C_PACKET_WRAP_DELETE_USER : PACKET
{
	u_short index;

	FS2C_PACKET_WRAP_DELETE_USER() : index(0) {}
	FS2C_PACKET_WRAP_DELETE_USER(u_short _index) : index(_index) 
	{
		size = sizeof(FS2C_PACKET_WRAP_DELETE_USER);
		type = static_cast<u_short>(eFS2C_PT::WRAP_DELETE_USER);
	}
};

struct FS2C_PACKET_DELETE_USER_OUT_OF_SECTOR : PACKET
{
	u_short count;
	u_short index[10];

	FS2C_PACKET_DELETE_USER_OUT_OF_SECTOR() : count(0)
	{
		memset(index, 0, 10);
	}
	FS2C_PACKET_DELETE_USER_OUT_OF_SECTOR(u_short _size, u_short _type, u_short _count, u_short* _index) : PACKET(_size, _type), count(_count)
	{
		memcpy(index, _index, sizeof(u_short) * _count);
	}
};

struct FS2C_PACKET_FECTCH_USER_INFO_IN_NEWSECTOR : PACKET
{
	u_short count;
	sUserInfoNewSector userInfo[10];

	FS2C_PACKET_FECTCH_USER_INFO_IN_NEWSECTOR() : count(0)
	{
		memset(userInfo, 0, 10);
	}
	FS2C_PACKET_FECTCH_USER_INFO_IN_NEWSECTOR(u_short _size, u_short _type, u_short _count, sUserInfoNewSector* _info) : PACKET(_size, _type), count(_count)
	{
		memcpy(userInfo, _info, sizeof(sUserInfoNewSector) * _count);
	}
};

struct FS2C_PACKET_DELETE_MONSTER_OUT_OF_SECTOR : PACKET
{
	u_short count;
	u_short index[10];

	FS2C_PACKET_DELETE_MONSTER_OUT_OF_SECTOR() : count(0)
	{
		memset(index, 0, 10);
	}
	FS2C_PACKET_DELETE_MONSTER_OUT_OF_SECTOR(u_short _size, u_short _type, u_short _count, u_short* _index) : PACKET(_size, _type), count(_count)
	{
		memcpy(index, _index, sizeof(u_short) * _count);
	}
};

struct FS2C_PACKET_FECTCH_MONSTER_INFO_IN_NEWSECTOR : PACKET
{
	u_short count;
	sMonsterInfoNewSector userInfo[10];

	FS2C_PACKET_FECTCH_MONSTER_INFO_IN_NEWSECTOR() : count(0)
	{
		memset(userInfo, 0, 10);
	}
	FS2C_PACKET_FECTCH_MONSTER_INFO_IN_NEWSECTOR(u_short _size, u_short _type, u_short _count, sMonsterInfoNewSector* _info) : PACKET(_size, _type), count(_count)
	{
		memcpy(userInfo, _info, sizeof(sMonsterInfoNewSector) * _count);
	}
};

struct FS2C_PACKET_CHATTING : PACKET
{
	u_short index;
	wchar_t name[name_max] = { 0, };
	wchar_t chat[chat_max_size] = { 0, };

	FS2C_PACKET_CHATTING() {}
	FS2C_PACKET_CHATTING(u_short _index, wchar_t* _pName, wchar_t* _pChat) :
		index(_index)
	{
		size = sizeof(FS2C_PACKET_CHATTING);
		type = static_cast<u_short>(eFS2C_PT::CHATTING);
		memcpy(name, _pName, sizeof(wchar_t) * name_max);
		memcpy(chat, _pChat, sizeof(wchar_t) * chat_max_size);
	}
};

struct FS2C_PACKET_INFIELD : PACKET
{
	u_short userCount;
	sInfield info[10];

	FS2C_PACKET_INFIELD() : userCount(0) {}
	FS2C_PACKET_INFIELD(u_short _size, u_short _type, u_short _count, sInfield* _pInfield)
		: PACKET(_size, _type), userCount(_count)
	{
		memcpy(info, _pInfield, sizeof(sInfield) * userCount);
	}
};

struct FS2C_PACKET_HEART_BEAT : PACKET
{
	FS2C_PACKET_HEART_BEAT() 
	{
		size = sizeof(FS2C_PACKET_HEART_BEAT);
		type = static_cast<u_short>(eFS2C_PT::HEART_BEAT);
	}
};

struct FS2MC_PACKET : PACKET
{
	float time;
	u_short userCount;

	FS2MC_PACKET() : time(0.0f), userCount(0) {}
	FS2MC_PACKET(float _time, u_short _userCount) :
		time(_time), userCount(_userCount)
	{
		size = sizeof(FS2MC_PACKET);
		type = 1;
	}
};

struct FS2WS_PACKET_IP_PORT : PACKET
{
	u_long ip;
	u_short port;

	FS2WS_PACKET_IP_PORT(u_long _ip, u_short _port) : ip(_ip), port(_port)
	{
		size = sizeof(FS2WS_PACKET_IP_PORT);
		type = 0;
	}
};

struct C2FS_PACKET_LATENCY : PACKET
{
	float time;

	C2FS_PACKET_LATENCY() : time(0.0f) {}
};

struct C2FS_PACKET_LOGIN : PACKET
{
	wchar_t key[name_max] = { 0, };

	C2FS_PACKET_LOGIN() {}
};

struct C2FS_PACKET_NEXT_FIELD : PACKET
{
	u_short fieldIndex;

	C2FS_PACKET_NEXT_FIELD() : fieldIndex(0) {}
};

struct C2FS_PACKET_WARP : PACKET
{
	u_short field;

	C2FS_PACKET_WARP() : field(0) {}
};

struct C2FS_PACKET_NOW_POSITION : PACKET
{
	u_short index;
	VECTOR3 position;

	C2FS_PACKET_NOW_POSITION() : index(0), position({ 0.0f, 0.0f, 0.0f }) {}
};

struct C2FS_PACKET_PLYAER_MOVE : PACKET
{
	u_short index;
	u_short state;
	VECTOR3 currentPosition;
	VECTOR3 goalPosition;

	C2FS_PACKET_PLYAER_MOVE() : index(0), state(0), currentPosition({ 0.0f, 0.0f, 0.0f }), goalPosition({ 0.0f, 0.0f, 0.0f }) {}
};

struct C2FS_PACKET_PLAYER_ARRIVE : PACKET
{
	u_short index;
	u_short state;
	float	rotation_y;
	VECTOR3 position;

	C2FS_PACKET_PLAYER_ARRIVE() : index(0), state(0), rotation_y(0.0f), position({ 0.0f, 0.0f, 0.0f }) {}
};

struct C2FS_PACKET_PLAYER_IDLE_ATTACK : PACKET
{
	float rotation_y;

	C2FS_PACKET_PLAYER_IDLE_ATTACK() : rotation_y(0.0f) {}
};

struct C2FS_PACKET_PLAYER_MOVE_ATTACK : PACKET
{
	VECTOR3 position;
	float rotation_y;

	C2FS_PACKET_PLAYER_MOVE_ATTACK() : position({0.0f, 0.0f, 0.0f}), rotation_y(0.0f) {}
};

struct C2FS_PACKET_TARGET_MONSTER : PACKET
{
	u_short count;
	u_short indexs[5] = { 0, };
	C2FS_PACKET_TARGET_MONSTER() : count(0) {}
};

struct C2FS_PACKET_CHATTING : PACKET
{
	wchar_t str[chat_max_size] = { 0, };

	C2FS_PACKET_CHATTING() {}
};

struct C2FS_PACKET_IDLE_ARCHER_ATTACK : PACKET
{
	float rotation_y;

	C2FS_PACKET_IDLE_ARCHER_ATTACK() : rotation_y(0.0f) {}
};

struct C2FS_PACKET_MOVE_ARCHER_ATTACK : PACKET
{
	VECTOR3 position;
	float rotation_y;

	C2FS_PACKET_MOVE_ARCHER_ATTACK() : position({ 0.0f, 0.0f, 0.0f }), rotation_y(0.0f) {}
};

struct C2FS_PACKET_HIT_SINGLE_MONSTER : PACKET
{
	u_short index;

	C2FS_PACKET_HIT_SINGLE_MONSTER() : index(0) {}
};

struct LS2FS_PACKET : PACKET
{
	u_short len;
	sCharacterInfo characterInfo;

	LS2FS_PACKET() : len(0) {}
};

struct FS2LS_PACKET : PACKET
{
	wchar_t key[name_max] = { 0, };

	FS2LS_PACKET() {}
	FS2LS_PACKET(wchar_t* _key)
	{
		size = sizeof(FS2LS_PACKET);
		type = 0;
		memcpy(key, _key, sizeof(wchar_t) * name_max);
	}
};

struct FS2C_PACKET_DUMMY_NEXT_FIELD : PACKET
{
	u_short fieldIndex;
	VECTOR3 position = { 0.0f, 0.0f, 0.0f };

	FS2C_PACKET_DUMMY_NEXT_FIELD() : fieldIndex(0) {}
};

struct MC2LS_PACKET : PACKET
{
	float time;

	MC2LS_PACKET() : time(0.0f) {}
};
#pragma pack( pop )