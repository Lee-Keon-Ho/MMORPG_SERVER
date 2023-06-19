#pragma once
#include "PriorityQueue.h"
#include "VECTOR.h"

#define ARRAY_MAX 3

using node_t = std::vector<CNode>;

class CAStar
{
private:
	CPriorityQueue* m_pOpenList;
	node_t m_closeList;
	std::vector<CNode*> m_tileGrid;
	int dx[ARRAY_MAX] = { -1,0,1 };
	int dy[ARRAY_MAX] = { 1,0,-1 };
	
public:
	CAStar();
	~CAStar();

	void Find(VECTOR3 _start, VECTOR3 _goal, bool* _pMap);
	bool SearchNode(CNode _node);
};