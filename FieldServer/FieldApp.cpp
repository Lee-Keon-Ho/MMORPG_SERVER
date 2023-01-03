#include "FieldApp.h"
#include "../NetCore/IOCP.h"
#include "../NetCore/workerThread.h"
#include "CUser.h"
#include <stdio.h>

#pragma comment (lib, "./../x64/Debug/NetCore.lib")

CFieldApp::CFieldApp() : m_pListener(nullptr), m_pThreadManager(nullptr)
{
}

CFieldApp::~CFieldApp()
{
}

bool CFieldApp::Initialize()
{
	WSADATA wsaData; // NetCore���� Init
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed WSAStartup()\n");
		return false;
	}

	return true;
}

bool CFieldApp::CreateInstance()
{
	if (m_pListener == nullptr) m_pListener = new CTcpListener("183.108.148.83", 30002);
	if (!m_pListener) return false;

	if (m_pThreadManager == nullptr) m_pThreadManager = new CThreadManager();
	if (!m_pThreadManager) return false;

	return true;
}

bool CFieldApp::StartInstance()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	if (!m_pThreadManager->Start(si.dwNumberOfProcessors * 2)) return false;
	if (!m_pListener->Start()) return false;
	
	printf("server start...\n");
	return true;
}

void CFieldApp::RunLoop()
{
	while (true) // 01-03 ������ ����ִ� �ɷ�
	{
		// UserHandler ��Ƽ� ó��
		// ��� user Vector
		while (true)
		{
			//if (user->GetReadSize() > 0) PakcetHandle();

			//if(user->GetReadSize() == 0 ) break;
		}
		

		// accept Thread ������ �� �𸣰ڴ� ��Ȯ�ؾ��Ѵ�.
		
		ACCEPT_SOCKET_INFO socketInfo;
		int size = sizeof(sockaddr_in);

		socketInfo.socket = accept(m_pListener->GetSocket(), (sockaddr*)&socketInfo.addr, &size);

		CUser* pUser = new CUser(socketInfo);
		CIocp::GetInstance()->Associate(socketInfo.socket);

		pUser->Recv();// OnConnect() ���� �����Ұ� �ִٸ� // player Map�� �̿�
	}
}

void CFieldApp::DeleteInstance()
{
	if (m_pListener) { delete m_pListener; m_pListener = nullptr; }
}