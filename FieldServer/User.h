#pragma once
#include "../NetCore/session.h"

class CUser : public CSession
{
private:

public:
	CUser();
	CUser(ACCEPT_SOCKET_INFO _socketInfo);
	~CUser();

	int PacketHandle();
};