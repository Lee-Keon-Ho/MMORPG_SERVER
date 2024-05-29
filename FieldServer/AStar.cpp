#include "AStar.h"

CAStar::CAStar() : m_pOpenList(nullptr)
{
	m_pOpenList = new CPriorityQueue();
	for (int y = 0; y < map_size_max; y++)
	{
		for (int x = 0; x < map_size_max; x++)
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
	CNode* startNode = new CNode(static_cast<int>(_start.x), static_cast<int>(_start.z));
	CNode* goalNode = new CNode(static_cast<int>(_goal.x), static_cast<int>(_goal.z));
	CNode current;

	std::priority_queue<std::pair<int, CNode*>> openSet;
	std::vector<CNode*> closedSet;

	startNode->m_h = heuristic(*startNode, *goalNode);
	openSet.push({-startNode->getF(), startNode});

	while (!openSet.empty())
	{
		if (openSet.size() > 100) return path;

		CNode* current = openSet.top().second;
		openSet.pop();

		if (current->m_x == goalNode->m_x && current->m_y == goalNode->m_y)
		{
			while (current)
			{
				path.push_back(VECTOR3(current->m_x, 1, current->m_y)); //static_cast
				current = current->m_parent;
			}
			std::reverse(path.begin(), path.end());
			break;
		}

		closedSet.push_back(current);

		for (int i = 0; i < array_max; ++i)
		{
			int nx = current->m_x + dx[i];
			int ny = current->m_y + dy[i];

			if (nx < 0 || ny < 0 || nx >= map_size_max || ny >= map_size_max)
			{
				continue;
			}

			if (!_pMap[ny * map_size_max + nx]) continue;

			CNode* neighbor = new CNode(nx, ny);
			neighbor->m_g = current->m_g + 1;
			neighbor->m_h = heuristic(*neighbor, *goalNode);
			neighbor->m_parent = current;

			auto it = std::find_if(closedSet.begin(), closedSet.end(),
				[neighbor](const CNode* node)
				{
					return node->m_x == neighbor->m_x && node->m_y == neighbor->m_y;
				});
			if (it != closedSet.end()) 
			{
				delete neighbor;
				continue;
			}

			openSet.push({ -neighbor->getF(), neighbor });
		}


		//for (int y : dy)
		//{
		//	for (int x : dx)
		//	{
		//		nextX = current.m_x + x;
		//		nextY = current.m_y + y;

		//		if (nextX < 0 || nextX >= map_size_max || nextY < 0 || nextY >= map_size_max) continue;
		//		if (x == 0 || y == 0) priority = 10;
		//		else priority = 14;

		//		pNode = m_tileGrid[nextY * map_size_max + nextX];
		//		
		//		if (!SearchNode(m_closeList, *pNode) && _pMap[nextY * map_size_max + nextX])
		//		{
		//			if (!SearchNode(m_openList, *pNode))
		//			{
		//				pNode->SetNode(goalNode, m_tileGrid[current.m_y * map_size_max + current.m_x], priority);
		//				priorityQueue.push(*pNode);
		//				m_openList.push_back(*pNode);
		//			}
		//			else continue;
		//		}

		//		if (pNode->m_x == static_cast<int>(_goal.x) && pNode->m_y == static_cast<int>(_goal.z))
		//		{
		//			vector<VECTOR3> temp;
		//			pNode->GetParent(temp);


		//			// 0 = x, 1 = y, 2 = xy
		//			int va = -1;

		//			VECTOR3 vector({ 0,0,0 });
		//			VECTOR3 endVector = *(temp.end() - 1);
		//			for (VECTOR3 v : temp) // begin은 무조건 들어간다
		//			{
		//				if (v.x == endVector.x && v.z == endVector.z)
		//				{
		//					path.push_back(v);
		//					break;
		//				}

		//				if (vector.x != v.x && vector.z != v.z)
		//				{
		//					if (va != 2)
		//					{
		//						va = 2;
		//						path.push_back(v);
		//					}
		//				}
		//				else if (vector.x != v.x)
		//				{
		//					if (va != 0)
		//					{
		//						va = 0;
		//						path.push_back(v);
		//					}
		//				}
		//				else if (vector.z != v.z)
		//				{
		//					if (va != 1)
		//					{
		//						va = 1;
		//						path.push_back(v);
		//					}
		//				}
		//				vector = v;
		//			}

		//			m_openList.clear();
		//			m_closeList.clear();

		//			if (path.size() == 0)
		//			{
		//				printf("No Find\n");
		//			}
		//			return path;
		//		}
		//	}
		//}
	}

	for (CNode* node : closedSet)
	{
		delete node;
	}

	return path;
}

bool CAStar::SearchNode(node_t _vector, CNode _node)
{
	for (CNode node : _vector)
	{
		if (node.m_x == _node.m_x && node.m_y == _node.m_y) return true;
	}

	return false;
}

int CAStar::heuristic(const CNode& a, const CNode& b)
{
	return std::abs(static_cast<int>(a.m_x - b.m_x)) + std::abs(static_cast<int>(a.m_y - b.m_y));
}

