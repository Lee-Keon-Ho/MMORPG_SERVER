#include "Monster.h"
#include "User.h"
#include "FieldManager.h"
#include "UserManager.h"
#include "Navigation.h"

#define DISTANCE 100.0f
#define REGEN 10.0f

CMonster::CMonster() : 
	m_unitVector({ 0.0f, 0.0f, 0.0f }), 
	m_currentPosition({ 0.0f, 0.0f, 0.0f }), 
	m_firstHitPosition({ 0.0f, 0.0f, 0.0f }),
	m_regenPosition({ 0.0f, 0.0f, 0.0f }),
	m_destinationPosition({ 0.0f, 0.0f, 0.0f }), 
	m_rangeMin({ 0,0 }),
	m_rangeMax({ 0,0 }),
	m_type(0),
	m_index(0),
	m_pSector(nullptr),
	m_pMap(nullptr),
	m_target(nullptr),
	m_pathIndex(0),
	m_hp(0),
	m_regen(0.0f),
	m_level(0),
	m_exp(0),
	m_damage(0),
	m_attackTick(0.0f),
	m_hitTick(0.0f),
	m_distance(DISTANCE),
	m_state(0),
	m_prevSector(0)
{
	m_count = Random(100, 0);
	m_currentSector = (static_cast<int>(m_currentPosition.x) / sector_size) + (static_cast<int>(m_currentPosition.z) / sector_size) * sector_line;

	m_pMap = CFieldManager::GetInstance()->GetMap(FOREST_HUNT);
	m_pMap->Add(this, m_currentSector);
}

CMonster::CMonster(VECTOR3 _position, VECTOR2_INT _rangeMin, VECTOR2_INT _rangeMax, int _type, int _index) :
	m_unitVector({ 0.0f, 0.0f, 0.0f }), 
	m_currentPosition(_position), 
	m_firstHitPosition({ 0.0f, 0.0f, 0.0f }),
	m_regenPosition(_position),
	m_destinationPosition(_position), 
	m_rangeMin(_rangeMin),
	m_rangeMax(_rangeMax),
	m_type(_type),
	m_index(_index),
	m_pSector(nullptr),
	m_pMap(nullptr),
	m_target(nullptr),
	m_pathIndex(0),
	m_hp(10),
	m_regen(0.0f),
	m_level(1),
	m_exp(5),
	m_damage(3),
	m_attackTick(0.0f),
	m_hitTick(0.0f),
	m_distance(DISTANCE),
	m_state(0),
	m_prevSector(0)
{
	m_count = Random(100, 0);
	m_currentSector = (static_cast<int>(m_currentPosition.x) / sector_size) + (static_cast<int>(m_currentPosition.z) / sector_size) * sector_line;

	if (m_type == 5)
	{
		m_pMap = CFieldManager::GetInstance()->GetMap(WINTER_HUNT);
		m_level = 3;
		m_hp = 20;
	}
	else if (m_type == 1)
	{
		m_level = 2;
		m_hp = 15;
		m_pMap = CFieldManager::GetInstance()->GetMap(FOREST_HUNT);
	}
	else m_pMap = CFieldManager::GetInstance()->GetMap(FOREST_HUNT);

	m_pMap->Add(this, m_currentSector);
	m_pSector = m_pMap->GetSector(m_currentSector);
}

CMonster::~CMonster()
{
}

void CMonster::SendPacketCreate(CUser* _pUser)
{
	if (m_state == RUN)
	{
		LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_CREATE_MONSTER(m_index, m_type, m_level, m_currentPosition, m_path[m_pathIndex]);
		_pUser->Send(packet, sizeof(FS2C_PACKET_CREATE_MONSTER));
	}
	else
	{
		LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_CREATE_MONSTER(m_index, m_type, m_level, m_currentPosition, m_currentPosition);
		_pUser->Send(packet, sizeof(FS2C_PACKET_CREATE_MONSTER));
	}
}

void CMonster::SendPacketMove()
{	
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_MOVE_MONSTER(m_index, m_type, m_currentPosition, m_path[m_pathIndex]);

	m_pSector->SendAll(packet, sizeof(FS2C_PACKET_MOVE_MONSTER));
}

