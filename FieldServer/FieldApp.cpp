#include "FieldApp.h"
#include "UserManager.h"
#include "MonsterManager.h"
#include "Navigation.h"
#include "Notice.h"
#include "FieldManager.h"
#include <stdio.h>

#ifdef _DEBUG
#pragma comment (lib, "./../x64/Debug/NetCore.lib")
#else
#pragma comment (lib, "./../x64/Release/NetCore.lib")
#endif

#pragma comment (lib, "winmm.lib")

CFieldApp::CFieldApp() : m_pThreadManager(nullptr), m_pFieldAcceptor(nullptr)
{
}

CFieldApp::~CFieldApp()
{
}

bool CFieldApp::Initialize()
{
	WSADATA wsaData; // NetCore에서 Init
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed WSAStartup()\n");
		return false;
	}

	return true;
}

bool CFieldApp::CreateInstance()
{
	CFieldManager::GetInstance();
	CUserManager::GetInstance();
	CMonsterManager::GetInstance();
	CNavigation::GetInstance();
	CNotice::GetInstance();

//	if (!m_pLoginConnector) m_pLoginConn = new CLoginconnector("192.168.1.183", 40001);

	// file에서 받아온다 원래는 이렇게 사용하지 않는다.
	if (!m_pFieldAcceptor) m_pFieldAcceptor = new CFieldAcceptor("112.184.241.183", 30002);
	if (!m_pFieldAcceptor) return false;
	if (!m_pThreadManager) m_pThreadManager = new CThreadManager();
	if (!m_pThreadManager) return false;

	return true;
}

bool CFieldApp::StartInstance()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	if (!m_pThreadManager->Start(si.dwNumberOfProcessors * 2)) return false;
	CNotice::GetInstance()->Start();
	CMonsterManager::GetInstance()->Start();

	printf("server start...\n");
	return true;
}

void CFieldApp::RunLoop()
{
	// 생각하고 수정을 해야한다. 방향성
	//WaitForSingleObject(CMonsterManager::GetInstance()->GetHandle(), INFINITE);
	while (true)
	{
		Sleep(1);
	}
}

void CFieldApp::DeleteInstance()
{
	if (m_pFieldAcceptor) { delete m_pFieldAcceptor; m_pFieldAcceptor = nullptr; }
	if (m_pThreadManager) { delete m_pThreadManager; m_pThreadManager = nullptr; }
}

void CFieldApp::Handle(ACCEPT_SOCKET_INFO _socket)
{
	CUser* pUser = new CUser(_socket);
}