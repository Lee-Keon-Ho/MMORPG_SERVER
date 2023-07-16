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
	return m_pMap; // 여기도 필요한 인덱스를 넣어주면 반환하게 수정 필요
}