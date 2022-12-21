#pragma once
#include <WinSock2.h>

struct ACCEPT_SOCKET_INFO
{
	SOCKET		socket;
	SOCKADDR_IN	addr;
};

class CTcpListener
{
protected:
	ACCEPT_SOCKET_INFO m_socket_info;

public:
	CTcpListener();
	CTcpListener(PCSTR _ip, u_short _port);
	~CTcpListener();

	void Start();

	SOCKET GetSocket() { return m_socket_info.socket; }
};