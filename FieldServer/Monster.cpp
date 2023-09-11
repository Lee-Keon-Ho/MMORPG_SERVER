#include <WinSock2.h>
#include "Monster.h"
#include "FieldManager.h"
#include "UserManager.h"

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
	m_currentSector = (static_cast<int>(m_currentPosition.x) / SECTOR_SIZE) + (static_cast<int>(m_currentPosition.z) / SECTOR_SIZE) * SECTOR_LINE;

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
	m_currentSector = (static_cast<int>(m_currentPosition.x) / SECTOR_SIZE) + (static_cast<int>(m_currentPosition.z) / SECTOR_SIZE) * SECTOR_LINE;

	m_pMap = CFieldManager::GetInstance()->GetMap(FOREST_HUNT);
	m_pMap->Add(this, m_currentSector);
	m_pSector = m_pMap->GetSector(m_currentSector);
}

CMonster::~CMonster()
{
}

void CMonster::SendPacketCreate(CUser* _pUser)
{
	PACKET_CREATE_MONSTER packet;
	packet.size = sizeof(PACKET_CREATE_MONSTER);
	packet.type = CS_PT_CREATE_MONSTER;
	packet.index = m_index;
	packet.monsterType = m_type;
	packet.level = m_level;
	packet.currentPosition = m_currentPosition;

	if (m_state == RUN) packet.destinationPosition = m_path[m_pathIndex];
	else packet.destinationPosition = m_currentPosition;
	
	_pUser->Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_CREATE_MONSTER));
	//m_pSector->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_CREATE_MONSTER));
}

// sendPacket으로 변경
void CMonster::SendPacketMove()
{
	PACKET_MOVE_MONSTER packet(sizeof(PACKET_MOVE_MONSTER), CS_PT_MOVE_MONSTER, m_index, m_type, m_currentPosition, m_path[m_pathIndex]);
	
	m_pSector->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_MOVE_MONSTER));
}

void CMonster::Update(float _deltaTick)
{
	//if (_deltaTick > 0.13f) _deltaTick = 0.12f;

	// 1. 함수포인터
	// (this->*StateFunc)[RUN]() ;  // 빠르다 단점 다운되면 여기서 원인을 찾아야 함.
	// 
	// 2. state 베이스로 하는 class 호출
	// CState <- CStateIdle
	// m_state->Update() ;  // virtual 단점 - 느리다
	
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

void CMonster::Hit(CUser* _pTarget)
{
	int damage = _pTarget->GetDamage();
	m_firstHitPosition = m_currentPosition;
	m_target = _pTarget;
	m_hp -= damage;

	if (m_hp <= 0)
	{
		m_state = DIE;

		PACKET_DIE_MONSTER packetDie(sizeof(PACKET_DIE_MONSTER), CS_PT_DIE_MONSTER, static_cast<u_short>(m_index), static_cast<u_short>(damage));

		m_pSector->SendAll(reinterpret_cast<char*>(&packetDie), sizeof(PACKET_DIE_MONSTER));
		
		m_target->AddExp(m_exp);
	}
	else
	{
		m_state = HIT;

		PACKET_HIT_MONSTER packet(sizeof(PACKET_HIT_MONSTER), CS_PT_HIT_MONSTER, static_cast<u_short>(m_index), static_cast<u_short>(m_hp), static_cast<u_short>(damage));

		m_pSector->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_HIT_MONSTER));
	}
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

	return &m_currentPosition;
}

void CMonster::Idle(float _deltaTick)
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
		SetNextDestination();
	}
}

