#include "UserManager.h"
#include "MonsterManager.h"
#include "Navigation.h"
#include <process.h>
#include <chrono>

CMonsterManager::CMonsterManager()
{
	Init("ForestMonster1.bin"); // 이걸 유니티에서 한번에 작업해서 만드는게 더 좋아보이기는 한데...
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
	Init("ForestMonster4_5.bin");
}

CMonsterManager::~CMonsterManager()
{
}

void CMonsterManager::Start()
{

	HANDLE thread = (HANDLE)_beginthreadex(NULL, 0, &CMonsterManager::MonsterManagerThreadFunc, this, 0, NULL);
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

	errno_t error = fopen_s(&file, _fileName, "rb");

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
		m_monsterList.push_back(new CMonster(vector3, rangeMin, rangeMax, type, i));
	}

	fclose(file);

	return true;
}

void CMonsterManager::CreateMonster()
{
	for (CMonster* m : m_monsterList)
	{
		m->SendPacketCreate();
	}
}

void CMonsterManager::RunLoop()
{
	DWORD prevTick = ::timeGetTime();
	DWORD nowTick;
	DWORD deltaTick;
	CNavigation* navi = CNavigation::GetInstance();
	bool* walkable = navi->GetWalkable();

	while (true)
	{
		Sleep(100);

		nowTick = ::timeGetTime();

		deltaTick = nowTick - prevTick;

		prevTick = nowTick;

		for (CMonster* m : m_monsterList)
		{
			m->Move(deltaTick * static_cast<float>(0.001));
		}

		for (CMonster* m : m_monsterList)
		{
			if (!m->CheckMovement()) continue;

			if (m->SetNextDestination(walkable))
			{
				m->SendPacketMove();
			}
		}
	}
}
