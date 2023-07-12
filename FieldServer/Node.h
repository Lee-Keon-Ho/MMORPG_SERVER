#pragma once
#include "VECTOR.h"
#include <vector>

class CNode
{
public:
	unsigned int m_x;
	unsigned int m_y;

	unsigned int m_f; // total cost
	unsigned int m_g; // cost from start
	unsigned int m_h; // cost to end

	CNode* m_parent;
public:
	CNode();
	CNode(unsigned int _x, unsigned int _y);
	~CNode();

	void GetParent(std::vector<VECTOR3>& _vector);

	void SetNode(CNode _pEnd, CNode* _pParent, unsigned int _dest);
	void SetParent(CNode* _pParent);
};