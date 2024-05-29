#include "../NetCore/session.h"
#include "workerThread.h"
#include "IOCP.h"
#include <process.h>
#include <stdio.h>
#include <iostream>
#include <exception>
CWorkerThread::CWorkerThread()
{
}

CWorkerThread::~CWorkerThread()
{
}

bool CWorkerThread::Start()
{
	m_threadId = (HANDLE)_beginthreadex(NULL, 0, &CWorkerThread::WorkerThreadFunc, this, 0, NULL);
	if (m_threadId == 0)
	{
		printf("Thread Error\n");
		return false;
	}
	return true;
}

unsigned int _stdcall CWorkerThread::WorkerThreadFunc(void* _pArgs)
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

	while (true)
	{
		if (!GetQueuedCompletionStatus(hIOCP, &bytesTrans, (PULONG_PTR)&bin, (LPOVERLAPPED*)&overlapped, INFINITE))
		{
			int error = GetLastError();
			printf("Error : %d\n", error);
			continue;
		}

		if (bytesTrans <= 0)
		{
			overlapped->session->Delete();
		}
		else
		{
			switch (overlapped->ioType)
			{
			case static_cast<int>(eFlag::RECV):
				overlapped->session->OnRecv(bytesTrans);
				break;
			case static_cast<int>(eFlag::SEND):
				overlapped->session->OnSend();
				break;
			}
		}
	}
}