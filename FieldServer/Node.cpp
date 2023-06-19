#include "node.h"

CNode::CNode() : m_x(0), m_y(0), m_f(0), m_g(0), m_h(0), m_parent(nullptr)
{
}

CNode::CNode(u_int _x, u_int _y) : m_x(_x), m_y(_y), m_f(0), m_g(0), m_h(0), m_parent(nullptr)
{

}

CNode::~CNode()
{
}

CNode* CNode::GetParent()
{
	return m_parent;
}

void CNode::SetNode(CNode _end, CNode* _parent, u_int _dest)
{
	int h = 0;
	u_int endX = _end.m_x;
	u_int endY = _end.m_y;

	if (m_x > endX) h += (m_x - endX) * 10;
	else h += (endX - m_x) * 10;
	if (m_y > endY) h += (m_y - endY) * 10;
	else h += (endY - m_y) * 10;

	if (m_g == 0) m_g = _dest + _parent->m_g;
	else if (m_g > _dest + _parent->m_g) m_g = _dest + _parent->m_g;

	m_h = h;
	m_f = m_g + m_h;

	m_parent = _parent;
}
