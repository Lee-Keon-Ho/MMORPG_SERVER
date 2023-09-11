#include "Navigation.h"

CNavigation::CNavigation()
{
	m_pMapGrid = new CMapGrid();
	m_pAStar = new CAStar();
	m_pMapGrid->Init("map.bin");
}

CNavigation::~CNavigation()
{
	if (m_pMapGrid != nullptr) { delete m_pMapGrid; }
	if (m_pAStar != nullptr) { delete m_pAStar; m_pAStar = nullptr; }
}

std::vector<VECTOR3> CNavigation::FindPath(VECTOR3& _start, VECTOR3& _goal)
{
	return m_pAStar->Find(_start, _goal, m_pMapGrid->GetWalkable());
}

bool* CNavigation::GetWalkable()
{
	return m_pMapGrid->GetWalkable();
}
