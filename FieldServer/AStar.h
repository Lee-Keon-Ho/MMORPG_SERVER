#pragma once
#include "PriorityQueue.h"
using node_t = std::vector<CNode>;

class CAStar
{
private:
	CPriorityQueue* m_pOpenList;
public:
	CAStar();
	~CAStar();
};