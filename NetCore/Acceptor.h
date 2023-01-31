#pragma once
#include "session.h"
#include "Thread.h"

class CAcceptor : public CThread
{
private:
	SOCKET m_socket;

public:
	CAcceptor();
	CAcceptor(SOCKET _socket);
	~CAcceptor();

	bool Start();
	void RunLoop();

	virtual void Handle(ACCEPT_SOCKET_INFO _socket) = 0;
};