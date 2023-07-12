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
	u_short nameSize;
	char name[8];

	PACKET_NEW_LOGIN() : index(0), character(0), nameSize(0), name("") {}
	PACKET_NEW_LOGIN(u_short _size, u_short _type, u_short _index, u_short _character, u_short _nameSize, const char* _name)
		: PACKET(_size, _type), index(_index) , character(_character), nameSize(_nameSize)
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
	VECTOR3 currentPosition;
	VECTOR3 destinationPosition;

	PACKET_CREATE_MONSTER() : index(0), monsterType(0), currentPosition({ 0,0,0 }), destinationPosition({ 0,0,0 }) {}
	PACKET_CREATE_MONSTER(u_short _size, u_short _type, u_short _index, u_short _monsterType, VECTOR3 _currentPosition, VECTOR3 _destinationPosition) : PACKET(_size, _type), index(_index), monsterType(_monsterType), currentPosition(_currentPosition), destinationPosition(_destinationPosition) {}
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

	PACKET_ENTER_SECTOR_MONSTER() : index(0), monsterType(0), currentPosition({0,0,0}) {}
	PACKET_ENTER_SECTOR_MONSTER(u_short _size, u_short _type, u_short _index, u_short _monsterType, VECTOR3 _currentPosition) : PACKET(_size, _type), index(_index), monsterType(_monsterType), currentPosition(_currentPosition) {}
};
#pragma pack( pop )