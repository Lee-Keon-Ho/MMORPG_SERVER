#pragma once
#include "../NetCore/acceptThread.h"
#include "../NetCore/TcpListener.h"
#include "User.h"
#include <vector>

// ���� ����� �޵�, TcpListener�� ���԰��踦 ���� ������ �ٽ� ������

class CFieldAcceptor : public CAcceptThread, CTcpListener
{
public:
	CFieldAcceptor();
	CFieldAcceptor(PCSTR _ip, u_short _port);
	~CFieldAcceptor();

	bool Start();
	void RunLoop();
};