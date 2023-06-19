#pragma once
#include "Monster.h"
#include "Navigation.h"
#include <vector>

class CMonsterManager
{
public:
	static CMonsterManager* GetInstance()
	{
		static CMonsterManager instance;
		return &instance;
	}

	typedef std::vector<CMonster*> monsterList_t;

private:
	CMonsterManager();
	~CMonsterManager();

private:
	monsterList_t m_monsterList;
	CNavigation* m_pNavigation;

public:
	void Start();
	void RunLoop();
};