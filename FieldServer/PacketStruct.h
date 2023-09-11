#pragma once
#include <WinSock2.h>
#include <string>
#include "VECTOR.h"

#pragma pack( push, 1)

struct PACKET
{
	u_short size;
	u_short type;

	PACKET() : size(0), type(0) {}
	PACKET(u_short _size, u_short _type) : size(_size), type(_type)	{}
};

struct PACKET_LATENCY : PACKET
{
	float time;

	PACKET_LATENCY() : time(0) {}
	PACKET_LATENCY(u_short _size, u_short _type, float _time) :
		PACKET(_size, _type), time(_time) {}
};

struct PACKET_NEW_LOGIN : PACKET
{
	u_short index;
	u_short character;
	u_short level;
	u_short nameSize;
	char name[8];

	PACKET_NEW_LOGIN() : index(0), character(0), level(0), nameSize(0), name("") {}
	PACKET_NEW_LOGIN(u_short _size, u_short _type, u_short _index, u_short _character, u_short _level, u_short _nameSize, const char* _name)
		: PACKET(_size, _type), index(_index) , character(_character), level(_level), nameSize(_nameSize)
	{
		memset(name, 0, 8);
		memcpy(name, _name, 8);
	}
};

struct PACKET_LOGIN : PACKET
{
	u_short index;

	PACKET_LOGIN() : index(0) {}
	PACKET_LOGIN(u_short _size, u_short _type, u_short _index)
		: PACKET(_size, _type), index(_index) {}
};

struct PACKET_NEWUSERENTRY : PACKET
{
	u_short index;
	u_short character;
	char name[9];
	VECTOR3 position;

	PACKET_NEWUSERENTRY() : index(0), character(0), position({0,0,0}), name("") {}
	PACKET_NEWUSERENTRY(u_short _size, u_short _type, u_short _index, u_short _character, const char* _name, VECTOR3 _position)
		: PACKET(_size, _type), index(_index), character(_character), position(_position)
	{
		memset(name, 0, 9);
		memcpy(name, _name, 9);
	}
};

struct PACKET_NOWPOSITION : PACKET
{
	u_short index;
	VECTOR3 position;

	PACKET_NOWPOSITION() : index(0), position({ 0,0,0 }) {}
	PACKET_NOWPOSITION(u_short _size, u_short _type, u_short _index, VECTOR3 _position) : PACKET(_size, _type), index(_index), position(_position) {}
};

struct PAKCET_INFIELD : PACKET
{
	float x;
	float y;
	float z;

	PAKCET_INFIELD() : x(0), y(0), z(0) {}
	PAKCET_INFIELD(u_short _size, u_short _type, float _x, float _y, float _z) 
		: PACKET(_size, _type), x(_x), y(_y), z(_z) {}
};

struct PACKET_SECTOR : PACKET
{
	u_short index;
	u_short sector;

	PACKET_SECTOR() : index(0), sector(0) {}
	PACKET_SECTOR(u_short _size, u_short _type, u_short _index, u_short _sector)
		: PACKET(_size, _type), index(_index), sector(_sector) {}
};

struct PACKET_MOVE_USER : PACKET_LOGIN
{
	VECTOR3 currentPosition;
	VECTOR3 endPosition;

	PACKET_MOVE_USER() : currentPosition({ 0,0,0 }), endPosition({ 0,0,0 }) {}
	PACKET_MOVE_USER(u_short _size, u_short _type, u_short _index, VECTOR3 _currentPosition, VECTOR3 _endPosition) :
		PACKET_LOGIN(_size, _type, _index), currentPosition(_currentPosition), endPosition(_endPosition) {}
};

struct PACKET_INSECTOR : PACKET_LOGIN
{
	VECTOR3 currentPosition;
	VECTOR3 endPosition;

