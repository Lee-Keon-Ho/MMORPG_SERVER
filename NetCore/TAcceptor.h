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
	void RunLoop() override;

	virtual void OnConnect(ACCEPT_SOCKET_INFO _info) = 0;

private:
	void onConnect(ACCEPT_SOCKET_INFO _info);
};