#pragma once
#define GRID_SIZE 256

class CMapGrid
{
private:
	bool m_walkable[GRID_SIZE * GRID_SIZE];

public:
	CMapGrid();
	~CMapGrid();

	bool Init(const char* _fileName);
	bool* GetWalkable();
};