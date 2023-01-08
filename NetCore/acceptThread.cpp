#include "acceptThread.h"
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

unsigned int _stdcall CAcceptThread::AcceptFunc(void* _pArgs)
{
	CAcceptThread* thread = (CAcceptThread*)_pArgs;

	thread->RunLoop();

	return 0;
}