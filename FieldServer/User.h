#pragma once
#include "../NetCore/session.h"

struct POSITION
{
	float x;
	float y;
	float z;
};

class CUser : public CSession
{
private:
	POSITION m_position;
	POSITION m_endPosition;
	float m_rotationY;
	int m_myNum;
	int m_state;

public:
	CUser();
	CUser(ACCEPT_SOCKET_INFO _socketInfo);
	~CUser();

	int PacketHandle() override;
	void SetPosition(POSITION _position);
	void SetEndPosition(POSITION _EndPosition);
	void SetRotationY(float _rotationY);
	void SetNumber(int _num);
	void SetState(int _state);

	POSITION* GetPosition();
	POSITION* GetEndPosition();
	float GetRotationY();
	int GetNumber();
	int GetState();
};