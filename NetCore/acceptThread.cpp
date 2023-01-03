#include "acceptThread.h"
#include "IOCP.h"
#include "overlapped_ex.h"
#include <process.h>
#include <stdio.h>

CAcceptThread::CAcceptThread()
{
}

CAcceptThread::CAcceptThread(SOCKET _socket) : m_threadId(nullptr), m_tcpSocket(_socket)
{
}

CAcceptThread::~CAcceptThread()
{
}

bool CAcceptThread::Start()
{
	m_threadId = (HANDLE)_beginthreadex(NULL, 0, &CAcceptThread::AcceptFunc, this, 0, NULL);
	if (m_threadId == 0)
	{
		printf("Thread Error\n");
		return false;
	}
	return true;
}

unsigned int _stdcall CAcceptThread::AcceptFunc(void* _pArgs)
{
	CAcceptThread* thread = (CAcceptThread*)_pArgs;

	thread->RunLoop();

	return 0;
}

void CAcceptThread::RunLoop()
{
	ACCEPT_SOCKET_INFO socketInfo;
	int size = sizeof(sockaddr_in);

	HANDLE hIOCP = CIocp::GetInstance()->GetHandle();
	DWORD bytesTrans;
	overlapped_ex overlapped;

	//CListener* m_listener;

	while (1)
	{
		socketInfo.socket = accept(m_tcpSocket, (sockaddr*)&socketInfo.addr, &size);
		// NetCore ¹Û¿¡¼­ ??? CUser user = new CUser(socketInfo);
		//OnAccept();
		//m_listener->Handle(void* _data);
	}
}