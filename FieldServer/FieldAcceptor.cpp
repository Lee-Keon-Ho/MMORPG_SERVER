#include "FieldAcceptor.h"
#include "UserManager.h"
#include <process.h>
#include <stdio.h>

CFieldAcceptor::CFieldAcceptor()
{
}

CFieldAcceptor::CFieldAcceptor(PCSTR _ip, u_short _port)
	: CTcpListener(_ip, _port), CAcceptThread(m_socket)
{
	
}

CFieldAcceptor::~CFieldAcceptor()
{
}

bool CFieldAcceptor::Start()
{
	if (!CTcpListener::Start()) return false;

	m_threadId = (HANDLE)_beginthreadex(NULL, 0, &CFieldAcceptor::AcceptFunc, this, 0, NULL);
	if (m_threadId == 0)
	{
		printf("Thread Error\n");
		return false;
	}

	return true;
}

void CFieldAcceptor::RunLoop()
{
	ACCEPT_SOCKET_INFO socketInfo;
	int size = sizeof(sockaddr_in);

	DWORD bytesTrans;

	//CListener* m_listener;

	while (1)
	{
		socketInfo.socket = accept(m_socket, (sockaddr*)&socketInfo.addr, &size);

		CUser* user = new CUser(socketInfo);

		CUserManager::GetInstance()->Add(user);
		// ����ȭ�� ������
		// OnConnect() ���� �����Ұ� �ִٸ� // player Map�� �̿�
		//m_listener->Handle(void* _data);
	}
}

