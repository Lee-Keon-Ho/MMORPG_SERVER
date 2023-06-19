#pragma once
#include "../NetCore/session.h"
#include "PacketStruct.h"
#include <iostream>
#include <string>

class CSector;

class CUser : public CSession
{
private:
	VECTOR3 m_position;
	VECTOR3 m_endPosition;
	float m_rotationY;
	int m_myNum;
	int m_state;
	int m_prevSector;
	int m_nowSector;
	char* m_name;
	int m_character;

	CSector* m_pSector;
public:
	CUser();
	CUser(ACCEPT_SOCKET_INFO _socketInfo);
	~CUser();

	int PacketHandle() override;
	void SetPosition(VECTOR3 _position);
	void SetEndPosition(VECTOR3 _EndPosition);
	void SetRotationY(float _rotationY);
	void SetNumber(int _num);
	void SetState(int _state);
	void SetPrevSector();
	void SetNowSector(VECTOR3 _vector);
	void SetUser(VECTOR3 _position);
	void SetUser(VECTOR3 _current, VECTOR3 _end, int _state);
	void SetUser(VECTOR3 _current, VECTOR3 _end, float _rotationY, int _state);
	void SetSector();

	VECTOR3* GetPosition();
	VECTOR3* GetEndPosition();
	float GetRotationY();
	int GetNumber();
	int GetState();
	int GetPrevSector();
	int GetNowSector();
	int GetUserCountInSector();
	const char* GetName();
	int GetCharacter();

	bool checkSector();
	void SendSector(char* _buffer, int _size);
	void Infield();
	void LogIn();
	void NewUser();
	void NowPosition();
	void MoveUser();
	void Arrive();
};