#pragma once
#include "Navigation.h"
#include "Monster.h"

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