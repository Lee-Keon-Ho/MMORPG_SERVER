#pragma once
#include <memory>

constexpr int name_max = 14;

#pragma pack( push, 1)
struct sCharacterInfo
{
	wchar_t name[name_max];
	int type;
	int level;
	int curHp;
	int maxHp;
	int curMp;
	int maxMp;
	VECTOR3 position;
	int curExp;
	int damageMin;
	int damageMax;
	int field;

	sCharacterInfo() : type(0), level(0), curHp(0), maxHp(0), curMp(0), maxMp(0), position({ 0.0f, 0.0f, 0.0f }), curExp(0), damageMin(0), damageMax(0), field(0)
	{
		memset(name, 0, sizeof(name));
	}
};

struct sUserInfoNewSector
{
	u_short index;
	u_short state;
	float rotationY;
	VECTOR3 startPosition = {0.0f,0.0f,0.0f };
	VECTOR3 goalPosition = { 0.0f,0.0f,0.0f };
};

struct sMonsterInfoNewSector
{
	u_short index;
	u_short type;
	VECTOR3 startPosition = { 0.0f,0.0f,0.0f };
	VECTOR3 goalPosition = { 0.0f,0.0f,0.0f };
};

struct sInfield
{
	u_short index;
	u_short state;
	u_short character;
	float rotationY;
	wchar_t name[name_max];
	VECTOR3 startPosition = { 0.0f,0.0f,0.0f };
	VECTOR3 goalPosition = { 0.0f,0.0f,0.0f };
};
#pragma pack( pop )