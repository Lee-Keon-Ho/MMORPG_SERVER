#include <Windows.h>
#include "MonsterManager.h"

CMonsterManager::CMonsterManager()
{
	//m_monsterList.push_back(new CMonster);
	m_pNavigation = new CNavigation();
}

CMonsterManager::~CMonsterManager()
{
	if (m_pNavigation != nullptr) { delete m_pNavigation; m_pNavigation = nullptr; };
}

void CMonsterManager::Start()
{
	// �ٸ� ������ �� �ϰ�
	// thread�� ����ؼ� RunLoop();
}

void CMonsterManager::RunLoop()
{
	while (true)
	{
		//(*m_monsterList.begin())->Move(m_pMapGrid);
		Sleep(1);
	}
}
