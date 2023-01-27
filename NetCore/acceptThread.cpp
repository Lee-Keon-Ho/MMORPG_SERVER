#include "acceptThread.h"
#include <process.h>
#include <stdio.h>

CAcceptThread::CAcceptThread() : m_threadId(nullptr)
{
}

CAcceptThread::~CAcceptThread()
{
}

bool CAcceptThread::tStart()
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