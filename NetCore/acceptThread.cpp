#include "acceptThread.h"
#include <stdio.h>

CAcceptThread::CAcceptThread() : m_threadId(nullptr)
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