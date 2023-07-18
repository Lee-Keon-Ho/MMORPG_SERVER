#include "node.h"

CNode::CNode() : m_x(0), m_y(0), m_f(0), m_g(0), m_h(0), m_parent(nullptr)
{
}

CNode::CNode(unsigned int _x, unsigned int _y) : m_x(_x), m_y(_y), m_f(0), m_g(0), m_h(0), m_parent(nullptr)
{

}

CNode::~CNode()
{
}

void CNode::GetParent(std::vector<VECTOR3>& _vector)
{
	if (m_parent != nullptr) m_parent->GetParent(_vector);
	_vector.push_back(VECTOR3(static_cast<float>(m_x), 1, static_cast<float>(m_y)));
}

void CNode::SetNode(CNode _end, CNode* _parent, unsigned int _dest)
{
	int h = 0;
	unsigned int endX = _end.m_x;
	unsigned int endY = _end.m_y;

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

void CNode::SetParent(CNode* _pParent)
{
	m_parent = _pParent;
}
