#include "TAcceptor.h"
#include "IOCP.h"
#include <stdio.h>

CTAcceptor::CTAcceptor(PCSTR _ip, u_short _port) :
	m_listener( _ip, _port )
{

}

CTAcceptor::~CTAcceptor()
{

}

bool CTAcceptor::Start()
{
	if (!m_listener.Start()) return false;
	if (!CThread::Start()) return false;
	return true;
}

void CTAcceptor::RunLoop()
{
	ACCEPT_SOCKET_INFO socketInfo;
	int size = sizeof(sockaddr_in);

	while (1)
	{
		socketInfo.socket = ::accept(m_listener.GetSokcet(), (sockaddr*)&socketInfo.addr, &size);

		onConnect(socketInfo);
	}
}

void CTAcceptor::onConnect(ACCEPT_SOCKET_INFO _info)
{
	OnConnect(_info);
}