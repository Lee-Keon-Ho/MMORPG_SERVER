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
	overlapped_ex* overlapped;
	int bin;

	while (1)
	{
		if (!GetQueuedCompletionStatus(hIOCP, &bytesTrans, (PULONG_PTR)&bin, (LPOVERLAPPED*)&overlapped, INFINITE))
		{
			// ���и� ������ io�۾��� �ϳ��� ������ ���̴� �װ��� Ȯ���ؾ� �Ѵ�
			// Ȯ���� ������ �װͿ� �´� ó���� ������Ѵ�.

			printf("%d\n", GetLastError()); // ����Ÿ� 100% �˼�����
			
			continue;
		}

		if (bytesTrans <= 0)
		{
			delete overlapped->session;
		}
		else
		{
			printf("socket : %d bytesTrans : %d\n", overlapped->session->GetSocket(), bytesTrans);
			overlapped->session->RecvHandle(bytesTrans);
		}
		
	}
}