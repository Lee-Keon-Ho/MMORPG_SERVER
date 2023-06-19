#pragma once
#include "MapGrid.h"
#include <stack>
class CMonster
{

private:
	bool* m_walkable;

public:
	CMonster();
	~CMonster();

	void Move(CMapGrid* _pGrid);
};

