#pragma once
#include "AStar.h"
#include "MapGrid.h"

class CNavigation 
{
public:
	static CNavigation* GetInstance()
	{
		static CNavigation instance;
		return &instance;
	}
private:
	CNavigation();
	~CNavigation();

private:
	CMapGrid* m_pMapGrid;
	CAStar* m_pAStar;

public:
	std::vector<VECTOR3> FindPath(VECTOR3 _start, VECTOR3 _goal);
	bool* GetWalkable();
};