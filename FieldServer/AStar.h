#pragma once
#include "PriorityQueue.h"
#include <queue>
#include <utility>

using namespace std;
using Path = std::vector<VECTOR3>;

struct compare
{
	bool operator()(const CNode _nodeA, const CNode _nodeB)
	{
		return _nodeA.m_f > _nodeB.m_f;
	}
};

using node_t = vector<CNode>;
using PriorityQueue = priority_queue<CNode, node_t, compare>;

constexpr int array_max = 4;
constexpr int map_size_max = 256;

class CAStar
{
private:
	CPriorityQueue*			m_pOpenList;
	node_t					m_openList;
	node_t					m_closeList;
	std::vector<CNode*>		m_tileGrid;
	int						dx[array_max] = { 1,0,-1,0 };
	int						dy[array_max] = { 0,1,0,-1 };
	
public:
	CAStar();
	~CAStar();

	Path Find(VECTOR3& _start, VECTOR3& _goal, bool* _pMap);
	bool SearchNode(node_t _vector, CNode _node);
	int heuristic(const CNode& a, const CNode& b);
};