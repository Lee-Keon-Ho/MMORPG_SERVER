#include "workerThread.h"
#include "IOCP.h"
#include "session.h"
#include <process.h>

CWorkerThread::CWorkerThread()
{
}

CWorkerThread::~CWorkerThread()
{
}

bool CWorkerThread::Start()
{
	//예외 처림? if문?
	HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &CWorkerThread::ThreadFunc, this, 0, NULL);
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
	LPOVERLAPPED overlapped;
	CSession* pSession;

	while (1)
	{
		if (!GetQueuedCompletionStatus(hIOCP, &bytesTrans, (PULONG_PTR)&pSession, (LPOVERLAPPED*)&overlapped, INFINITE))
		{
			//delete pSession; // 갑자기 지우면 안된다.
			//continue;
		}

		//printf("recv ok %ld : %ld \n", pSession->GetSocket(), bytesTrans);

		pSession->RecvHandle(bytesTrans);

		/*if (state < 0)
		{
			delete pSession;
		}*/
	}
}
