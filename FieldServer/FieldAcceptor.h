#pragma once
#include "../NetCore/TAcceptor.h"

class CFieldAcceptor : public CTAcceptor
{
private:

public:
	CFieldAcceptor(PCSTR _ip, u_short _port);
	~CFieldAcceptor();

	void Handle(ACCEPT_SOCKET_INFO _socket) override;
};