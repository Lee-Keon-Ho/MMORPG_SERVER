#include "FieldAcceptor.h"
#include "UserManager.h"
#include "User.h"

CFieldAcceptor::CFieldAcceptor(PCSTR _ip, u_short _port)
	: CTAcceptor(_ip, _port)
{
	
}

CFieldAcceptor::~CFieldAcceptor()
{
}
/*
bool CFieldAcceptor::Start()
{
	if (!CTAcceptor::Start()) return false;

	return true;
}
*/
void CFieldAcceptor::Handle(ACCEPT_SOCKET_INFO _socket)
{
	CUser* pUser = new CUser(_socket);
}