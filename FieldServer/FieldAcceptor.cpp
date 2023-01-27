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

		printf("socket : %d \n", socketInfo.socket); // �ӽ�

		CUser* user = new CUser(socketInfo);

		CUserManager::GetInstance()->Add(user);
		// ����ȭ�� ������
		// OnConnect() ���� �����Ұ� �ִٸ�
	}
}
