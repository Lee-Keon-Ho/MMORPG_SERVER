#include "FieldServerAccept.h"
#include "UserManager.h"
#include <process.h>
#include <stdio.h>

CFieldServerAccept::CFieldServerAccept()
{
}

CFieldServerAccept::CFieldServerAccept(SOCKET _socket) : CAcceptThread(_socket)
{
}

CFieldServerAccept::~CFieldServerAccept()
{
}

bool CFieldServerAccept::Start()
{
	m_threadId = (HANDLE)_beginthreadex(NULL, 0, &CFieldServerAccept::AcceptFunc, this, 0, NULL);
	if (m_threadId == 0)
	{
		printf("Thread Error\n");
		return false;
	}
	return true;
}

unsigned int _stdcall CFieldServerAccept::AcceptFunc(void* _pArgs)
{
	CFieldServerAccept* thread = (CFieldServerAccept*)_pArgs;

	thread->RunLoop();

	return 0;
}

void CFieldServerAccept::RunLoop()
{
	ACCEPT_SOCKET_INFO socketInfo;
	int size = sizeof(sockaddr_in);

	DWORD bytesTrans;

	//CListener* m_listener;

	while (1)
	{
		socketInfo.socket = accept(m_tcpSocket, (sockaddr*)&socketInfo.addr, &size);

		CUser* user = new CUser(socketInfo);

		CUserManager::GetInstatnce()->Add(user);
		// 동기화의 문제점
		// OnConnect() 뭔가 설정할게 있다면 // player Map을 이용
		//m_listener->Handle(void* _data);
	}
}

