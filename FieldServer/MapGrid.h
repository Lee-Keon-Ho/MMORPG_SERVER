#pragma once

constexpr int grid_size = 256;

class CMapGrid
{
private:
	bool m_walkable[grid_size * grid_size];

public:
	CMapGrid();
	CMapGrid(const char* _fileName);
	~CMapGrid();

	bool* GetWalkable();
};