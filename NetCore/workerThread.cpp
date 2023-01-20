#include "../NetCore/session.h"
#include "workerThread.h"
#include "IOCP.h"
#include <process.h>
#include <stdio.h>
CWorkerThread::CWorkerThread()
{
}

CWorkerThread::~CWorkerThread()
{
}

bool CWorkerThread::Start(int _type)
{
	m_type = _type;
	if (_type == ALL)
	{
		m_threadId = (HANDLE)_beginthreadex(NULL, 0, &CWorkerThread::ThreadFunc, this, 0, NULL);
		if (m_threadId == 0)
		{
			printf("Thread Error\n");
			return false;
		}
		return true;
	}
	else
	{
		m_threadId = (HANDLE)_beginthreadex(NULL, 0, &CWorkerThread::ThreadFunc, this, 0, NULL);
		if (m_threadId == 0)
		{
			printf("Thread Error\n");
			return false;
		}
		return true;
	}
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
	overlapped_ex* overlapped;
	int bin;

	while (1)
	{
		if (!GetQueuedCompletionStatus(hIOCP, &bytesTrans, (PULONG_PTR)&bin, (LPOVERLAPPED*)&overlapped, INFINITE))
		{
			// 실패를 했으면 io작업중 하나가 실패한 것이니 그것을 확인해야 한다
			// 확인을 했으면 그것에 맞는 처리를 해줘야한다.

			printf("%d\n", GetLastError()); // 끊긴거를 100% 알수 없다
			
			continue;
		}

		if (bytesTrans <= 0)
		{
			delete overlapped->session;
		}
		else
		{
			overlapped->session->RecvHandle(bytesTrans);
			
			//printf("socket : %d bytesTrans : %d\n", overlapped->session->GetSocket(), bytesTrans);
		}
	}
}

int CWorkerThread::GetType() { return m_type; }