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
	if (m_tileGrid.size() > 0) { m_tileGrid.clear(); }
}

Path CAStar::Find(VECTOR3& _start, VECTOR3& _goal, bool* _pMap)
{
	PriorityQueue priorityQueue;
	Path path;
	CNode startNode(static_cast<unsigned int>(_start.x), static_cast<unsigned int>(_start.z));
	CNode goalNode(static_cast<unsigned int>(_goal.x), static_cast<unsigned int>(_goal.z));
	CNode current;
	CNode* pNode;
	unsigned int nextX = 0;
	unsigned int nextY = 0;
	int priority = 0;

	for (CNode* node : m_tileGrid)
	{
		node->SetParent(nullptr);
	}

	priorityQueue.push(startNode);
	m_openList.push_back(startNode);

	while (!priorityQueue.empty())
	{
		if (priorityQueue.size() > 100) return path;

		current = priorityQueue.top();

		priorityQueue.pop();

		m_closeList.push_back(current);

		for (int y : dy)
		{
			for (int x : dx)
			{
				nextX = current.m_x + x;
				nextY = current.m_y + y;

				if (nextX < 0 || nextX >= MAP_SIZE_MAX || nextY < 0 || nextY >= MAP_SIZE_MAX) continue;
				if (x == 0 || y == 0) priority = 10;
				else priority = 14;

				pNode = m_tileGrid[nextY * MAP_SIZE_MAX + nextX];
				
				if (!SearchNode(m_closeList, *pNode) && _pMap[nextY * MAP_SIZE_MAX + nextX])
				{
					if (!SearchNode(m_openList, *pNode))
					{
						pNode->SetNode(goalNode, m_tileGrid[current.m_y * MAP_SIZE_MAX + current.m_x], priority);
						priorityQueue.push(*pNode);
						m_openList.push_back(*pNode);
					}
					else continue;
				}

				if (pNode->m_x == static_cast<unsigned int>(_goal.x) && pNode->m_y == static_cast<unsigned int>(_goal.z))
				{
					vector<VECTOR3> temp;
					pNode->GetParent(temp);


					// 0 = x, 1 = y, 2 = xy
					int va = -1;

					VECTOR3 vector({ 0,0,0 });
					VECTOR3 endVector = *(temp.end() - 1);
					for (VECTOR3 v : temp) // begin은 무조건 들어간다
					{
						if (v.x == endVector.x && v.z == endVector.z)
						{
							path.push_back(v);
							break;
						}

						if (vector.x != v.x && vector.z != v.z)
						{
							if (va != 2)
							{
								va = 2;
								path.push_back(v);
							}
						}
						else if (vector.x != v.x)
						{
							if (va != 0)
							{
								va = 0;
								path.push_back(v);
							}
						}
						else if (vector.z != v.z)
						{
							if (va != 1)
							{
								va = 1;
								path.push_back(v);
							}
						}
						vector = v;
					}

					m_openList.clear();
					m_closeList.clear();

					if (path.size() == 0)
					{
						printf("No Find\n");
					}
					return path;
				}
			}
		}
	}

	return path;
	/////////////////////////////////////////////

	/*m_pOpenList->Insert(startNode);

	while (!m_pOpenList->IsEmpty())
	{
		current = m_pOpenList->Top();

		m_pOpenList->Pop();

		m_closeList.push_back(current);

		for (int y : dy)
		{
			for (int x : dx)
			{
				nextX = current.m_x + x;
				nextY = current.m_y + y;

				if (nextX < 0 || nextX >= MAP_SIZE_MAX || nextY < 0 || nextY >= MAP_SIZE_MAX) continue;
				if (x == 0 || y == 0) priority = 10;
				else priority = 14;

				pNode = m_tileGrid[nextY * MAP_SIZE_MAX + nextX];
				if (!SearchNode(m_closeList, *pNode) && _pMap[nextY * MAP_SIZE_MAX + nextX])
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
					return;
				}
			}
		}
	}*/
}

bool CAStar::SearchNode(node_t _vector, CNode _node)
{
	for (CNode node : _vector)
	{
		if (node.m_x == _node.m_x && node.m_y == _node.m_y) return true;
	}

	return false;
}

