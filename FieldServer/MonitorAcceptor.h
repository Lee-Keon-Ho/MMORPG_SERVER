#pragma once
#include "../NetCore/TAcceptor.h"

class CMonitorAcceptor : public CTAcceptor
{
public:
	CMonitorAcceptor(PCSTR _ip, u_short _port);
	~CMonitorAcceptor();

	virtual void OnConnect(ACCEPT_SOCKET_INFO _info);
};