#include "MapGrid.h"
#include <stdio.h>

CMapGrid::CMapGrid()
{
}

CMapGrid::~CMapGrid()
{
}

bool CMapGrid::Init(const char* _fileName) // read map file
{
	FILE* file;

	errno_t error = fopen_s(&file, _fileName, "rb");

	if (error != 0) return false;

	fread(&m_walkable, 256 * 256, 1, file);

	fclose(file);

	return true;
}

bool* CMapGrid::GetWalkable()
{
	return m_walkable;
}
