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
public:
	CUser();
	CUser(ACCEPT_SOCKET_INFO _socketInfo);
	~CUser();

	int PacketHandle() override;
	void SetPosition(POSITION _position);
};