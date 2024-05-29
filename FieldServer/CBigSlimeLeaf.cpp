#include "CBigSlimeLeaf.h"
#include "Sector.h"
#include "FieldManager.h"
#include "Navigation.h"

#define DISTANCE 100.0f

CBigSlimeLeaf::CBigSlimeLeaf() : CMonster()
{
	
}

CBigSlimeLeaf::CBigSlimeLeaf(VECTOR3 _position, VECTOR2_INT _rangeMin, VECTOR2_INT _rangeMax, int _type, int _index) : CMonster(_position, _rangeMin, _rangeMax, _type, _index)
{

}

CBigSlimeLeaf::~CBigSlimeLeaf()
{
}

void CBigSlimeLeaf::Update(float _deltaTick, CNavigation* _pNavi)
{
	switch (m_state)
	{
	case IDLE:
		Idle(_deltaTick, _pNavi); // _deltaTick
		break;
	case RUN:
		Run(_deltaTick, _pNavi);
		break;
	case HIT:
		Hit(_deltaTick, _pNavi);
		break;
	case TARGET_RUN:
		TargetRun(_deltaTick, _pNavi);
		break;
	case ATTACK:
		Attack(_deltaTick, _pNavi);
		break;
	case DIE:
		Die(_deltaTick, _pNavi);
		break;
	case WAIT:
		Wait(_deltaTick, _pNavi);
		break;
	}
}

void CBigSlimeLeaf::Idle(float _deltaTick, CNavigation* _pNavi)
{
	if (m_target == nullptr)
	{
		CheckMonsterInRange(_pNavi);
	}
	else if (m_target != nullptr)
	{
		if (m_attackTick == 0.0f) m_state = ATTACK;
		m_attackTick += _deltaTick;
		if (m_attackTick >= 2.0f) m_attackTick = 0.0f;
	}
	if (m_target == nullptr)
	{
		if (Random(100, 0) == 0)
		{
			SetNextDestination(_pNavi);
		}
	}
}

void CBigSlimeLeaf::TargetRun(float _deltaTick, CNavigation* _pNavi)
{
	if (Distance(m_currentPosition, m_firstHitPosition) >= 40.0f)
	{
		_pNavi->Add(m_currentPosition, m_firstHitPosition, &m_path, m_pMap->GetMapGrid()->GetWalkable());

		//if (m_path.size() > 0)
		//{
		//	m_destinationPosition = m_firstHitPosition;
		//	m_target = nullptr;
		//	m_state = RUN;
		//	m_pathIndex = 1;
		//	SendPacketMove();
		//}
		return;
	}

	if (Distance(*m_target->GetPosition(), m_currentPosition) < 1.5f)
	{
		m_target->SendPacket_Hit(m_damage);
		m_state = ATTACK;
		m_pathIndex = 0;
		return;
	}

	if (m_path.size() == 0) return;
	float distance = Distance(m_path[m_pathIndex], m_currentPosition);
	int sector;

	if (distance >= 0.4f && distance < m_distance)
	{
		m_distance = distance;
		m_currentPosition.x += (m_unitVector.x * 1.0f * _deltaTick);
		m_currentPosition.z += (m_unitVector.z * 1.0f * _deltaTick);
	}
	else
	{
		sector = (static_cast<int>(m_currentPosition.x) / sector_size) + (static_cast<int>(m_currentPosition.z) / sector_size) * sector_line;
		if (sector != m_currentSector)
		{
			SendPacketExitSector(m_currentSector, sector);
			SendPacketEnterSector(sector, m_currentSector);
			m_currentSector = sector;
			m_pSector = m_pMap->GetSector(m_currentSector);
		}

		m_distance = DISTANCE;
		m_currentPosition = m_path[m_pathIndex];
		m_pathIndex++;

		if (m_pathIndex >= m_path.size())
		{
			TarGetDestination(_pNavi);
			m_pathIndex = 0;
		}
		else
		{
			SendPacketMove();
			SetUnitVector();
		}
	}
}

void CBigSlimeLeaf::CheckMonsterInRange(CNavigation* _pNavi)
{
	for (int i = 0; i < 9; i++)
	{
		std::map<SOCKET, CUser*> userList = m_pSector->GetAdjacentGetMap(i);
		CMap* map = CFieldManager::GetInstance()->GetMap(0);

		for (auto user : userList)
		{
			if (Distance(m_currentPosition, *user.second->GetPosition()) < 20.0f)
			{
				m_firstHitPosition = m_currentPosition;
				m_target = user.second;
				TarGetDestination(_pNavi);
				return;
			}
		}
	}
}
