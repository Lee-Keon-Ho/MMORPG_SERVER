#include "FieldManager.h"

CFieldManager::CFieldManager() : m_pMap(nullptr)
{
	if (m_pMap == nullptr) m_pMap = new CMap();
}

CFieldManager::~CFieldManager()
{
}

CMap* CFieldManager::GetMap(int _index)
{
	return m_pMap; // ���⵵ �ʿ��� �ε����� �־��ָ� ��ȯ�ϰ� ���� �ʿ�
}