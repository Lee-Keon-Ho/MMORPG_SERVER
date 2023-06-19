#pragma once
#include "node.h"
#include <vector>

using node_t = std::vector<CNode>;

class CPriorityQueue
{
private:
	node_t m_tileList;
	int m_size;
public:
	CPriorityQueue();
	~CPriorityQueue();

	void Insert(CNode _node);
	bool IsEmpty();
	void Swap(CNode* _pNodeA, CNode* _pNodeB);
	CNode Top();
	void Pop();
	CNode* Find(CNode _pNode);
	int Size();
};