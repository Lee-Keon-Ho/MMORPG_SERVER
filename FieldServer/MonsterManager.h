#pragma once
#include "Navigation.h"

class CMonster;

class CMonsterManager
{
public:
	static CMonsterManager* GetInstance()
	{
		static CMonsterManager instance;
		return &instance;
	}

	typedef std::vector<CMonster*> monsterList_t; // using으로 작업하고

private:
	CMonsterManager();
	~CMonsterManager();

private:
	monsterList_t	m_monsterList;
	CNavigation*	m_pNavigation;
	HANDLE			m_thread;
public:
	void Start();
	void RunLoop();

	static unsigned int _stdcall MonsterManagerThreadFunc(void* _pArgs);

	bool Init(const char* _fileName);
	void Hit(CUser* _pTarget, int _index);
	HANDLE GetHandle();
};