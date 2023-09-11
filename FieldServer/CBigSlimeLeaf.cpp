#include "CBigSlimeLeaf.h"
#include "Sector.h"
#include "FieldManager.h"

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

void CBigSlimeLeaf::Update(float _deltaTick)
{
	switch (m_state)
	{
	case IDLE:
		Idle(_deltaTick); // _deltaTick
		break;
	case RUN:
		Run(_deltaTick);
		break;
	case HIT:
		Hit(_deltaTick);
		break;
	case TARGET_RUN:
		TargetRun(_deltaTick);
		break;
	case ATTACK:
		Attack(_deltaTick);
		break;
	case DIE:
		Die(_deltaTick);
		break;
	}
}

void CBigSlimeLeaf::Idle(float _deltaTick)
{
	if (m_target == nullptr)
	{
		CheckMonsterInRange();
	}
	else if (m_target != nullptr)
	{
		if (m_attackTick == 0.0f) m_state = ATTACK;
		m_attackTick += _deltaTick;
		if (m_attackTick >= 2.0f) m_attackTick = 0.0f;
	}
	else if(Random(100, 0) == 0)
	{
		SetNextDestination();
	}
}

void CBigSlimeLeaf::TargetRun(float _deltaTick)
{
	if (Distance(m_currentPosition, m_firstHitPosition) >= 40.0f)
	{
		m_path = CNavigation::GetInstance()->FindPath(m_currentPosition, m_firstHitPosition);

		if (m_path.size() > 1)
		{
			m_destinationPosition = m_firstHitPosition;
			m_target = nullptr;
			m_state = RUN;
			m_pathIndex = 1;
			SendPacketMove();
		}
		return;
	}

	if (Distance(*m_target->GetPosition(), m_currentPosition) < 1.5f)
	{
		m_target->SendPacket_Hit(m_damage);
		m_state = ATTACK;
		m_pathIndex = 0;
		return;
	}

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
		sector = (static_cast<int>(m_currentPosition.x) / SECTOR_SIZE) + (static_cast<int>(m_currentPosition.z) / SECTOR_SIZE) * SECTOR_LINE;
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
			TarGetDestination();
			m_pathIndex = 0;
		}
		else
		{
			SendPacketMove();
			SetUnitVector();
		}
	}
}

void CBigSlimeLeaf::CheckMonsterInRange()
{
	for (int i = 0; i < 9; i++)
	{
		std::map<SOCKET, CUser*> userList = m_pSector->GetAdjacentGetMap(i);

		for (auto user : userList)
		{
			if (Distance(m_currentPosition, *user.second->GetPosition()) < 20.0f)
			{
				m_firstHitPosition = m_currentPosition;
				m_target = user.second;
				TarGetDestination();
				m_state = TARGET_RUN;
				return;
			}
		}
	}
}
