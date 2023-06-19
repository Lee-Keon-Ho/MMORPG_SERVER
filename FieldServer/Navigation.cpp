#include "Navigation.h"

CNavigation::CNavigation()
{
}

CNavigation::CNavigation(const char* _fileName)
{
	m_pMapGrid = new CMapGrid();
	m_pMapGrid->Init(_fileName);
	m_pAStar = new CAStar();
}

CNavigation::~CNavigation()
{
	if (m_pMapGrid != nullptr) { delete m_pMapGrid; }
	if (m_pAStar != nullptr) { delete m_pAStar; m_pAStar = nullptr; }
}

void CNavigation::FindPath(VECTOR3 _start, VECTOR3 _goal)
{
	m_pAStar->Find(_start, _goal, m_pMapGrid->GetWalkable());
}