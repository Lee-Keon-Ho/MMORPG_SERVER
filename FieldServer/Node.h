#pragma once
#include <Windows.h>

class CNode
{
public:
	u_int m_x;
	u_int m_y;

	u_int m_f; // total cost
	u_int m_g; // cost from start
	u_int m_h; // cost to end

	CNode* m_parent;
public:
	CNode();
	CNode(u_int _x, u_int _y);
	~CNode();

	CNode* GetParent(); // delete

	void SetNode(CNode _pEnd, CNode* _pParent, u_int _dest);
};