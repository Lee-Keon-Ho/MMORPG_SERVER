#pragma once
#include "Monster.h"

class CMap;

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
	monsterList_t	m_monsterList;
	//CMap*			m_pMap;
public:
	void Start();
	void RunLoop();

	static unsigned int _stdcall MonsterManagerThreadFunc(void* _pArgs);

	bool Init(const char* _fileName);
	void CreateMonster();
	//void SetMap(CMap* _pMap);
};