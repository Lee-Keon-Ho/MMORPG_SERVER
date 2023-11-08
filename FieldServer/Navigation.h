#pragma once
#include "Monster.h"
#include "AStar.h"
#include "MapGrid.h"
#include <deque>
#include <Windows.h>
#include "Job.h"

using namespace std;

class CNavigation 
{
public:
	typedef deque<unique_ptr<CJob>> t_que; // tQ

private:
	HANDLE m_thread;

	// 이 부분을 새로운 클래스를 만들어서
	// 필요한 내비로 찾아라 라는 식으로 만들면 훨씬 좋을 것 같다.
	CMapGrid* m_pMapGrid;
	CMapGrid* m_pWinterMapGrid;
	unique_ptr<CAStar> m_pAStar;

	t_que m_jobs;

	CRITICAL_SECTION m_spin_cs; // cs_spin 이름 변경, 새로 만들거나
public:
	CNavigation();
	~CNavigation();

	void FindPath(VECTOR3& _start, VECTOR3& _goal, Path* _pPath, bool* _walkable);

	void Start();
	static unsigned int _stdcall ThreadFunc(void* _pArgs);
	void RunLoop();
	void Add(VECTOR3& _startPos, VECTOR3& _endPos, Path* _path, bool* _walkable);
	bool* GetWalkable();
	bool* GetWinterWalkable();
	void SetWalkable(CMapGrid* _mapGrid, int _index);
};