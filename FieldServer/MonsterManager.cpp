#include <Windows.h>
#include "MonsterManager.h"

CMonsterManager::CMonsterManager()
{
	//m_monsterList.push_back(new CMonster);
	m_pNavigation = new CNavigation("map.bin");
}

CMonsterManager::~CMonsterManager()
{
	if (m_pNavigation != nullptr) { delete m_pNavigation; m_pNavigation = nullptr; };
}

void CMonsterManager::Start()
{
	VECTOR3 start(1, 0, 1);
	VECTOR3 goal(28, 0, 1);
	m_pNavigation->FindPath(start, goal);
	// 다른 세팅을 더 하고
	// thread를 사용해서 RunLoop();
}

void CMonsterManager::RunLoop()
{
	while (true)
	{
		//(*m_monsterList.begin())->Move(m_pMapGrid);
		Sleep(1);
	}
}
