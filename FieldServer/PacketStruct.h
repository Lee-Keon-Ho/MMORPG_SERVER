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
	u_short number;
	u_short character;
	u_short nameSize;
	char name[8];

	PACKET_NEW_LOGIN() : number(0), character(0), nameSize(0), name("") {}
	PACKET_NEW_LOGIN(u_short _size, u_short _type, u_short _number, u_short _character, u_short _nameSize, const char* _name)
		: PACKET(_size, _type), number(_number) , character(_character), nameSize(_nameSize)
	{
		memset(name, 0, 8);
		memcpy(name, _name, 8);
	}
};

struct PACKET_LOGIN : PACKET
{
	u_short number;

	PACKET_LOGIN() : number(0) {}
	PACKET_LOGIN(u_short _size, u_short _type, u_short _number)
		: PACKET(_size, _type), number(_number) {}
};

struct PACKET_NEWUSER : PACKET
{
	u_short number;
	u_short character;
	char name[9];
	VECTOR3 position;

	PACKET_NEWUSER() : number(0), character(0), position({0,0,0}), name("") {}
	PACKET_NEWUSER(u_short _size, u_short _type, u_short _number, u_short _character, const char* _name, VECTOR3 _position)
		: PACKET(_size, _type), number(_number), character(_character), position(_position) 
	{
		memset(name, 0, 9);
		memcpy(name, _name, 9);
	}
};

struct PACKET_NOWPOSITION : PACKET
{
	u_short number;
	VECTOR3 position;

	PACKET_NOWPOSITION() : number(0), position({ 0,0,0 }) {}
	PACKET_NOWPOSITION(u_short _size, u_short _type, u_short _number, VECTOR3 _position) : PACKET(_size, _type), number(_number), position(_position) {}
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
	u_short number;
	u_short sector;

	PACKET_SECTOR() : number(0), sector(0) {}
	PACKET_SECTOR(u_short _size, u_short _type, u_short _number, u_short _sector)
		: PACKET(_size, _type), number(_number), sector(_sector) {}
};

struct PACKET_MOVEUSER : PACKET_LOGIN
{
	VECTOR3 currentPosition;
	VECTOR3 endPosition;

	PACKET_MOVEUSER() : currentPosition({ 0,0,0 }), endPosition({ 0,0,0 }) {}
	PACKET_MOVEUSER(u_short _size, u_short _type, u_short _number, VECTOR3 _currentPosition, VECTOR3 _endPosition) :
		PACKET_LOGIN(_size, _type, _number), currentPosition(_currentPosition), endPosition(_endPosition) {}
};

struct PACKET_INSECTOR : PACKET_LOGIN
{
	VECTOR3 currentPosition;
	VECTOR3 endPosition;

	PACKET_INSECTOR() : currentPosition({ 0,0,0 }), endPosition({ 0,0,0 }) {}
	PACKET_INSECTOR(u_short _size, u_short _type, u_short _number, VECTOR3 _currentPosition, VECTOR3 _endPosition)
		: PACKET_LOGIN(_size, _type, _number), currentPosition(_currentPosition), endPosition(_endPosition) {}
};

struct PACKET_OUTSECTOR : PACKET
{
	u_short number;

	PACKET_OUTSECTOR() : number(0) {}
	PACKET_OUTSECTOR(u_short _size, u_short _type, u_short _number) :
		PACKET(_size, _type), number(_number) {}
};

struct PACKET_ARRIVE : PACKET
{
	u_short		number;
	float		rotationY;
	VECTOR3		endPosition;

	PACKET_ARRIVE() : number(0), rotationY(0), endPosition({ 0,0,0 }) {};
	PACKET_ARRIVE(u_short _size, u_short _type, u_short _number, float _rotationY, VECTOR3 _endPosition) :
		PACKET(_size, _type), number(_number), rotationY(_rotationY), endPosition(_endPosition) {}
};

struct PACKET_LOGOUT : PACKET
{
	u_short number;

	PACKET_LOGOUT() : number(0) {}
	PACKET_LOGOUT(u_short _size, u_short _type, u_short _number) :
		PACKET(_size, _type), number(_number) {}
};

#pragma pack( pop )