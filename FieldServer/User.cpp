#include "User.h"
#include "PacketHandler.h"
#include <stdio.h>

CUser::CUser()
{
}

CUser::CUser(ACCEPT_SOCKET_INFO _socketInfo) : CSession(_socketInfo)
{
}

CUser::~CUser()
{
}

int CUser::PacketHandle()
{
	int readSize = CPacketHandler::GetInstance()->Handle(this);

	if (readSize > 100 || readSize < 0)
	{
		printf("readSize Error : %d\n", readSize);
	}

	if (readSize > 0)
	{
		m_ringBuffer->Read(readSize);
		return readSize;
	}

	return 0;
}

void CUser::SetPosition(POSITION _position)
{
	m_position = _position;
}

void CUser::SetEndPosition(POSITION _EndPosition)
{
	m_endPosition = _EndPosition;
}

void CUser::SetRotationY(float _rotationY)
{
	m_rotationY = _rotationY;
}

POSITION* CUser::GetPosition()
{
	return &m_position;
}

POSITION* CUser::GetEndPosition()
{
	return &m_endPosition;
}

float CUser::GetRotationY()
{
	return m_rotationY;
}

void CUser::SetNumber(int _num)
{
	m_myNum = _num;
}

void CUser::SetState(int _state)
{
	m_state = _state;
}

int CUser::GetNumber()
{
	return m_myNum;
}

int CUser::GetState()
{
	return m_state;
}
