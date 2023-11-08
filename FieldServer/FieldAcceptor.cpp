#include "FieldAcceptor.h"
#include "User.h"

CFieldAcceptor::CFieldAcceptor(PCSTR _ip, u_short _port)
	: CTAcceptor(_ip, _port)
{
	CTAcceptor::Start();
}

CFieldAcceptor::~CFieldAcceptor()
{
}

void CFieldAcceptor::Handle(ACCEPT_SOCKET_INFO _socket)
{
	CUser* pUser = new CUser(_socket);
}