#include "FieldAcceptor.h"
#include "UserManager.h"
#include <process.h>
#include <stdio.h>

CFieldAcceptor::CFieldAcceptor()
{
}

CFieldAcceptor::CFieldAcceptor(PCSTR _ip, u_short _port)
	: CTcpListener(_ip, _port), CAcceptThread()
{
	
}

CFieldAcceptor::~CFieldAcceptor()
{
}

bool CFieldAcceptor::Start()
{
	if (!CTcpListener::Start()) return false;

	if (!CAcceptThread::tStart()) return false;

	return true;
}

void CFieldAcceptor::RunLoop()
{
	ACCEPT_SOCKET_INFO socketInfo;
	int size = sizeof(sockaddr_in);

	DWORD bytesTrans;

	while (1)
	{
		socketInfo.socket = accept(m_socket, (sockaddr*)&socketInfo.addr, &size);

		printf("socket : %d \n", socketInfo.socket); // 임시

		CUser* user = new CUser(socketInfo);

		CUserManager::GetInstance()->Add(user);
		// 동기화의 문제점
		// OnConnect() 뭔가 설정할게 있다면
	}
}
