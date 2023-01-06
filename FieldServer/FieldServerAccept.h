#pragma once
#include "../NetCore/acceptThread.h"
#include "User.h"
#include <vector>

// 다중 상속을 받든, TcpListener를 포함관계를 같든 구조를 다시 만들자

class CFieldServerAccept : public CAcceptThread // CFieldAcceptor
{
public:
	CFieldServerAccept();
	CFieldServerAccept(SOCKET _socket);
	~CFieldServerAccept();

	bool Start();
	static unsigned int _stdcall AcceptFunc(void* _pArgs);
	void RunLoop();
};