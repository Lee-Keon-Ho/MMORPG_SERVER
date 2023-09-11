#pragma once
#include "PriorityQueue.h"
#include <queue>
#include <utility>

using namespace std;

#define ARRAY_MAX 3

struct compare
{
	bool operator()(const CNode _nodeA, const CNode _nodeB)
	{
		return _nodeA.m_f > _nodeB.m_f;
	}
};

using node_t = vector<CNode>;
using PriorityQueue = priority_queue<CNode, node_t, compare>;

class CAStar
{
private:
	CPriorityQueue*			m_pOpenList;
	node_t					m_openList;
	node_t					m_closeList;
	std::vector<CNode*>		m_tileGrid;
	int						dx[ARRAY_MAX] = { -1,0,1 };
	int						dy[ARRAY_MAX] = { 1,0,-1 };
	
public:
	CAStar();
	~CAStar();

	std::vector<VECTOR3> Find(VECTOR3& _start, VECTOR3& _goal, bool* _pMap);
	bool SearchNode(node_t _vector, CNode _node);
};