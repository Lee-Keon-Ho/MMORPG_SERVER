#include "TcpListener.h"
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

CTcpListener::CTcpListener()
{
}

CTcpListener::CTcpListener(PCSTR _ip, u_short _port)
{
	m_socket_info.socket = WSASocketW(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	m_socket_info.addr.sin_family = AF_INET;
	inet_pton(AF_INET, _ip, &m_socket_info.addr);
	m_socket_info.addr.sin_port = htons(_port);
	bind(m_socket_info.socket, (sockaddr*)&m_socket_info.addr, sizeof(sockaddr_in));
}

CTcpListener::~CTcpListener()
{
}

void CTcpListener::Start()
{
	listen(m_socket_info.socket, SOMAXCONN);
}