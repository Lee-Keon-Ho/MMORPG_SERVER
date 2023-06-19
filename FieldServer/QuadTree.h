#pragma once
#include <list>
#include "User.h"

class CQuadTree
{
private:
	typedef std::list<CUser*> userList_t;
private:
	int m_x;
	int m_z;

	userList_t m_users;

	CQuadTree* m_pNw;
	CQuadTree* m_pNe;
	CQuadTree* m_pSw;
	CQuadTree* m_pSe;
public:
	CQuadTree(int _x, int _z);
	~CQuadTree();

	void insert(int _x, int _z);
	bool Contains(CUser* _user);
};