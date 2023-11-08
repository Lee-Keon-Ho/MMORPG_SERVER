#include "MapGrid.h"
#include <stdio.h>
#include <stdexcept>
CMapGrid::CMapGrid()
{
	memset(m_walkable, 0, GRID_SIZE * GRID_SIZE);
}

CMapGrid::CMapGrid(const char* _fileName)
{
	FILE* file;

	errno_t error = fopen_s(&file, _fileName, "rb");

	if (error != 0) throw std::runtime_error("fopen error\n");

	fread(&m_walkable, 256 * 256, 1, file);

	fclose(file);
}

CMapGrid::~CMapGrid()
{
}

bool* CMapGrid::GetWalkable()
{
	return m_walkable;
}