	PACKET_INSECTOR() : currentPosition({ 0,0,0 }), endPosition({ 0,0,0 }) {}
	PACKET_INSECTOR(u_short _size, u_short _type, u_short _index, VECTOR3 _currentPosition, VECTOR3 _endPosition)
		: PACKET_LOGIN(_size, _type, _index), currentPosition(_currentPosition), endPosition(_endPosition) {}
};

struct PACKET_OUTSECTOR : PACKET
{
	u_short index;

	PACKET_OUTSECTOR() : index(0) {}
	PACKET_OUTSECTOR(u_short _size, u_short _type, u_short _index) :
		PACKET(_size, _type), index(_index) {}
};

struct PACKET_ARRIVE : PACKET
{
	u_short		index;
	float		rotationY;
	VECTOR3		endPosition;

	PACKET_ARRIVE() : index(0), rotationY(0), endPosition({ 0,0,0 }) {};
	PACKET_ARRIVE(u_short _size, u_short _type, u_short _index, float _rotationY, VECTOR3 _endPosition) :
		PACKET(_size, _type), index(_index), rotationY(_rotationY), endPosition(_endPosition) {}
};

struct PACKET_LOGOUT : PACKET
{
	u_short index;

	PACKET_LOGOUT() : index(0) {}
	PACKET_LOGOUT(u_short _size, u_short _type, u_short _index) :
		PACKET(_size, _type), index(_index) {}
};

struct PACKET_IDLE_ATTACK : PACKET
{
	u_short index;
	float rotationY;

	PACKET_IDLE_ATTACK() : index(0), rotationY(0) {};
	PACKET_IDLE_ATTACK(u_short _size, u_short _type, u_short _index, float _rotationY) :
		PACKET(_size, _type), index(_index), rotationY(_rotationY) {}
};

struct PACKET_MOVE_ATTACK : PACKET
{
	u_short index;
	float rotationY;
	VECTOR3 currentPosition;

	PACKET_MOVE_ATTACK() : index(0), rotationY(0), currentPosition({ 0,0,0 }) {};
	PACKET_MOVE_ATTACK(u_short _size, u_short _type, u_short _index, float _rotationY, VECTOR3 _currentPosition) :
		PACKET(_size, _type), index(_index), rotationY(_rotationY), currentPosition(_currentPosition) {}
};

struct PACKET_ADJACENTSECTOR_NEWUSERENTRY : PACKET
{
	u_short index;

	PACKET_ADJACENTSECTOR_NEWUSERENTRY() : index(0) {}
	PACKET_ADJACENTSECTOR_NEWUSERENTRY(u_short _size, u_short _type, u_short _index) : PACKET(_size, _type), index(_index) {}
};

struct PACKET_CREATE_MONSTER : PACKET
{
	u_short index;
	u_short monsterType;
	u_short level;
	VECTOR3 currentPosition;
	VECTOR3 destinationPosition;

	PACKET_CREATE_MONSTER() : index(0), monsterType(0), level(0), currentPosition({ 0,0,0 }), destinationPosition({ 0,0,0 }) {}
	PACKET_CREATE_MONSTER(u_short _size, u_short _type, u_short _index, u_short _monsterType, u_short _level, VECTOR3 _currentPosition, VECTOR3 _destinationPosition) : PACKET(_size, _type), index(_index), monsterType(_monsterType), level(_level), currentPosition(_currentPosition), destinationPosition(_destinationPosition) {}
};

struct PACKET_MOVE_MONSTER : PACKET
{
	u_short index;
	u_short monsterType;
	VECTOR3 currentPosition;
	VECTOR3 destinationPosition;

	PACKET_MOVE_MONSTER() : index(0), monsterType(0), currentPosition({ 0,0,0 }), destinationPosition({ 0,0,0 }) {}
	PACKET_MOVE_MONSTER(u_short _size, u_short _type, u_short _index, u_short _monsterType, VECTOR3 _currentPosition, VECTOR3 _destinationPosition) : PACKET(_size, _type),
		index(_index), monsterType(_monsterType), currentPosition(_currentPosition), destinationPosition(_destinationPosition) {}
};

