#include "AStar.h"

CAStar::CAStar() : m_pOpenList(nullptr)
{
	m_pOpenList = new CPriorityQueue();
}

CAStar::~CAStar()
{
	if (m_pOpenList != nullptr) { delete m_pOpenList; m_pOpenList = nullptr; }
}

