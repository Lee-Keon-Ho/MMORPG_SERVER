#include "TcpListener.h"
#include <WS2tcpip.h>
#include <stdio.h>
#pragma comment (lib, "ws2_32.lib")

CTcpListener::CTcpListener(PCSTR _ip, u_short _port)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed WSAStartup()\n");
	}

	m_socket = WSASocketW(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	m_addr.sin_family = AF_INET;
	inet_pton(AF_INET, _ip, &m_addr.sin_addr);
	m_addr.sin_port = htons(_port);
	bind(m_socket, (sockaddr*)&m_addr, sizeof(sockaddr_in));
}

CTcpListener::~CTcpListener()
{
	closesocket(m_socket);
}

bool CTcpListener::Start()
{
	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}