void CMonster::Run(float _deltaTick)
{
	//assert(m_isMove);

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

void CMonster::Hit(float _deltaTick)
{
	m_attackTick = 0.0f;
	float distance = Distance(*m_target->GetPosition(), m_currentPosition);
	if (distance > 1.5f)
	{
		TarGetDestination();
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

void CMonster::Attack(float _deltaTick)
{
	if (Distance(*m_target->GetPosition(), m_currentPosition) > 1.5f)
	{
		TarGetDestination();
	}
	else
	{
		SendPacketAttack();
		m_attackTick += _deltaTick;
		m_state = IDLE;

		if (Distance(*m_target->GetPosition(), m_currentPosition) <= 1.5f)
		{
			m_target->SendPacket_Hit(m_damage);
		}
	}
}

void CMonster::Die(float _deltaTick)
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

void CMonster::TarGetDestination()
{
	u_int x;
	u_int z;

	CNavigation* navi = CNavigation::GetInstance();
	bool* walkable = navi->GetWalkable();
	VECTOR3 targetPosition = *m_target->GetPosition(); // Distance()를 확인해야한다.

	if (targetPosition.x - m_currentPosition.x < 0) // VECTOR3 로 빼자
	{
		x = targetPosition.x - 1.5f;
	}
	else if(targetPosition.x - m_currentPosition.x == 0)
	{
		x = targetPosition.x;
	}
	else
	{
		x = targetPosition.x + 1.5f;
	}
	
	if (targetPosition.z - m_currentPosition.z < 0)
	{
		z = targetPosition.z - 1.5f;
	}
	else if (targetPosition.z - m_currentPosition.z == 0)
	{
		z = targetPosition.z;
	}
	else
	{
		z = targetPosition.z + 1.5f;
	}

	if (walkable[z * 256 + x] != 1) return;
	m_destinationPosition = targetPosition;

	m_path = navi->FindPath(m_currentPosition, m_destinationPosition);

	if (m_path.size() > 1)
	{
		m_state = TARGET_RUN;
		m_pathIndex = 1;
		SetUnitVector();
		SendPacketMove();
	}
	else m_pathIndex = 0;

	m_distance = DISTANCE;
}

void CMonster::TargetRun(float _deltaTick)
{
	if (Distance(m_currentPosition, m_firstHitPosition) >= 10.0f)
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

void CMonster::SetNextDestination()
{
	u_int x;
	u_int z;
	CNavigation* navi = CNavigation::GetInstance();
	bool* walkable = navi->GetWalkable();

	while (true)
	{
		x = static_cast<u_int>(m_destinationPosition.x + Random(5, -5));
		z = static_cast<u_int>(m_destinationPosition.z + Random(5, -5));

		if (walkable[z * 256 + x] != 1) continue;
		if (x <= m_rangeMax.x && x >= m_rangeMin.x && z <= m_rangeMax.z && z >= m_rangeMin.z)
		{
			m_destinationPosition.x = static_cast<float>(x);
			m_destinationPosition.z = static_cast<float>(z);
			m_path = navi->FindPath(m_currentPosition, m_destinationPosition);

			if (m_path.size() > 1) m_pathIndex = 1;
			else m_pathIndex = 0;
			break;
		}
	}

	if (m_path.size() > 1 && m_path.size() < 100)
	{
		SetUnitVector();
		if (Distance(m_path[m_pathIndex], m_currentPosition) >= 0.4f)
		{
			m_state = RUN;
			SendPacketMove();
		}
	}

	m_distance = DISTANCE;
}

void CMonster::SetUnitVector()
{
	VECTOR3 vector = { m_path[m_pathIndex].x - m_currentPosition.x , m_path[m_pathIndex].y - m_currentPosition.y, m_path[m_pathIndex].z - m_currentPosition.z };

	float mag = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

	m_unitVector = { vector.x / mag, 0.0f, vector.z / mag };
}

void CMonster::SendPacketAttack()
{
	PACKET_ATTACK_MONSTER packet(sizeof(PACKET_ATTACK_MONSTER), CS_PT_ATTACK_MONSTER, static_cast<u_short>(m_index), *m_target->GetPosition());

	m_pSector->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_ATTACK_MONSTER));
}

void CMonster::SendPacketRegen()
{
	PACKET_REGEN_MONSTER packet(sizeof(PACKET_REGEN_MONSTER), CS_PT_REGEN_MONSTER, static_cast<u_short>(m_index), static_cast<u_short>(m_type), static_cast<u_short>(m_level),m_regenPosition);

	m_pSector->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_REGEN_MONSTER));
}

void CMonster::SendPacketExitSector(int _sectorA, int _sectorB)
{
	PACKET_EXIT_SECTOR_MONSTER packet(sizeof(PACKET_EXIT_SECTOR_MONSTER), CS_PT_EXIT_SECTOR_MONSTER, m_index, m_type);

	m_pMap->Del(this, _sectorA);
	m_pMap->DifferenceSend(reinterpret_cast<char*>(&packet), sizeof(PACKET_EXIT_SECTOR_MONSTER), _sectorA, _sectorB);
}

void CMonster::SendPacketEnterSector(int _sectorA, int _sectorB)
{
	PACKET_ENTER_SECTOR_MONSTER packet(sizeof(PACKET_ENTER_SECTOR_MONSTER), CS_PT_ENTER_SECTOR_MONSTER, m_index, m_type, m_currentPosition, m_path[m_pathIndex]);
	
	m_pMap->Add(this, _sectorA);
	m_pMap->DifferenceSend(reinterpret_cast<char*>(&packet), sizeof(PACKET_ENTER_SECTOR_MONSTER), _sectorA, _sectorB);
}