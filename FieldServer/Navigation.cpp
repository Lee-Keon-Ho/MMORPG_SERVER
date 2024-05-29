#include "Navigation.h"
#include "../NetCore/Lock.h"
#include <process.h>

using namespace std;

CNavigation::CNavigation() : 
	m_thread(nullptr) ,
	m_pMapGrid(nullptr), 
	m_pWinterMapGrid(nullptr),
	m_lock(new SRWLOCK())
{
	InitializeSRWLock(m_lock);
	m_pAStar = make_unique<CAStar>();
}

CNavigation::~CNavigation()
{
	if (m_lock) { delete m_lock; m_lock = nullptr; }
}

void CNavigation::FindPath(VECTOR3& _start, VECTOR3& _goal, Path* _pPath, bool* _walkable)
{
	*_pPath = m_pAStar.get()->Find(_start, _goal, _walkable);
}

void CNavigation::Start()
{
	m_thread = (HANDLE)_beginthreadex(NULL, 0, &CNavigation::ThreadFunc, this, 0, NULL);
}

unsigned int _stdcall CNavigation::ThreadFunc(void* _pArgs)
{
	CNavigation* pMonsterManager = (CNavigation*)_pArgs;
	pMonsterManager->RunLoop();
	return 0;
}

void CNavigation::RunLoop()
{
	while (true)
	{
		if (m_jobs.size() > 0)
		{
			unique_ptr<CJob> pJob;

			{
				CLock lock(m_lock, eLockType::EXCLUSIVE);
				pJob = move(m_jobs.front());
				m_jobs.pop_front();
			}

			FindPath(pJob.get()->GetStartPos(), pJob.get()->GetEndPos(), pJob.get()->GetPath(), pJob.get()->Getwalkable());
		}
		Sleep(1);
	}
}

void CNavigation::Add(VECTOR3& _startPos, VECTOR3& _endPos, Path* _path, bool* _walkable)
{
	CLock lock(m_lock, eLockType::EXCLUSIVE);
	m_jobs.push_back(make_unique<CJob>(_startPos, _endPos, _path, _walkable));
}

bool* CNavigation::GetWalkable()
{
	return m_pMapGrid->GetWalkable();
}

bool* CNavigation::GetWinterWalkable()
{
	return m_pWinterMapGrid->GetWalkable();
}

void CNavigation::SetWalkable(CMapGrid* _mapGrid, int _index)
{
	if (_index == 1) m_pMapGrid = _mapGrid;
	else if (_index == 2) m_pWinterMapGrid = _mapGrid;
}