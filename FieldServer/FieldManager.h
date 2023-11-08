#pragma once
#include "Map.h"

enum eField
{
	FOREST_TOWN = 0,
	FOREST_HUNT,
	WINTER_HUNT,
	MAX
};

class CFieldManager
{
public:
	static CFieldManager* GetInstance()
	{
		static CFieldManager instance;
		return &instance;
	}

private:
	CFieldManager();
	~CFieldManager();

private:
	CMap* m_pMap[MAX]; // ���߿� �̸� ���� ForestMap �����

public:
	CMap* GetMap(int _index);
};