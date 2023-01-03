#include "workerThread.h"
#include "IOCP.h"
#include "overlapped_ex.h"
#include <process.h>
#include <stdio.h>
CWorkerThread::CWorkerThread()
{
}

CWorkerThread::~CWorkerThread()
{
}

bool CWorkerThread::Start()
{
	m_threadId = (HANDLE)_beginthreadex(NULL, 0, &CWorkerThread::ThreadFunc, this, 0, NULL);
	if (m_threadId == 0)
	{
		printf("Thread Error\n");
		return false;
	}
	return true;
}

unsigned int _stdcall CWorkerThread::ThreadFunc(void* _pArgs)
{
	CWorkerThread* thread = (CWorkerThread*)_pArgs;

	thread->RunLoop();

	return 0;
}

void CWorkerThread::RunLoop()
{
	HANDLE hIOCP = CIocp::GetInstance()->GetHandle();
	DWORD bytesTrans;
	overlapped_ex overlapped;

	while (1)
	{
		if (!GetQueuedCompletionStatus(hIOCP, &bytesTrans, (PULONG_PTR)&overlapped.session, (LPOVERLAPPED*)&overlapped, INFINITE))
		{
			continue;
		}

		//printf("recv ok %ld : %ld \n", pSession->GetSocket(), bytesTrans);
		overlapped.session->RecvHandle(bytesTrans);
	}
}