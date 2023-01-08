#pragma once
#include "../NetCore/acceptThread.h"
#include "../NetCore/TcpListener.h"
#include "User.h"
#include <vector>

// 다중 상속을 받든, TcpListener를 포함관계를 같든 구조를 다시 만들자

class CFieldAcceptor : public CAcceptThread, CTcpListener
{
public:
	CFieldAcceptor();
	CFieldAcceptor(PCSTR _ip, u_short _port);
	~CFieldAcceptor();

	bool Start();
	void RunLoop();
};