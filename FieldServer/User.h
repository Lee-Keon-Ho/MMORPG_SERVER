#pragma once
#include "../NetCore/session.h"
#include "PacketStruct.h"
#include "struct.h"
#include <string>

constexpr int attack = 2;

class CSector;
class CMap;

class CUser : public CSession
{
private:
	VECTOR3		m_endPosition;
	float		m_rotationY;
	int			m_index;
	int			m_state;
	int			m_prevSector;
	int			m_currentSector;
	float		m_maxExp;

	sCharacterInfo m_characterInfo;

	CMap*		m_pMap;
	CSector*	m_pSector;
public:
	CUser();
	CUser(ACCEPT_SOCKET_INFO _socketInfo);
	CUser(sCharacterInfo& _info);
	~CUser();

	int PacketHandle() override;
	void AddExp(int _exp);
	void SetPosition(VECTOR3& _position);
	void SetEndPosition(VECTOR3& _EndPosition);
	void SetRotationY(float _rotationY);
	void SetIndex(int _num);
	void SetPrevSector();
	void SetCurrentSector(VECTOR3& _vector);
	void SetField(int _field);
	void SetInfo(sCharacterInfo& _info);
	void SetInfo(float _rotationY, int _state);
	void SetInfo();
	void SetInfo(VECTOR3& _current, VECTOR3& _end, int _state);
	void SetInfo(VECTOR3& _current, VECTOR3& _end, float _rotationY, int _state);
	void SetSector();

	VECTOR3* GetPosition();
	VECTOR3* GetEndPosition();
	float GetRotationY();
	int GetIndex();
	int GetState();
	int GetPrevSector();
	int GetNowSector();
	int GetUserCountInSector();
	const wchar_t* GetName();
	int GetCharacter();
	int GetDamage();
	int GetField();
	sCharacterInfo& GetCharacterInfo();

	void LogOut();
	void Delete();
	void CheckSectorUpdates();
	void SendSector(LKH::sharedPtr<PACKET> _buffer, int _size);
	void SendPacket_Infield();
	void SendPacket_LogIn();
	void SendPacket_NextField();
	void SendPacket_NewUserEntry();
	void SendPacket_AdjacentSector_NewUserEntry();
	void SendPacket_Move();
	void SendPacket_Arrive();
	void SendPacket_ChannelChange();
	void SendPacket_LogOut();
	void SendPacket_Idle_Attack();
	void SendPacket_Move_Attack();
	void SendPacket_Idle_ArcherAttack();
	void SendPacket_Move_ArcherAttack();
	void SendPacket_EXP();
	void SendPacket_LevelUp();
	void SendPacket_Hit(int _damage);
	void SendPacket_Chatting(wchar_t* _str);
	void SendPacket_MonsterInfo();
};