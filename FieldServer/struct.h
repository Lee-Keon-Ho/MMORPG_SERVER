#pragma once
#include <memory>

struct sCharacterInfo
{
	wchar_t name[16];
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

	sCharacterInfo() : type(0), level(0), curHp(0), maxHp(0), curMp(0), maxMp(0), position({ 0.0f, 0.0f, 0.0f }), curExp(0), damageMin(0), damageMax(0)
	{
		memset(name, 0, sizeof(name));
	}
};