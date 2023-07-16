#pragma once
#include "Map.h"

enum eField
{
	FOREST_TOWN = 0,
	FOREST_HUNT
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
	CMap* m_pMap; // 나중에 이름 변경 ForestMap 사냥터

public:
	CMap* GetMap(int _index);
};