#include "MonitorAcceptor.h"
#include "MonitorConnection.h"

CMonitorAcceptor::CMonitorAcceptor(PCSTR _ip, u_short _port)
	: CTAcceptor(_ip, _port)
{
	CTAcceptor::Start();
}

CMonitorAcceptor::~CMonitorAcceptor()
{
}

void CMonitorAcceptor::OnConnect(ACCEPT_SOCKET_INFO _info)
{
	CMoniterConnection::GetInstance()->SetFieldSockeInfo(_info);
}
