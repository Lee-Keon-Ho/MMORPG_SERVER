#pragma once
#include "../NetCore/acceptThread.h"
#include "User.h"
#include <vector>

// ���� ����� �޵�, TcpListener�� ���԰��踦 ���� ������ �ٽ� ������

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