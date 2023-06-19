#include "QuadTree.h"

CQuadTree::CQuadTree(int _x, int _z) : m_x(_x), m_z(_z),
	m_pNw(nullptr), m_pNe(nullptr), m_pSw(nullptr), m_pSe(nullptr)
{
	
}

CQuadTree::~CQuadTree()
{
}

void CQuadTree::insert(int _x, int _z)
{
	if (_x < m_x && _z > m_z)
	{
		if (m_pNw == nullptr) m_pNw = new CQuadTree(_x, _z);
		else m_pNw->insert(_x, _z);
	}
	if (_x > m_x && _z > m_z)
	{
		if (m_pNe == nullptr) m_pNe = new CQuadTree(_x, _z);
		else m_pNe->insert(_x, _z);
	}
	if (_x < m_x && _z < m_z)
	{
		if (m_pSw == nullptr) m_pSw = new CQuadTree(_x, _z);
		else m_pSw->insert(_x, _z);
	}
	if (_x > m_x && _z < m_z)
	{
		if (m_pSe == nullptr) m_pSe = new CQuadTree(_x, _z);
		else m_pSe->insert(_x, _z);
	}
}

bool CQuadTree::Contains(CUser* _user)
{
	if(_user->GetPosition()->x)
	return false;

}
