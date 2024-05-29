#pragma once
#include "../NetCore/sharedPtr.h"
#include "VECTOR.h"
#include "PacketStruct.h"
#include <vector>

class CSector;
class CMap;
class CUser;
class CNavigation;

using Path = std::vector<VECTOR3>;

class CMonster 
{
protected:
	enum eState
	{
		IDLE = 0,
		RUN,
		HIT,
		TARGET_RUN,
		ATTACK,
		DIE,
		WAIT
	};
protected:
	VECTOR3			m_unitVector;
	VECTOR3			m_currentPosition;
	VECTOR3			m_destinationPosition;
	VECTOR3			m_firstHitPosition;
	VECTOR3			m_regenPosition;
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
	int				m_hp;
	int				m_level;
	float			m_regen;
	int				m_exp;
	int				m_damage;
	float			m_attackTick;
	float			m_hitTick;

	CSector*		m_pSector;
	CMap*			m_pMap;
	CUser*			m_target;
	float			m_distance;
public:
	CMonster();
	CMonster(VECTOR3 _position, VECTOR2_INT _rangeMin, VECTOR2_INT _rangeMax, int _type, int _index);
	virtual ~CMonster();

	void SendPacketCreate(CUser* _pUser);
	void SendPacketMove();

	virtual void Update(float _deltaTick, CNavigation* _pNavi);
	void Hit(CUser* _pTarget);

	int GetIndex();
	int GetType();
	VECTOR3* GetPosition();
	VECTOR3* GetDestinationPosition();
	Path* GetPath();

protected:
	virtual void Idle(float _deltaTick, CNavigation* _pNavi);

	void Run(float _deltaTick, CNavigation* _pNavi);
	void Hit(float _deltaTick, CNavigation* _pNavi);
	void Attack(float _deltaTick, CNavigation* _pNavi);
	void Die(float _deltaTick, CNavigation* _pNavi);
	void Wait(float _deltaTick, CNavigation* _pNavi);
	void TarGetDestination(CNavigation* _pNavi);
	void TargetRun(float _deltaTick, CNavigation* _pNavi);
	void SetNextDestination(CNavigation* _pNavi);
	void SetUnitVector();
	void SendPacketAttack();
	void SendPacketRegen();
	void SendPacketExitSector(int _sectorA, int _sectorB); // 몬스터가 기존 sector를 나가면
	void SendPacketEnterSector(int _sectorA, int _sectorB); // 몬스터가 새로운 sector에 들어가면
};