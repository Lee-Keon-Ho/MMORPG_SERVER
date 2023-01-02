#include "ThreadManager.h"
#include <process.h>
CThreadManager::CThreadManager()
{

}

CThreadManager::~CThreadManager()
{
}

bool CThreadManager::Init(int _size)
{
	CWorkerThread* workerThread;
	
	for (unsigned int i = 0; i < _size; ++i)
	{
		workerThread = new CWorkerThread();
		workerThread->Start();
		m_thread.push_back(workerThread);
	}
	return true;
}