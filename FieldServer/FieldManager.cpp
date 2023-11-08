#include "FieldManager.h"
#include "Navigation.h"
#include <stdexcept>
CFieldManager::CFieldManager()
{
	try
	{
		m_pMap[0] = new CMap();
		m_pMap[1] = new CMap("map.bin");
		m_pMap[2] = new CMap("WinterMap.bin");
	}
	catch (runtime_error e)
	{
		std::cout << e.what() << endl;
	}
}

CFieldManager::~CFieldManager()
{
	for (int i = 0; i < MAX; i++)
	{
		delete m_pMap[i];
	}
}

CMap* CFieldManager::GetMap(int _index)
{
	return m_pMap[_index];
}