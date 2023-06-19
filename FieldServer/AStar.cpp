#include "AStar.h"

#define MAP_SIZE_MAX 256

CAStar::CAStar() : m_pOpenList(nullptr)
{
	m_pOpenList = new CPriorityQueue();
	for (int y = 0; y < MAP_SIZE_MAX; y++)
	{
		for (int x = 0; x < MAP_SIZE_MAX; x++)
		{
			m_tileGrid.push_back(new CNode(x,y));
		}
	}
}

CAStar::~CAStar()
{
	if (m_pOpenList != nullptr) { delete m_pOpenList; m_pOpenList = nullptr; }
}

void CAStar::Find(VECTOR3 _start, VECTOR3 _goal, bool* _pMap)
{
	CNode startNode(static_cast<unsigned int>(_start.x), static_cast<unsigned int>(_start.z));
	CNode goalNode(static_cast<unsigned int>(_goal.x), static_cast<unsigned int>(_goal.z));
	CNode current;
	CNode* pNode;
	unsigned int nextX = 0;
	unsigned int nextY = 0;
	int priority = 0;

	bool loop = true;

	m_pOpenList->Insert(startNode);

	while (loop)
	{
		current = m_pOpenList->Top();

		m_pOpenList->Pop();

		m_closeList.push_back(current);

		for (int y = 0; y < ARRAY_MAX; y++)
		{
			for (int x = 0; x < ARRAY_MAX; x++)
			{
				nextX = current.m_x + dx[x];
				nextY = current.m_y + dy[y];

				if (nextX < 0 || nextX >= MAP_SIZE_MAX || nextY < 0 || nextY >= MAP_SIZE_MAX) continue;
				if (dx[x] == 0 || dy[y] == 0) priority = 10;
				else priority = 14;

				pNode = m_tileGrid[nextY * MAP_SIZE_MAX + nextX];
				if (!SearchNode(*pNode) && _pMap[nextY * MAP_SIZE_MAX + nextX])
				{
					if (!m_pOpenList->Find(*pNode))
					{
						pNode->SetNode(goalNode, m_tileGrid[current.m_y * MAP_SIZE_MAX + current.m_x], priority);
						m_pOpenList->Insert(*pNode);
					}
					else continue;
				}

				if (pNode->m_x == static_cast<unsigned int>(_goal.x) && pNode->m_y == static_cast<unsigned int>(_goal.z))
				{
					loop = false;
				}
			}
		}


	}
}

bool CAStar::SearchNode(CNode _node)
{
	for (CNode node : m_closeList)
	{
		if (node.m_x == _node.m_x && node.m_y == _node.m_y) return true;
	}

	return false;
}

