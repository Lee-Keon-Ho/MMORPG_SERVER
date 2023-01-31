#pragma once
#include "TcpListener.h"
#include "Thread.h"
#include "session.h"

class CTAcceptor : public CThread
{
private:
	CTcpListener m_listener;

public:
	CTAcceptor(PCSTR _ip, u_short _port);
	~CTAcceptor();

	bool Start();
	void RunLoop();

	virtual void Handle(ACCEPT_SOCKET_INFO _socket) = 0;
};