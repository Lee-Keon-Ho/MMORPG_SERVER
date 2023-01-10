#pragma once
#include <WinSock2.h>

class CTcpListener
{
protected:
	SOCKET		m_socket;
	SOCKADDR_IN	m_addr;

public:
	CTcpListener();
	CTcpListener(PCSTR _ip, u_short _port);
	~CTcpListener();

	bool Start();
};