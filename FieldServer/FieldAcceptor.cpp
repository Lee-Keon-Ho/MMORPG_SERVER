#include "../NetCore/IOCP.h"
#include "FieldAcceptor.h"
#include "User.h"
#include "UserManager.h"
#include <iostream>
CFieldAcceptor::CFieldAcceptor(PCSTR _ip, u_short _port)
	: CTAcceptor(_ip, _port)
{

}

CFieldAcceptor::~CFieldAcceptor()
{
}

void CFieldAcceptor::OnConnect(ACCEPT_SOCKET_INFO _socketInfo)
{
	CIocp::GetInstance()->Associate(_socketInfo.socket);
	CUserManager::GetInstance()->Add(_socketInfo);
}