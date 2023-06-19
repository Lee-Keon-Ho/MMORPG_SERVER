#pragma once
#include "AStar.h"
#include "MapGrid.h"

class CNavigation 
{
private:
	CMapGrid* m_pMapGrid;
	CAStar* m_pAStar;
public:
	CNavigation();
	CNavigation(const char* _fileName);
	~CNavigation();

	void FindPath(VECTOR3 _start, VECTOR3 _goal);
};