#include <WinSock2.h>
#include "Monster.h"
#include "Map.h"
#include "UserManager.h"
#include <random>

#define DISTANCE 100.0f

CMonster::CMonster() : 
	m_unitVector({ 0, 0, 0 }), 
	m_currentPosition({ 0, 0, 0 }), 
	m_destinationPosition({ 0, 0, 0 }), 
	m_rangeMin({ 0,0 }),
	m_rangeMax({ 0,0 }),
	m_type(0),
	m_isMove(false), 
	m_pathIndex(0)
{
	m_count = Random(100, 0);
	m_currentSector = (static_cast<int>(m_currentPosition.x) / 18) + (static_cast<int>(m_currentPosition.z) / 18) * 15;

	CMap::GetInstance()->Add(this, m_currentSector);
}

CMonster::CMonster(VECTOR3 _position, VECTOR2_INT _rangeMin, VECTOR2_INT _rangeMax, int _type, int _index) :
	m_unitVector({ 0, 0, 0 }), 
	m_currentPosition(_position), 
	m_destinationPosition(_position), 
	m_rangeMin(_rangeMin),
	m_rangeMax(_rangeMax),
	m_type(_type),
	m_isMove(false), 
	m_index(_index), 
	m_pathIndex(0)
{
	m_count = Random(100, 0);
	m_currentSector = (static_cast<int>(m_currentPosition.x) / 18) + (static_cast<int>(m_currentPosition.z) / 18) * 15;
	m_pSector = CMap::GetInstance()->GetSector(m_currentSector);
	CMap::GetInstance()->Add(this, m_currentSector);
}

CMonster::~CMonster()
{
}

void CMonster::SendPacketCreate() // 수정
{
	PACKET_CREATE_MONSTER packet;
	packet.size = sizeof(PACKET_CREATE_MONSTER);
	packet.type = CS_PT_CREATE_MONSTER;
	packet.index = m_index;
	packet.monsterType = m_type;
	packet.currentPosition = m_currentPosition;

	if (m_isMove) packet.destinationPosition = m_path[m_pathIndex];
	else packet.destinationPosition = m_currentPosition;
	
	m_pSector->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_CREATE_MONSTER));
}

//void CMonster::Move()
void CMonster::SendPacketMove()
{
	PACKET_MOVE_MONSTER packet(sizeof(PACKET_MOVE_MONSTER), CS_PT_MOVE_MONSTER, m_index, m_type, m_currentPosition, m_path[m_pathIndex]);

	m_pSector->SendAll(reinterpret_cast<char*>(&packet), sizeof(PACKET_MOVE_MONSTER));
}

void CMonster::Move(float _deltaTick)
{
	if (!m_isMove) return;
	if (_deltaTick > 0.13) _deltaTick = 0.12;

	float distance = Distance();
	int sector;

	if (distance >= 0.4f && distance < m_distance)
	{
		m_distance = distance;
		m_currentPosition.x += (m_unitVector.x * 2.0f * _deltaTick);
		m_currentPosition.z += (m_unitVector.z * 2.0f * _deltaTick);
	}
	else
	{
		m_distance = DISTANCE;
		m_currentPosition = m_path[m_pathIndex];
		m_pathIndex++;

		if (m_pathIndex >= m_path.size())
		{
			m_state = IDLE;
			m_isMove = false;
			m_currentPosition.x = m_destinationPosition.x;
			m_currentPosition.z = m_destinationPosition.z;
			m_pathIndex = 0;
		}
		else
		{
			SendPacketMove();
			SetUnitVector();
		}

		sector = (static_cast<int>(m_currentPosition.x) / 18) + (static_cast<int>(m_currentPosition.z) / 18) * 15;

		if (sector != m_currentSector)
		{
			SendPacketExitSector(m_currentSector, sector);
			SendPacketEnterSector(sector, m_currentSector);
			m_currentSector = sector;
			m_pSector = CMap::GetInstance()->GetSector(m_currentSector);
		}
	}
}

bool CMonster::CheckMovement()
{
	if (m_isMove) return false;
	m_count = Random(100, 0);
	if (m_count == 0) return true;
	return false;
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
	if (m_isMove) return &m_path[m_pathIndex];

	return &m_currentPosition;
}

float CMonster::Distance()
{
	VECTOR3 goal = m_path[m_pathIndex];
	float dx = goal.x - m_currentPosition.x; // operater 사용해서 한번에 만들어 보자
	float dy = goal.y - m_currentPosition.y;
	float dz = goal.z - m_currentPosition.z;

	return (dx * dx) + (dy * dy) + (dz * dz);
}

int CMonster::Random(int _max, int _min)
{
	std::random_device random;
	std::mt19937 gen(random());
	std::uniform_int_distribution<int> dist(_min, _max);
	return dist(gen);
}

void CMonster::SetNextDestination(bool* _walkable)
{
	if (m_isMove) return;

	int x;
	int z;

	while (true)
	{
		x = m_destinationPosition.x + Random(5, -5);
		z = m_destinationPosition.z + Random(5, -5);
		
		if (_walkable[z * 256 + x] != 1) continue;
		if (x <= m_rangeMax.x && x >= m_rangeMin.x && z <= m_rangeMax.z && z >= m_rangeMin.z)
		{
			m_destinationPosition.x = x;
			m_destinationPosition.z = z;

			m_path = CNavigation::GetInstance()->FindPath(m_currentPosition, m_destinationPosition);

			if (m_path.size() > 1) m_pathIndex = 1;
			break;
		}
	}

	if (m_path.size() != 1)
	{
		SetUnitVector();
		if (Distance() >= 0.4f)
		{
			m_isMove = true;
			m_state = RUN;
		}
		else
		{
			m_isMove = false;
			m_state = IDLE;
		}
	}

	m_distance = DISTANCE;
}

void CMonster::SetUnitVector()
{
	VECTOR3 vector = { m_path[m_pathIndex].x - m_currentPosition.x , m_path[m_pathIndex].y - m_currentPosition.y, m_path[m_pathIndex].z - m_currentPosition.z };

	float mag = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

	m_unitVector = { vector.x / mag, 0, vector.z / mag };
}

void CMonster::SendPacketExitSector(int _sectorA, int _sectorB)
{
	CMap* pMap = CMap::GetInstance();
	PACKET_EXIT_SECTOR_MONSTER packet(sizeof(PACKET_EXIT_SECTOR_MONSTER), CS_PT_EXIT_SECTOR_MONSTER, m_index, m_type);

	pMap->Del(this, _sectorA);
	pMap->DifferenceSend(reinterpret_cast<char*>(&packet), sizeof(PACKET_EXIT_SECTOR_MONSTER), _sectorA, _sectorB);
}

void CMonster::SendPacketEnterSector(int _sectorA, int _sectorB)
{
	CMap* pMap = CMap::GetInstance();

	PACKET_ENTER_SECTOR_MONSTER packet(sizeof(PACKET_ENTER_SECTOR_MONSTER), CS_PT_ENTER_SECTOR_MONSTER, m_index, m_type, m_currentPosition);

	pMap->Add(this, _sectorA);
	pMap->DifferenceSend(reinterpret_cast<char*>(&packet), sizeof(PACKET_ENTER_SECTOR_MONSTER), _sectorA, _sectorB);
}