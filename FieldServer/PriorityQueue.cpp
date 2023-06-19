#include "PriorityQueue.h"

CPriorityQueue::CPriorityQueue() : m_size(0)
{
	m_tileList.push_back(CNode());
}

CPriorityQueue::~CPriorityQueue()
{
}

void CPriorityQueue::Insert(CNode _node)
{
	int parentIndex;
	int index = ++m_size;

	m_tileList.push_back(_node);

	while (true)
	{
		if (index >> 1 == 0) break;
		parentIndex = index >> 1;

		if (m_tileList[index].m_f <= m_tileList[parentIndex].m_f)
		{
			Swap(&m_tileList[index], &m_tileList[parentIndex]);
		}
		index = parentIndex;
	}
}

bool CPriorityQueue::IsEmpty()
{
	return m_size == 0;
}

void CPriorityQueue::Swap(CNode* _pNodeA, CNode* _pNodeB)
{
	CNode temp = *_pNodeA;
	*_pNodeA = *_pNodeB;
	*_pNodeB = temp;
}

CNode CPriorityQueue::Top()
{
	return m_tileList[1];
}

void CPriorityQueue::Pop()
{
	int parentIndex;
	int size = --m_size;
	int index = 1;
	int childIndex;
	CNode* front = &m_tileList[1];
	CNode* back = &m_tileList.back();

	Swap(front, back);

	m_tileList.pop_back();

	while (true)
	{
		childIndex = index << 1;
		if (childIndex > size) break;
		else if (childIndex == size)
		{
			if (m_tileList[index].m_f >= m_tileList[childIndex].m_f)
			{
				Swap(&m_tileList[index], &m_tileList[childIndex]);
				index = childIndex << 1;
			}
			else break;
		}
		else
		{
			if (m_tileList[childIndex].m_f < m_tileList[childIndex + 1].m_f)
			{
				if (m_tileList[index].m_f >= m_tileList[childIndex].m_f)
				{
					Swap(&m_tileList[index], &m_tileList[childIndex]);
					index = childIndex << 1;
				}
				else break;
			}
			else
			{
				if (m_tileList[index].m_f >= m_tileList[childIndex + 1].m_f)
				{
					Swap(&m_tileList[index], &m_tileList[childIndex + 1]);
					index = (childIndex << 1) + 1;
				}
				else break;
			}
		}
	}
}

CNode* CPriorityQueue::Find(CNode _node)
{
	for (CNode node : m_tileList)
	{
		if (node.m_x == _node.m_x && node.m_y == _node.m_y) return &node;
	}

	return nullptr;
}

int CPriorityQueue::Size()
{
	return m_size;
}