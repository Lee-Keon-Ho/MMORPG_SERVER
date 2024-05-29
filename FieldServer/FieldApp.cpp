#include "FieldApp.h"
#include "UserManager.h"
#include "MonsterManager.h"
#include "Notice.h"
#include "FieldManager.h"
#include "MonitorConnection.h"
#include <thread>
#include <chrono>
#include <stdio.h>
#include <WS2tcpip.h>
#include "ODBCManager.h"
#pragma comment(lib, "Winmm.lib")

#ifdef _DEBUG
#pragma comment (lib, "./../x64/Debug/NetCore.lib")
#else
#pragma comment (lib, "./../x64/Release/NetCore.lib")
#endif

#pragma comment (lib, "winmm.lib")

CFieldApp::CFieldApp() : 
	m_pThreadManager(nullptr), 
	m_pFieldAcceptor(nullptr), 
	m_pLoginConnector(nullptr),
	m_pMonitorAcceptor(nullptr),
	m_pHeartBeat(nullptr)
{
}

CFieldApp::~CFieldApp()
{
}

bool CFieldApp::Initialize()
{
	return true;
}

bool CFieldApp::CreateInstance()
{
	CFieldManager::GetInstance();
	CUserManager::GetInstance();
	CMonsterManager::GetInstance();
	CNotice::GetInstance();

	if (!m_pFieldAcceptor) m_pFieldAcceptor = std::make_unique<CFieldAcceptor>("112.184.241.183", 30005);
	if (!m_pFieldAcceptor) return false;
	if (!m_pLoginConnector) m_pLoginConnector = std::make_unique<CLoginConnector>("112.184.241.183", 60003);
	if (!m_pLoginConnector) return false;
	/*if(!m_pMonitorAcceptor) m_pMonitorAcceptor = std::make_unique<CMonitorAcceptor>("112.184.241.183", 90003);*/
	//if (!m_pMonitorAcceptor) return false;
	if (!m_pThreadManager) m_pThreadManager = std::make_unique<CThreadManager>();
	if (!m_pThreadManager) return false;
	if (!CODBCManager::GetInstance()->Initialize((SQLWCHAR*)L"account")) return false;
	if (!m_pHeartBeat) m_pHeartBeat = std::make_unique<CHeartBeat>();
	if (!m_pHeartBeat) return false;

	m_hEvent = CreateEvent(nullptr, false, false, nullptr);

	return true;
}

bool CFieldApp::StartInstance()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	if (!m_pLoginConnector.get()->Start()) return false;
	if (!m_pFieldAcceptor.get()->Start()) return false;
	//if (!m_pMonitorAcceptor.get()->Start()) return false;
	if (!m_pThreadManager.get()->Start(si.dwNumberOfProcessors * 2)) return false;

	CNotice::GetInstance()->Start();
	CMonsterManager::GetInstance()->Start();
	//CMoniterConnection::GetInstance()->Start();
	printf("server start...\n");
	return true;
}

void CFieldApp::RunLoop()
{
	DWORD dwResult = 0;

	while (true)
	{
		dwResult = WaitForSingleObject(m_hEvent, wait_time);
		if (dwResult == WAIT_OBJECT_0 || dwResult == WAIT_TIMEOUT)
		{
			m_pHeartBeat.get()->Check();
			ResetEvent(m_hEvent);
		}
		else
		{
			std::cout << "WaitForSingleObject fail Error : " + GetLastError() << std::endl;
		}
	}
}

void CFieldApp::DeleteInstance()
{
	if (m_hEvent != nullptr) CloseHandle(m_hEvent);
}