#pragma once
#include "VECTOR.h"
#include <vector>

class CNode
{
public:
	int m_x;
	int m_y;

	int m_f; // total cost
	int m_g; // cost from start
	int m_h; // cost to end

	CNode* m_parent;
public:
	CNode();
	CNode(int _x, int _y);
	~CNode();

	void GetParent(std::vector<VECTOR3>& _vector);

	void SetNode(CNode _pEnd, CNode* _pParent, int _dest);
	void SetParent(CNode* _pParent);
    int getF() const {
        return m_g + m_h;
    }
};