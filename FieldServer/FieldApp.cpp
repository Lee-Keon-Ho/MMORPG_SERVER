#include "FieldApp.h"
#include "../NetCore/IOCP.h"
#include "../NetCore/workerThread.h"
#include "UserManager.h"
#include "User.h"
#include <stdio.h>

#pragma comment (lib, "./../x64/Debug/NetCore.lib")

CFieldApp::CFieldApp() : m_pThreadManager(nullptr), m_pFieldAcceptor(nullptr)
{
}

CFieldApp::~CFieldApp()
{
}

bool CFieldApp::Initialize()
{
	WSADATA wsaData; // NetCore¿¡¼­ Init
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed WSAStartup()\n");
		return false;
	}

	return true;
}

bool CFieldApp::CreateInstance()
{
	//if (!m_pFieldAcceptor) m_pFieldAcceptor = new CFieldAcceptor("112.184.241.36", 30002);
	if (!m_pFieldAcceptor) m_pFieldAcceptor = new CFieldAcceptor("220.70.186.44", 30002);
	if (!m_pFieldAcceptor) return false;
	if (m_pThreadManager == nullptr) m_pThreadManager = new CThreadManager();
	if (!m_pThreadManager) return false;

	return true;
}

bool CFieldApp::StartInstance()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	if (!m_pThreadManager->Start(si.dwNumberOfProcessors * 2)) return false;

	printf("server start...\n");
	return true;
}

void CFieldApp::RunLoop()
{
	while (true)
	{
		//CUserManager::GetI0nstance()->OnPacket();
		//CUserManager::GetInstance()->Del();
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