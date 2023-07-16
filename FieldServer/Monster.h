#pragma once
#include "VECTOR.h"
#include "PacketStruct.h"
#include "Navigation.h"
#include <vector>

class CSector;
class CMap;

using Path = std::vector<VECTOR3>;

class CMonster
{
	enum eState
	{
		IDLE = 0,
		RUN = 1
	};
private:
	VECTOR3			m_unitVector;
	VECTOR3			m_currentPosition;
	VECTOR3			m_destinationPosition;
	VECTOR2_INT		m_rangeMin;
	VECTOR2_INT		m_rangeMax;
	int				m_index;
	int				m_type;
	Path			m_path;
	int				m_pathIndex;
	int				m_prevSector;
	int				m_currentSector;
	int				m_state;
	int				m_count;

	bool			m_isMove;

	CSector*		m_pSector;
	CMap*			m_pMap;
	float			m_distance;
public:
	CMonster();
	CMonster(VECTOR3 _position, VECTOR2_INT _rangeMin, VECTOR2_INT _rangeMax, int _type, int _index);
	~CMonster();

	void SendPacketCreate();
	void SendPacketMove();

	void SetNextDestination(bool* _walkable);
	void Move(float _deltaTick);
	bool CheckMovement();
	int GetIndex();
	int GetType();
	VECTOR3* GetPosition();
	VECTOR3* GetDestinationPosition();

private:
	float Distance();
	int Random(int _max, int _min);
	void SetUnitVector();
	void SendPacketExitSector(int _sectorA, int _sectorB); // 몬스터가 기존 sector를 나가면
	void SendPacketEnterSector(int _sectorA, int _sectorB); // 몬스터가 새로운 sector에 들어가면
};

