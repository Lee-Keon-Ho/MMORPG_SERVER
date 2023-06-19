#pragma once
#include "MapGrid.h"
#include "AStar.h"

class CNavigation 
{
private:
	CMapGrid* m_pMapGrid;
	CAStar* m_pAStar;
public:
	CNavigation();
	CNavigation(const char* _fileName);
	~CNavigation();

	void FindPath();
};