#include "ThreadManager.h"
#include <process.h>
CThreadManager::CThreadManager()
{

}

CThreadManager::~CThreadManager()
{
	int size = m_thread.size();
	CWorkerThread* pWorkerThread = nullptr;

	for (int i = 0; i < size; i++)
	{
		pWorkerThread = m_thread[i];
		delete pWorkerThread;
	}
	m_thread.clear();
}

bool CThreadManager::Start(int _size)
{
	CWorkerThread* workerThread;
	// 여기는 try catch가 더 좋을거 같다
	for (unsigned int i = 0; i < _size; ++i)
	{
		workerThread = new CWorkerThread();
		if (workerThread->Start())
		{
			m_thread.push_back(workerThread);
		}
	}
	return true;
}