void CMonster::Update(float _deltaTick, CNavigation* _pNavi)
{
	switch (m_state) 
	{
	case IDLE:
		Idle(_deltaTick, _pNavi);
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

void CMonster::Hit(CUser* _pTarget)
{
	int damage = _pTarget->GetDamage();
	m_firstHitPosition = m_currentPosition;
	m_target = _pTarget;
	m_hp -= damage;

	if (m_hp <= 0)
	{
		m_state = DIE;

		LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_DIE_MONSTER(static_cast<u_short>(m_index), static_cast<u_short>(damage));

		m_pSector->SendAll(packet, sizeof(FS2C_PACKET_DIE_MONSTER));
		
		m_target->AddExp(m_exp);
		m_target = nullptr;
	}
	else
	{
		if (m_state != TARGET_RUN) m_state = HIT;

		LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_HIT_MONSTER(static_cast<u_short>(m_index), static_cast<u_short>(m_hp), static_cast<u_short>(damage));

		m_pSector->SendAll(packet, sizeof(FS2C_PACKET_HIT_MONSTER));
	}
}

void CMonster::Wait(float _deltaTick, CNavigation* _pNavi)
{
	if (m_path.size() == 0) m_state = WAIT;

	if (m_target == nullptr)
	{
		if (m_path.size() > 1 && m_path.size() < 100)
		{
			m_pathIndex = 1;
			SetUnitVector();
			if (Distance(m_path[m_pathIndex], m_currentPosition) >= 0.4f)
			{
				m_state = RUN;
				SendPacketMove();
			}
		}
		else
		{
			m_state = IDLE;
		}
	}
	else
	{
		if (m_path.size() > 1 && m_path.size() < 100)
		{
			m_state = TARGET_RUN;
			m_pathIndex = 1;
			SetUnitVector();
			SendPacketMove();
		}
		else
		{
			m_state = ATTACK;
		}
	}

	m_distance = DISTANCE;
}

int CMonster::GetIndex()
{
	return m_index;
}

int CMonster::GetType()
{
	return m_type;
}

VECTOR3* CMonster::GetPosition()
{
	return &m_currentPosition;
}

VECTOR3* CMonster::GetDestinationPosition()
{
	if (m_state == RUN) return &m_path[m_pathIndex];
	else if(m_state == IDLE) return &m_currentPosition;
	else return &m_destinationPosition;
}

Path* CMonster::GetPath()
{
	return &m_path;
}

void CMonster::Idle(float _deltaTick, CNavigation* _pNavi)
{
	if (m_target != nullptr)
	{
		if (m_attackTick >= 2.0f)
		{
			m_state = ATTACK;
			m_attackTick = 0.0f;
		}
		else m_attackTick += _deltaTick;
	}
	else if (Random(100, 0) == 0)
	{
		SetNextDestination(_pNavi);
	}
}

void CMonster::Run(float _deltaTick, CNavigation* _pNavi)
{
	if (m_pathIndex == 0) return;
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
			m_state = IDLE;
			m_currentPosition.x = m_destinationPosition.x;
			m_currentPosition.z = m_destinationPosition.z;
			m_pathIndex = 0;
		}
		else
		{
			SendPacketMove();
			SetUnitVector();
		}
	}
}

void CMonster::Hit(float _deltaTick, CNavigation* _pNavi)
{
	m_attackTick = 0.0f;
	float distance = Distance(*m_target->GetPosition(), m_currentPosition);
	if (distance > 1.0f)
	{
		TarGetDestination(_pNavi);
	}
	else
	{
		m_hitTick += _deltaTick;
		if (m_hitTick >= 2.0f)
		{
			m_state = ATTACK;
			m_hitTick = 0.0f;
		}
		
	}
}

void CMonster::Attack(float _deltaTick, CNavigation* _pNavi)
{
	if (Distance(*m_target->GetPosition(), m_currentPosition) > 1.2f)
	{
		TarGetDestination(_pNavi);
	}
	else
	{
		SendPacketAttack();
		m_attackTick += _deltaTick;
		m_state = IDLE;

		if (Distance(*m_target->GetPosition(), m_currentPosition) <= 2.0f)
		{
			m_target->SendPacket_Hit(m_damage);
		}
	}
}

void CMonster::Die(float _deltaTick, CNavigation* _pNavi)
{
	m_regen += _deltaTick;
	if (m_regen >= REGEN)
	{
		m_pathIndex = 0;
		m_target = nullptr;
		m_state = IDLE;
		m_regen = 0.0f;
		m_hp = 10;
		SendPacketRegen();
	}
}

