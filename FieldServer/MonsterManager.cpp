#include "Monster.h"
#include "UserManager.h"
#include "MonsterManager.h"
#include "CBigSlimeLeaf.h"
#include "FieldManager.h"
#include <process.h>
#include <chrono>

// 전체 몬스터를 관리할 것이냐, 맵의 몬스터만 관리할 것인가
CMonsterManager::CMonsterManager() : m_pNavigation(nullptr)
{
	Init("ForestMonster1.bin"); // map
	Init("ForestMonster2.bin");
	Init("ForestMonster2_1.bin");
	Init("ForestMonster3.bin");
	Init("ForestMonster3_1.bin");
	Init("ForestMonster3_2.bin");
	Init("ForestMonster3_3.bin");
	Init("ForestMonster4_1.bin");
	Init("ForestMonster4_2.bin");
	Init("ForestMonster4_3.bin");
	Init("ForestMonster4_4.bin");
	Init("WinterMonster.bin");

	CFieldManager* FM = CFieldManager::GetInstance();

	if (m_pNavigation == nullptr)
	{
		m_pNavigation = new CNavigation();
		for (int i = 1; i < MAX; i++)
		{
			m_pNavigation->SetWalkable(FM->GetMap(i)->GetMapGrid(), i);
		}
	}
}

CMonsterManager::~CMonsterManager()
{
	if (m_pNavigation) { delete m_pNavigation; m_pNavigation = nullptr; }
}

void CMonsterManager::Start()
{
	m_pNavigation->Start();
	m_thread = (HANDLE)_beginthreadex(NULL, 0, &CMonsterManager::MonsterManagerThreadFunc, this, 0, NULL);
}

unsigned int _stdcall CMonsterManager::MonsterManagerThreadFunc(void* _pArgs)
{
	CMonsterManager* pMonsterManager = (CMonsterManager*)_pArgs;
	pMonsterManager->RunLoop();
	return 0;
}

bool CMonsterManager::Init(const char* _fileName)
{
	FILE* file;

	errno_t error = fopen_s(&file, _fileName, "rb"); // c++함수로 교체

	if (error != 0) return false;

	int size;
	int type;
	VECTOR3 vector3({ 0.0f,0.0f,0.0f });
	VECTOR2_INT rangeMin({ 0,0 });
	VECTOR2_INT rangeMax({ 0,0 });

	fread(&size, sizeof(int), 1, file);
	fread(&type, sizeof(int), 1, file);
	fread(&rangeMin, sizeof(VECTOR2_INT), 1, file);
	fread(&rangeMax, sizeof(VECTOR2_INT), 1, file);

	size += static_cast<int>(m_monsterList.size());

	for (int i = static_cast<int>(m_monsterList.size()); i < size; i++)
	{
		fread(&vector3, sizeof(VECTOR3), 1, file);
		CMonster* pMonster;

		if (type == 1)
		{
			pMonster = new CBigSlimeLeaf(vector3, rangeMin, rangeMax, type, i);
		}
		else
		{
			pMonster = new CMonster(vector3, rangeMin, rangeMax, type, i);
		}
		m_monsterList.push_back(pMonster);
	}

	fclose(file);

	return true;
}

void CMonsterManager::Hit(CUser* _pTarget, int _index)
{
	m_monsterList[_index]->Hit(_pTarget);
}

HANDLE CMonsterManager::GetHandle()
{
	return m_thread;
}

void CMonsterManager::RunLoop()
{
	DWORD prevTick = ::timeGetTime();
	DWORD nowTick;
	DWORD deltaTick;

	while (true)
	{
		Sleep(200);

		nowTick = ::timeGetTime();

		deltaTick = nowTick - prevTick;

		prevTick = nowTick;

		for (CMonster* m : m_monsterList)
		{
			m->Update(deltaTick * 0.001f, m_pNavigation); // 확인해야 한다
		}
	}
}