struct PACKET_EXIT_SECTOR_MONSTER : PACKET
{
	u_short index;
	u_short monsterType;

	PACKET_EXIT_SECTOR_MONSTER() : index(0), monsterType(0) {}
	PACKET_EXIT_SECTOR_MONSTER(u_short _size, u_short _type, u_short _index, u_short _monsterType) : PACKET(_size, _type), index(_index), monsterType(_monsterType) {}
};

struct PACKET_ENTER_SECTOR_MONSTER : PACKET
{
	u_short index;
	u_short monsterType;
	VECTOR3 currentPosition;
	VECTOR3 m_destinationPosition;

	PACKET_ENTER_SECTOR_MONSTER() : index(0), monsterType(0), currentPosition({ 0,0,0 }), m_destinationPosition({0,0,0}) {}
	PACKET_ENTER_SECTOR_MONSTER(u_short _size, u_short _type, u_short _index, u_short _monsterType, VECTOR3 _currentPosition, VECTOR3 _destinationPosition) : PACKET(_size, _type), index(_index), monsterType(_monsterType), currentPosition(_currentPosition), m_destinationPosition(_destinationPosition) {}
};

struct PACKET_HIT_MONSTER : PACKET
{
	u_short index;
	u_short curHp;
	u_short playerDamage;

	PACKET_HIT_MONSTER() : index(0), curHp(0), playerDamage(0) {}
	PACKET_HIT_MONSTER(u_short _size, u_short _type, u_short _index, u_short _curHp, u_short _damage) : PACKET(_size, _type), index(_index), curHp(_curHp), playerDamage(_damage) {}
};

struct PACKET_ATTACK_MONSTER : PACKET
{
	u_short index;
	VECTOR3 currentPosition;

	PACKET_ATTACK_MONSTER() : index(0), currentPosition({0,0,0}) {}
	PACKET_ATTACK_MONSTER(u_short _size, u_short _type, u_short _index, VECTOR3 _position) : PACKET(_size, _type), index(_index), currentPosition(_position) {}
};
struct PACKET_DIE_MONSTER : PACKET
{
	u_short index;
	u_short playerDamage;

	PACKET_DIE_MONSTER() : index(0), playerDamage(0) {}
	PACKET_DIE_MONSTER(u_short _size, u_short _type, u_short _index, u_short _damage) : PACKET(_size, _type), index(_index), playerDamage(_damage) {}
};
struct PACKET_REGEN_MONSTER : PACKET
{
	u_short index;
	u_short monsterType;
	u_short level;
	VECTOR3 position;

	PACKET_REGEN_MONSTER() : index(0), monsterType(0), level(0), position({ 0,0,0 }) {}
	PACKET_REGEN_MONSTER(u_short _size, u_short _type, u_short _index, u_short _monsterType, u_short _level, VECTOR3 _position) : PACKET(_size, _type), index(_index), monsterType(_monsterType), level(_level), position(_position) {}
};
struct PACKET_EXP : PACKET
{
	float exp;

	PACKET_EXP() : exp(0.0f) {}
	PACKET_EXP(u_short _size, u_short _type, float _exp) : PACKET(_size, _type), exp(_exp) {}
};
struct PACKET_PLAYER_HIT : PACKET
{
	u_short curHp;

	PACKET_PLAYER_HIT() : curHp(0) {}
	PACKET_PLAYER_HIT(u_short _size, u_short _type, u_short _curHp) : PACKET(_size, _type), curHp(_curHp) {}
};
struct PACKET_PLAYER_LEVEL_UP : PACKET
{
	u_short level;
	float curExp;
	float maxExp;

	PACKET_PLAYER_LEVEL_UP() : level(0), curExp(0), maxExp(0) {}
	PACKET_PLAYER_LEVEL_UP(u_short _size, u_short _type, u_short _level, u_short _curExp, u_short _maxExp) : PACKET(_size, _type), level(_level), curExp(_curExp), maxExp(_maxExp) {}
};
#pragma pack( pop )