void CMonster::TarGetDestination(CNavigation* _pNavi)
{
	u_int x;
	u_int z;

	VECTOR3 targetPosition = *m_target->GetPosition(); // Distance()를 확인해야한다.

	//if (targetPosition.x - m_currentPosition.x < 0) // VECTOR3 로 빼자
	//{
	//	x = targetPosition.x - 1.0f;
	//}
	//else if(targetPosition.x - m_currentPosition.x == 0)
	//{
	//	x = targetPosition.x;
	//}
	//else
	//{
	//	x = targetPosition.x + 1.0f;
	//}
	//
	//if (targetPosition.z - m_currentPosition.z < 0)
	//{
	//	z = targetPosition.z - 1.0f;
	//}
	//else if (targetPosition.z - m_currentPosition.z == 0)
	//{
	//	z = targetPosition.z;
	//}
	//else
	//{
	//	z = targetPosition.z + 1.0f;
	//}

	m_destinationPosition = targetPosition;
	
	_pNavi->Add(m_currentPosition, m_destinationPosition, &m_path, m_pMap->GetMapGrid()->GetWalkable());
	m_state = WAIT;
}

void CMonster::TargetRun(float _deltaTick, CNavigation* _pNavi)
{
	if (Distance(m_currentPosition, m_firstHitPosition) >= 150.0f)
	{
		m_destinationPosition = m_firstHitPosition;
		m_target = nullptr;
		_pNavi->Add(m_currentPosition, m_destinationPosition, &m_path, m_pMap->GetMapGrid()->GetWalkable());
		m_state = WAIT;
		return;
	}

	if (Distance(*m_target->GetPosition(), m_currentPosition) <= 1.2f)
	{
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

void CMonster::SetNextDestination(CNavigation* _pNavi)
{
	u_int x;
	u_int z;
	bool* walkable = m_pMap->GetMapGrid()->GetWalkable();

	while (true)
	{
		x = static_cast<u_int>(m_destinationPosition.x + Random(5, -5));
		z = static_cast<u_int>(m_destinationPosition.z + Random(5, -5));

		if (walkable[z * 256 + x] != 1) continue;
		if (x <= m_rangeMax.x && x >= m_rangeMin.x && z <= m_rangeMax.z && z >= m_rangeMin.z)
		{
			m_destinationPosition.x = static_cast<float>(x);
			m_destinationPosition.z = static_cast<float>(z);
			break;
		}
	}
	_pNavi->Add(m_currentPosition, m_destinationPosition, &m_path, walkable);
	m_state = WAIT;
}

void CMonster::SetUnitVector()
{
	VECTOR3 vector = { m_path[m_pathIndex].x - m_currentPosition.x , m_path[m_pathIndex].y - m_currentPosition.y, m_path[m_pathIndex].z - m_currentPosition.z };

	float mag = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

	m_unitVector = { vector.x / mag, 0.0f, vector.z / mag };
}

void CMonster::SendPacketAttack()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_ATTACK_MONSTER(static_cast<u_short>(m_index), *m_target->GetPosition());
	m_pSector->SendAll(packet, sizeof(FS2C_PACKET_ATTACK_MONSTER));
}

void CMonster::SendPacketRegen()
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_REGEN_MONSTER(static_cast<u_short>(m_index), static_cast<u_short>(m_type), static_cast<u_short>(m_level), m_regenPosition);

	m_pSector->SendAll(packet, sizeof(FS2C_PACKET_REGEN_MONSTER));
}

void CMonster::SendPacketExitSector(int _sectorA, int _sectorB)
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_EXIT_SECTOR_MONSTER(m_index, m_type);

	m_pMap->Del(this, _sectorA);
	m_pMap->DifferenceSend(packet, sizeof(FS2C_PACKET_EXIT_SECTOR_MONSTER), _sectorA, _sectorB);
}

void CMonster::SendPacketEnterSector(int _sectorA, int _sectorB)
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_ENTER_SECTOR_MONSTER(m_index, m_type, m_currentPosition, m_path[m_pathIndex]);


	m_pMap->Add(this, _sectorA);
	m_pMap->DifferenceSend(packet, sizeof(FS2C_PACKET_ENTER_SECTOR_MONSTER), _sectorA, _sectorB);
}